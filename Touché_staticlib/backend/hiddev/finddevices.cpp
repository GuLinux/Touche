/***********************************************************************
Copyright (c) 2012 "Marco Gulino <marco.gulino@gmail.com>"

This file is part of Touché: https://github.com/rockman81/Touche

Touché is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details (included the COPYING file).

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "finddevices.h"
#include <QFileSystemWatcher>
#include <QMap>
#include "backend/hiddev/hiddev.h"
#include <QDir>
#include <QDebug>
#include "domain/deviceinfo.h"
#include <QTimer>
#include <QThread>
#include <QPair>

#define HIDDEV_PATH "/dev/usb/"
#define DEV_PATH "/dev"

class FindDevicesPrivate {
public:
    FindDevicesPrivate(FindDevices *q) : q_ptr(q) {}
    QFileSystemWatcher fileSystemWatcher;
    QMap<QString, QPair<QThread*, HidDev*> > devices;
    FindDevices * const q_ptr;
    Q_DECLARE_PUBLIC(FindDevices)

    void scanDevices() {
        QDir hiddev(HIDDEV_PATH);
        if(!hiddev.exists()) {
            if(!fileSystemWatcher.directories().contains(DEV_PATH))
                fileSystemWatcher.addPath(DEV_PATH);
            return;
        }
        fileSystemWatcher.removePath(DEV_PATH);
        if(!fileSystemWatcher.directories().contains(HIDDEV_PATH))
            fileSystemWatcher.addPath(HIDDEV_PATH);
        foreach(QFileInfo device, hiddev.entryInfoList(QDir::Files | QDir::System)) {
            addDevice(device.absoluteFilePath());
        }
    }

    void addDevice(const QString &devicePath) {
        Q_Q(FindDevices);
        if(devices.contains(devicePath)) return;
        qDebug() << "Adding device: " << devicePath;
        QThread *deviceThread = new QThread(q);
        HidDev *device = new HidDev(devicePath);
        deviceThread->start();
        device->moveToThread(deviceThread);
        devices.insert(devicePath, QPair<QThread*, HidDev*>(deviceThread, device) );
        q->connect(device, SIGNAL(removed(DeviceInfo*)), SLOT(deviceRemoved(DeviceInfo*)));
        q->connect(device, SIGNAL(event(InputEvent*,DeviceInfo*)), SIGNAL(event(InputEvent*,DeviceInfo*)));
        q->connect(device, SIGNAL(connected(DeviceInfo*)), SIGNAL(connected(DeviceInfo*)));
        QMetaObject::invokeMethod(device, "start", Qt::QueuedConnection);

    }
};

FindDevices::FindDevices(QObject *parent) :
    QObject(parent), d_ptr(new FindDevicesPrivate(this))
{
    Q_D(FindDevices);
    connect(&d->fileSystemWatcher, SIGNAL(directoryChanged(QString)), SLOT(deviceChanged()));
    QTimer::singleShot(300, this, SLOT(deviceChanged()));
}

FindDevices::~FindDevices()
{
    delete d_ptr;
}

void FindDevices::deviceRemoved(DeviceInfo *deviceInfo)
{
    Q_D(FindDevices);
    QPair<QThread*, HidDev*> device = d->devices.take(deviceInfo->path());
    device.first->wait(1000);
    emit disconnected(deviceInfo);
    delete device.first;
    delete device.second;
}

void FindDevices::deviceChanged()
{
    Q_D(FindDevices);
    qDebug() << "Device list changed";
    d->scanDevices();
}

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
#include <QtCore/QTimer>
#include <QThread>
#include <QPair>
#include <QCoreApplication>
#include "backend/config/keyboarddatabase.h"


#define HIDDEV_PATH "/dev/usb/"
#define DEV_PATH "/dev"

typedef QPair<QThread*, HidDev*> DevicePair;
class FindDevicesPrivate {
public:
    FindDevicesPrivate(KeyboardDatabase* keyboardDatabase, FindDevices *q) : keyboardDatabase(keyboardDatabase), q_ptr(q) {}
    QFileSystemWatcher fileSystemWatcher;
    KeyboardDatabase* keyboardDatabase;
    QMap<QString, DevicePair > devices;
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
        HidDev *device = new HidDev(devicePath, keyboardDatabase);
        deviceThread->start();
        device->moveToThread(deviceThread);
        devices.insert(devicePath, QPair<QThread*, HidDev*>(deviceThread, device) );
        q->connect(device, SIGNAL(removed(DeviceInfo*)), SLOT(deviceRemoved(DeviceInfo*)));
        q->connect(device, SIGNAL(noMoreEvents(DeviceInfo*)), SIGNAL(noMoreEvents(DeviceInfo*)));
        q->connect(device, SIGNAL(inputEvent(InputEventP,DeviceInfo*)), SIGNAL(inputEvent(InputEventP,DeviceInfo*)));
        q->connect(device, SIGNAL(connected(DeviceInfo*)), SIGNAL(connected(DeviceInfo*)));
        QMetaObject::invokeMethod(device, "start", Qt::QueuedConnection);

    }
};

FindDevices::FindDevices(KeyboardDatabase* keyboardDatabase, QObject *parent) :
    QObject(parent), d_ptr(new FindDevicesPrivate(keyboardDatabase, this))
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
    qDebug() << "device removed: " << deviceInfo->name();
    Q_D(FindDevices);
    DevicePair device = d->devices.take(deviceInfo->path());
    emit disconnected(deviceInfo);
    delete device.second;
    device.first->quit();
    device.first->wait();
    qDebug() << "cleaning up on FindDevices";
    delete device.first;
    qDebug() << "Devices list is now " << d->devices;
}

void FindDevices::deviceChanged()
{
    Q_D(FindDevices);
    qDebug() << "Device list changed";
    d->scanDevices();
}

void FindDevices::stop()
{
    Q_D(FindDevices);
    foreach( DevicePair devicePair, d->devices.values()) {
        devicePair.second->stop();
    }
}

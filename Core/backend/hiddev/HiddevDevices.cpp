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

#include "HiddevDevices.h"
#include <QFileSystemWatcher>
#include <QMap>
#include "backend/hiddev/hiddev.h"
#include <QDir>
#include <KDebug>
#include "domain/deviceinfo.h"
#include <QtCore/QTimer>
#include <QThread>
#include <QPair>
#include <QCoreApplication>
#include "backend/config/keyboarddatabase.h"


#define HIDDEV_PATH "/dev/usb/"
#define DEV_PATH "/dev"

typedef QPair<QThread*, HidDev*> DevicePair;
class HiddevDevicesPrivate {
public:
    HiddevDevicesPrivate(KeyboardDatabase* keyboardDatabase, HiddevDevices *q) : keyboardDatabase(keyboardDatabase), q_ptr(q) {}
    QFileSystemWatcher fileSystemWatcher;
    KeyboardDatabase* keyboardDatabase;
    QMap<QString, DevicePair > devices;
    HiddevDevices * const q_ptr;
    Q_DECLARE_PUBLIC(HiddevDevices)

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
        Q_Q(HiddevDevices);
        if(devices.contains(devicePath)) return;
        kDebug() << "Adding device: " << devicePath;
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

HiddevDevices::HiddevDevices(KeyboardDatabase* keyboardDatabase, QObject *parent) :
    Device(parent), d_ptr(new HiddevDevicesPrivate(keyboardDatabase, this))
{
    Q_D(HiddevDevices);
    connect(&d->fileSystemWatcher, SIGNAL(directoryChanged(QString)), SLOT(deviceChanged()));
    QTimer::singleShot(300, this, SLOT(deviceChanged()));
}

HiddevDevices::~HiddevDevices()
{
    delete d_ptr;
}

void HiddevDevices::deviceRemoved(DeviceInfo *deviceInfo)
{
    kDebug() << "device removed: " << deviceInfo->name();
    Q_D(HiddevDevices);
    DevicePair device = d->devices.take(deviceInfo->path());
    emit disconnected(deviceInfo);
    delete device.second;
    device.first->quit();
    device.first->wait();
    kDebug() << "cleaning up on HiddevDevices";
    delete device.first;
    kDebug() << "Devices list is now " << d->devices;
}

void HiddevDevices::deviceChanged()
{
    Q_D(HiddevDevices);
    kDebug() << "Device list changed";
    d->scanDevices();
}

void HiddevDevices::stop()
{
    Q_D(HiddevDevices);
    foreach( DevicePair devicePair, d->devices.values()) {
        devicePair.second->stop();
    }
}

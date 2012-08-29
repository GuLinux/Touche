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

#include "DevicesList.h"
#include <QList>

class DevicesListPrivate {
public:
    QList<Device*> devices;
};

DevicesList::DevicesList(QObject *parent) :
    Device(parent), d_ptr(new DevicesListPrivate)
{
}


DevicesList::~DevicesList()
{
    delete d_ptr;
}


DevicesList *DevicesList::add(Device *device)
{
    Q_D(DevicesList);
    d->devices << device;
    connect(device, SIGNAL(connected(DeviceInfo*)), this, SIGNAL(connected(DeviceInfo*)));
    connect(device, SIGNAL(disconnected(DeviceInfo*)), this, SIGNAL(disconnected(DeviceInfo*)));
    connect(device, SIGNAL(inputEvent(InputEventP,DeviceInfo*)), this, SIGNAL(inputEvent(InputEventP,DeviceInfo*)));
    connect(device, SIGNAL(noMoreEvents(DeviceInfo*)), this, SIGNAL(noMoreEvents(DeviceInfo*)));
    return this;
}


DevicesList *DevicesList::remove(Device *device)
{
    Q_D(DevicesList);
    d->devices.removeAll(device);
    device->disconnect(this);
    return this;
}


void DevicesList::deviceChanged()
{
    Q_D(DevicesList);
    foreach(Device *device, d->devices)
        device->deviceChanged();
}


void DevicesList::deviceRemoved(DeviceInfo *deviceInfo)
{
    Q_D(DevicesList);
    foreach(Device *device, d->devices)
        device->deviceRemoved(deviceInfo);
}


void DevicesList::stop()
{
    Q_D(DevicesList);
    foreach(Device *device, d->devices)
        device->stop();
}

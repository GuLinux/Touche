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

#ifndef DEVICESLIST_H
#define DEVICESLIST_H

#include "domain/Device.h"

class DevicesListPrivate;
class DevicesList : public Device
{
    Q_OBJECT
public:
    explicit DevicesList(QObject *parent = 0);
    virtual ~DevicesList();

    DevicesList *add(Device *device);
    DevicesList *remove(Device *device);


public slots:
    virtual void deviceRemoved(DeviceInfo *deviceInfo);
    virtual void deviceChanged();
    virtual void stop();
private:
    DevicesListPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(DevicesList)
};

#endif // DEVICESLIST_H

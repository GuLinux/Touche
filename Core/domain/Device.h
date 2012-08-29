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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "domain/inputevent.h"

class DeviceInfo;
class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);
    virtual ~Device();
signals:
    void inputEvent(InputEventP keyEvent, DeviceInfo *deviceInfo);
    void noMoreEvents(DeviceInfo *deviceInfo);
    void connected(DeviceInfo *deviceInfo);
    void disconnected(DeviceInfo *deviceInfo);

public slots:
    virtual void deviceRemoved(DeviceInfo *deviceInfo);
    virtual void deviceChanged();
    virtual void stop();
    
};

#endif // DEVICE_H

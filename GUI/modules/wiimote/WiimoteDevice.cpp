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

#include "WiimoteDevice.h"
#include "wiimotemanager.h"

WiimoteDevice::WiimoteDevice(WiimoteManager *wiimoteManager, QObject *parent) :
    Device(parent), wiimoteManager(wiimoteManager)
{
}


WiimoteDevice::~WiimoteDevice()
{
}


void WiimoteDevice::deviceRemoved(DeviceInfo *deviceInfo)
{
}


void WiimoteDevice::deviceChanged()
{
}


void WiimoteDevice::stop()
{
}

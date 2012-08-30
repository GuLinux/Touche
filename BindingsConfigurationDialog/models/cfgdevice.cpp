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

#include "cfgdevice.h"
#include "domain/deviceinfo.h"
#include "models/cfgkey.h"
#include <QStringList>
#include <KDebug>

CfgDevice::CfgDevice(DeviceInfo *deviceInfo, QObject *parent) :
    QObject(parent), m_deviceInfo(deviceInfo)
{
    kDebug() << "CfgDevice for " << deviceInfo->name() << ", " << deviceInfo->configID();
    QMap<QString, QVariant> keysMap = deviceInfo->keyboardDatabaseEntry().value("keys").toMap();
    QStringList keys = keysMap.keys();
    foreach(const QString keyName, keys) {
        m_keys << new CfgKey(keyName, keysMap.value(keyName).toMap(), this);
    }
}

QList<CfgKey*> CfgDevice::cfgKeys()
{
    return m_keys;
}


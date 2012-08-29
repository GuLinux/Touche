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

#ifndef CFGDEVICE_H
#define CFGDEVICE_H

#include <QtCore/QObject>
#include <QList>
#include <KDebug>
#include "cfgkey.h"
#include "domain/deviceinfo.h"
class DeviceInfo;
class CfgDevice : public QObject
{
    Q_OBJECT
public:
    explicit CfgDevice(DeviceInfo *deviceInfo, QObject *parent = 0);
    QList<CfgKey*> cfgKeys();

signals:
    
public slots:
private:
    QList<CfgKey*> m_keys;
    DeviceInfo *m_deviceInfo;

public:
    inline friend QDebug operator<< (QDebug s, const CfgDevice* cfgDevice) {
        s.nospace() << "(" << cfgDevice->m_deviceInfo->name() << ", " << cfgDevice->m_keys << ")";
        return s.space();
    }

    inline friend QDebug operator<< (QDebug s, const CfgDevice& cfgDevice) {
        return s << &cfgDevice;
    }
};

#endif // CFGDEVICE_H

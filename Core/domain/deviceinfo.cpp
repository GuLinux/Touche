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

#include "deviceinfo.h"

class DeviceInfoPrivate {
public:
    QString deviceReportedName, path, name;
    quint16 vendor, productID, version;
    quint16 bus, deviceNumber, interfaceNumber;
    QMap<QString, QVariant> keyboardDatabaseEntry;
    QString configurationIdentifier;
};


DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent), d_ptr(new DeviceInfoPrivate())
{
}

DeviceInfo::~DeviceInfo()
{
    delete d_ptr;
}

DeviceInfo *DeviceInfo::name(const QString &name)
{
    Q_D(DeviceInfo);
    d->name=name;
    return this;
}

QString DeviceInfo::name()
{
    Q_D(DeviceInfo);
    if(d->name.isEmpty()) return deviceReportedName();
    return d->name;
}


QString DeviceInfo::deviceReportedName()
{
    Q_D(DeviceInfo);
    return d->deviceReportedName;
}

DeviceInfo *DeviceInfo::deviceReportedName(const QString &deviceReportedName)
{
    Q_D(DeviceInfo);
    d->deviceReportedName = deviceReportedName;
    return this;
}

DeviceInfo *DeviceInfo::path(const QString &path)
{
    Q_D(DeviceInfo);
    d->path=path;
    return this;
}

DeviceInfo *DeviceInfo::vendor(quint16 vendor)
{
    Q_D(DeviceInfo);
    d->vendor=vendor;
    return this;
}

DeviceInfo *DeviceInfo::productID(quint16 productID)
{
    Q_D(DeviceInfo);
    d->productID=productID;
    return this;
}

DeviceInfo *DeviceInfo::version(quint16 version)
{
    Q_D(DeviceInfo);
    d->version=version;
    return this;
}

DeviceInfo *DeviceInfo::bus(quint16 bus)
{
    Q_D(DeviceInfo);
    d->bus=bus;
    return this;
}

DeviceInfo *DeviceInfo::deviceNumber(quint16 deviceNumber)
{
    Q_D(DeviceInfo);
    d->deviceNumber=deviceNumber;
    return this;
}

DeviceInfo *DeviceInfo::interfaceNumber(quint16 interfaceNumber)
{
    Q_D(DeviceInfo);
    d->interfaceNumber=interfaceNumber;
    return this;
}


QString DeviceInfo::path()
{
    Q_D(DeviceInfo);
    return d->path;
}

QString DeviceInfo::configurationIdentifier()
{
    Q_D(DeviceInfo);
    if(d->configurationIdentifier == QString()) {
        d->configurationIdentifier = QString("0x%1:0x%2").arg(d->vendor, 0, 16).arg(d->productID, 0, 16);
    }
    return d->configurationIdentifier;
}

quint32 DeviceInfo::version()
{
    Q_D(DeviceInfo);
    return d->version;
}

quint32 DeviceInfo::bus()
{
    Q_D(DeviceInfo);
    return d->bus;
}

quint32 DeviceInfo::deviceNumber()
{
    Q_D(DeviceInfo);
    return d->deviceNumber;
}

quint32 DeviceInfo::interfaceNumber()
{
    Q_D(DeviceInfo);
    return d->interfaceNumber;
}

QMap<QString, QVariant> DeviceInfo::keyboardDatabaseEntry()
{
    Q_D(DeviceInfo);
    return d->keyboardDatabaseEntry;
}

DeviceInfo *DeviceInfo::keyboardDatabaseEntry(const QMap<QString, QVariant> &keyboardDatabaseEntry)
{
    Q_D(DeviceInfo);
    d->keyboardDatabaseEntry=keyboardDatabaseEntry;
    return this;
}

DeviceInfo *DeviceInfo::configurationIdentifier(const QString &configurationIdentifier)
{
    Q_D(DeviceInfo);
    d->configurationIdentifier=configurationIdentifier;
    return this;
}

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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QtCore/QObject>
#include <QMap>
#include <QVariant>

class DeviceInfoPrivate;
class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = 0);
    ~DeviceInfo();
    DeviceInfo * name(const QString &name);
    DeviceInfo * path(const QString &path);
    DeviceInfo * vendor(quint16 vendor);
    DeviceInfo * productID(quint16 productID);
    DeviceInfo * version(quint16 version);
    DeviceInfo * bus(quint16 bus);
    DeviceInfo * deviceNumber(quint16 deviceNumber);
    DeviceInfo * interfaceNumber(quint16 interfaceNumber);
    DeviceInfo * deviceReportedName(const QString &deviceReportedName);
    DeviceInfo * keyboardDatabaseEntry(const QMap<QString, QVariant> &keyboardDatabaseEntry);
    QString name();
    QString path();
    QString configID();
    quint32 vendor();
    quint32 productID();
    quint32 version();
    quint32 bus();
    quint32 deviceNumber();
    quint32 interfaceNumber();
    QString deviceReportedName();
    QMap<QString, QVariant> keyboardDatabaseEntry();
signals:
    
public slots:
private:
    DeviceInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(DeviceInfo)
};

#endif // DEVICEINFO_H

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

#ifndef KEYBOARDDATABASE_H
#define KEYBOARDDATABASE_H

#include <QObject>
#include <QStringList>

class DeviceInfo;
class KeyboardDatabasePrivate;
class DatabaseEntry;
class KeyboardDatabase : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardDatabase(QStringList databaseFiles, QObject *parent =0);
    ~KeyboardDatabase();
    DatabaseEntry *keyboard(DeviceInfo* deviceInfo);
    const QMap<QString, QVariant> deviceConfiguration(DeviceInfo *deviceInfo);
signals:
    
public slots:
    void deviceAdded(DeviceInfo *device);
    void deviceRemoved(DeviceInfo *device);
private:
    KeyboardDatabasePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(KeyboardDatabase)
};



#endif // KEYBOARDDATABASE_H

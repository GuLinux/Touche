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

#include "keyboarddatabase.h"
#include <qjson/parser.h>
#include <QFile>
#include <QVariantMap>
#include <KDebug>
#include "domain/deviceinfo.h"
#include "domain/inputevent.h"
#include "databaseentry.h"

class KeyboardDatabasePrivate {
public:
    KeyboardDatabasePrivate(QStringList databaseFiles, KeyboardDatabase *parent) : databaseFiles(databaseFiles), q_ptr(parent) {}
    QStringList const databaseFiles;
    QMap<QString, DatabaseEntry*> database;
    DatabaseEntry emptyDatabaseEntry;
    KeyboardDatabase * q_ptr;
    Q_DECLARE_PUBLIC(KeyboardDatabase)

    QMap<QString, QVariant> keyboardConfiguration(DeviceInfo *deviceInfo) {
        QJson::Parser parser;

        foreach(QString database, databaseFiles) {
            QFile databaseFile(database);
            if(!databaseFile.exists())
                continue;
            QVariant config = parser.parse(&databaseFile);
            if(config.toMap().contains(deviceInfo->configID())) {
                QMap<QString, QVariant> thisKeyboardDatabase = config.toMap().value(deviceInfo->configID()).toMap();
                return thisKeyboardDatabase;
            }
        }
        return QMap<QString, QVariant>();
    }
};


KeyboardDatabase::KeyboardDatabase(QStringList databaseFiles, QObject *parent) :
    QObject(parent), d_ptr(new KeyboardDatabasePrivate(databaseFiles, this))
{
    Q_D(KeyboardDatabase);
    qDebug() << "KeyboardDatabasePaths:  "<< databaseFiles;
    d->emptyDatabaseEntry.setProperty("deviceName", "Device not configured");
    kDebug() << "Using keyboard database paths: " << databaseFiles;
}

KeyboardDatabase::~KeyboardDatabase()
{
    delete d_ptr;
}

DatabaseEntry *KeyboardDatabase::keyboard(DeviceInfo *deviceInfo)
{
    Q_D(KeyboardDatabase);
    return d->database.value(deviceInfo->configID(), &d->emptyDatabaseEntry);
}

void KeyboardDatabase::deviceAdded(DeviceInfo *device)
{
    Q_D(KeyboardDatabase);
    QMap<QString, QVariant> thisKeyboardDatabase = d->keyboardConfiguration(device);
    if(thisKeyboardDatabase.isEmpty())
        return;
    d->database.insert(device->configID(), DatabaseEntry::fromConfig(thisKeyboardDatabase, this));
}

void KeyboardDatabase::deviceRemoved(DeviceInfo *device)
{
    Q_D(KeyboardDatabase);
    if(!device) return;
    delete d->database.take(device->configID());
}

const QMap<QString, QVariant> KeyboardDatabase::deviceConfiguration(DeviceInfo *deviceInfo)
{
    Q_D(KeyboardDatabase);
    return d->keyboardConfiguration(deviceInfo);
}



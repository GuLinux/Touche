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

#include "databaseentry.h"

#include "domain/inputevent.h"
#include "domain/configevent.h"
#include "domain/nomoreeventsinputevent.h"
#include <QList>
#include <QDebug>

class DatabaseEntryPrivate {
public:
    QList<ConfigEvent*> configuredEvents;
    void addEvents(const QMap<QString, QVariant> &events, const QString &keyName, QObject *parent) {
        ConfigEvent *configEvent = new ConfigEvent(parent);
        foreach(QString eventName, events.keys()) {
            configEvent->setProperty("keyName", keyName);
            QVariantMap payload = events.value(eventName).toMap();
            configEvent->addInputEvent(eventName, payload);
        }
        configuredEvents << configEvent;
    }
};

DatabaseEntry::DatabaseEntry(QObject *parent) : QObject(parent), d_ptr(new DatabaseEntryPrivate())
{
}

DatabaseEntry::~DatabaseEntry()
{
    delete d_ptr;
}

QList<ConfigEvent *> DatabaseEntry::configuredEvents()
{
    Q_D(DatabaseEntry);
    return d->configuredEvents;
}

DatabaseEntry *DatabaseEntry::fromConfig(QMap<QString, QVariant> config, QObject *parent)
{
    DatabaseEntry* databaseEntry = new DatabaseEntry(parent);
    databaseEntry->setProperty("deviceName", config.value("name").toString());
    QMap<QString, QVariant> keys = config.value("keys").toMap();
    foreach(QString keyName, keys.keys()) {
        QMap<QString, QVariant> events = keys.value(keyName).toMap();
        databaseEntry->d_ptr->addEvents(events, keyName, databaseEntry);
    }
    return databaseEntry;
}

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

#ifndef DATABASEENTRY_H
#define DATABASEENTRY_H

#include <QObject>
#include <QVariant>
#include <QList>

class ConfigEvent;
class DatabaseEntryPrivate;
class InputEvent;
class DatabaseEntry : public QObject {
    Q_OBJECT
public:
    DatabaseEntry(QObject *parent=0);
    static DatabaseEntry *fromConfig(QMap<QString, QVariant> config, QObject *parent);
    ~DatabaseEntry();
    QList<ConfigEvent *> configuredEvents();
private:
    DatabaseEntryPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(DatabaseEntry)
};

#endif // DATABASEENTRY_H

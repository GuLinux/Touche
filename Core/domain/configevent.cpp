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

#include "configevent.h"
#include "inputevent.h"
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <KDebug>
#include "backend/config/bindingsconfig.h"
#include "domain/binding.h"

class BindingsConfig;
class ConfigEventPrivate {
public:
    QMap<QString, QVariantMap> cfgInputEvents;
};

ConfigEvent::ConfigEvent(QObject *parent) :
    QObject(parent), d_ptr(new ConfigEventPrivate())
{
}

ConfigEvent::~ConfigEvent()
{
    delete d_ptr;
}

QDebug operator<<(QDebug dbg, ConfigEvent *c) {
  dbg.space() << c->d_ptr->cfgInputEvents;
  return dbg.space();
}


Binding *ConfigEvent::matches(InputEvent *other, const QStringList &tags, BindingsConfig *bindingsConfig)
{
    Q_D(ConfigEvent);
    foreach(QString tag, tags) {
        QVariantMap payload = d->cfgInputEvents.value(tag);
        kDebug() << "tag: " << ", payload: " << payload;
        if(other->matches(payload)) {
            QString eventName = QString("%1/%2")
                    .arg(property("keyName").toString() )
                    .arg(tag);
            kDebug() << "Got match: " << eventName;
            return bindingsConfig->bindingFor(eventName, other->nearestQObject());
        }
    }
    return 0;
}

void ConfigEvent::addInputEvent(const QString &tag, const QVariantMap &payload)
{
    Q_D(ConfigEvent);
    d->cfgInputEvents.insert(tag, payload);
}

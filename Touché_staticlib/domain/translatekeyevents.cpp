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

#include "translatekeyevents.h"
#include "domain/inputevent.h"
#include <QDebug>
#include <QMap>
#include "domain/binding.h"
#include "domain/runcommandbinding.h"
#include "domain/tokeybinding.h"
#include "backend/config/keyboarddatabase.h"
#include "backend/config/databaseentry.h"
#include "domain/configevent.h"
#include "backend/config/bindingsconfig.h"

class TranslateKeyEventsPrivate {
public:
    TranslateKeyEventsPrivate(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig) : keyboardDatabase(keyboardDatabase), lastMatch(0), bindingsConfig(bindingsConfig) {}
    KeyboardDatabase *keyboardDatabase;
    ConfigEvent *lastMatch;
    BindingsConfig *bindingsConfig;

    bool match(ConfigEvent *configuredEvent, InputEvent *inputEvent, QStringList tagsToMatch) {
        if(!configuredEvent)
            return false;
        bool matches = configuredEvent->matches(inputEvent, tagsToMatch, bindingsConfig);
        if(matches) lastMatch = configuredEvent;
        return matches;
    }
};

TranslateKeyEvents::TranslateKeyEvents(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig, QObject *parent) :
    QObject(parent), d_ptr(new TranslateKeyEventsPrivate(keyboardDatabase, bindingsConfig))
{
}

TranslateKeyEvents::~TranslateKeyEvents()
{
    delete d_ptr;
}

void TranslateKeyEvents::event(InputEvent *keyEvent, DeviceInfo *deviceInfo)
{
    Q_D(TranslateKeyEvents);
    DatabaseEntry *databaseEntry = d->keyboardDatabase->keyboard(deviceInfo);
    if(d->match(d->lastMatch, keyEvent, QStringList("keyrelease"))) {
        d->lastMatch=0;
        return;
    }
    foreach(ConfigEvent *configKeyEvent, databaseEntry->configuredEvents()) {
        if(d->match(configKeyEvent, keyEvent, QStringList("keypress")))
            return;
    }
}

void TranslateKeyEvents::noMoreEvents(DeviceInfo *deviceInfo)
{
    Q_D(TranslateKeyEvents);
    Q_UNUSED(deviceInfo);
    if(!d->lastMatch) return;

    InputEvent * voidInputEvent = new InputEvent(d->lastMatch);
    if(d->match(d->lastMatch, voidInputEvent, QStringList("keyrelease"))) {
        d->lastMatch=0;
    }
}

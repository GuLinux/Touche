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
#include <KDebug>
#include <QMap>
#include "domain/binding.h"
#include "domain/runcommandbinding.h"
#include "domain/tokeybinding.h"
#include "backend/config/keyboarddatabase.h"
#include "backend/config/databaseentry.h"
#include "domain/configevent.h"
#include "backend/config/bindingsconfig.h"
#include "nomoreeventsinputevent.h"

class TranslateKeyEventsPrivate {
public:
    TranslateKeyEventsPrivate(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig, TranslateKeyEvents* parent)
        : keyboardDatabase(keyboardDatabase),
          bindingsConfig(bindingsConfig), suspended(false), q_ptr(parent) {
        resetLastMatch();
    }
    KeyboardDatabase *keyboardDatabase;
    QPair<QStringList,ConfigEvent*> lastMatch;
    inline bool hasLastMatch() { return lastMatch.second!=0; }
    inline void resetLastMatch() { lastMatch = QPair<QStringList,ConfigEvent*>(QStringList(), 0); }
    BindingsConfig *bindingsConfig;
    bool suspended;
    TranslateKeyEvents* const q_ptr;
    Q_DECLARE_PUBLIC(TranslateKeyEvents)

    Binding* match(ConfigEvent *configuredEvent, InputEvent *inputEvent, QStringList tagsToMatch) {
        Q_Q(TranslateKeyEvents);
        if(!configuredEvent)
            return 0;
        Binding *binding = configuredEvent->matches(inputEvent, tagsToMatch, bindingsConfig);
        QPair<QStringList,ConfigEvent*> currentMatch(tagsToMatch, configuredEvent);
        if(binding && lastMatch != currentMatch) {
            lastMatch = currentMatch;
            if(!suspended) binding->execute();
            emit q->keyEvent(configuredEvent->property("keyName").toString());
        }
        return binding;
    }
};

TranslateKeyEvents::TranslateKeyEvents(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig, QObject *parent) :
    QObject(parent), d_ptr(new TranslateKeyEventsPrivate(keyboardDatabase, bindingsConfig, this))
{
}

TranslateKeyEvents::~TranslateKeyEvents()
{
    delete d_ptr;
}

void TranslateKeyEvents::inputEvent(InputEventP keyEvent, DeviceInfo *deviceInfo)
{
    Q_D(TranslateKeyEvents);
    DatabaseEntry *databaseEntry = d->keyboardDatabase->keyboard(deviceInfo);

    if(d->match(d->lastMatch.second, keyEvent, QStringList("keyrelease"))) {
        d->resetLastMatch();
        return;
    }
    foreach(ConfigEvent *configKeyEvent, databaseEntry->configuredEvents()) {
        if(d->match(configKeyEvent, keyEvent, QStringList{"keypress", "keydoubleclick"}))
            return;
    }
}

void TranslateKeyEvents::noMoreEvents(DeviceInfo *deviceInfo)
{
    Q_D(TranslateKeyEvents);
    Q_UNUSED(deviceInfo);
    if(!d->hasLastMatch()) return;

    InputEvent * voidInputEvent = new NoMoreEventsInputEvent(d->lastMatch.second);
    if(d->match(d->lastMatch.second, voidInputEvent, QStringList("keyrelease"))) {
        d->resetLastMatch();
    }
}

void TranslateKeyEvents::resume()
{
    Q_D(TranslateKeyEvents);
    d->suspended=false;
}

void TranslateKeyEvents::suspend()
{
    Q_D(TranslateKeyEvents);
    d->suspended=true;
}

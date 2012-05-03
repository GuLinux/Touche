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
    TranslateKeyEventsPrivate(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig) : keyboardDatabase(keyboardDatabase), bindingsConfig(bindingsConfig) {}
    KeyboardDatabase *keyboardDatabase;
    ConfigEvent *lastMatch = 0;
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

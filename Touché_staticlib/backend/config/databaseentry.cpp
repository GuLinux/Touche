#include "databaseentry.h"

#include "domain/inputevent.h"
#include "domain/configevent.h"
#include <QList>
#include <QDebug>

class DatabaseEntryPrivate {
public:
    QList<ConfigEvent*> configuredEvents;
    void addEvents(const QMap<QString, QVariant> &events, const QString &keyName, QObject *parent) {
        ConfigEvent *configEvent = new ConfigEvent(parent);
        foreach(QString eventName, events.keys()) {
            configEvent->setProperty("keyName", keyName);
            QList<QVariant> registers = events.value(eventName).toList();
            addRegisters(configEvent, registers, eventName);
        }
        configuredEvents << configEvent;
    }

    void addRegisters(ConfigEvent *configEvent, QList<QVariant> configuredRegisters, const QString &eventName) {
        InputEvent *inputEvent = new InputEvent(configEvent);
        foreach(QVariant keyRegister, configuredRegisters) {
            inputEvent->addRegister(keyRegister.toMap().value("hid").toUInt(), keyRegister.toMap().value("value").toUInt());
        }
        inputEvent->setProperty("eventType", eventName);
        configEvent->addInputEvent(eventName, inputEvent);
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
    QMap<QString, QVariant> keys = config.value("keys").toMap();
    foreach(QString keyName, keys.keys()) {
        QMap<QString, QVariant> events = keys.value(keyName).toMap();
        databaseEntry->d_ptr->addEvents(events, keyName, databaseEntry);
    }
    return databaseEntry;
}

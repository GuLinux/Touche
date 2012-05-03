#include "keyboarddatabase.h"
#include <qjson/parser.h>
#include <QFile>
#include <QVariantMap>
#include <QDebug>
#include "domain/deviceinfo.h"
#include "domain/inputevent.h"
#include "databaseentry.h"

class KeyboardDatabasePrivate {
public:
    KeyboardDatabasePrivate(QStringList databaseFiles, KeyboardDatabase *parent) : databaseFiles(databaseFiles), q_ptr(parent) {}
    QStringList const databaseFiles;
    QMap<QString, DatabaseEntry*> database;
    KeyboardDatabase * q_ptr;
    Q_DECLARE_PUBLIC(KeyboardDatabase)
};


KeyboardDatabase::KeyboardDatabase(QStringList databaseFiles, QObject *parent) :
    QObject(parent), d_ptr(new KeyboardDatabasePrivate(databaseFiles, this))
{
}

KeyboardDatabase::~KeyboardDatabase()
{
    delete d_ptr;
}

DatabaseEntry *KeyboardDatabase::keyboard(DeviceInfo *deviceInfo)
{
    Q_D(KeyboardDatabase);
    return d->database.value(deviceInfo->configID());
}

void KeyboardDatabase::deviceAdded(DeviceInfo *device)
{
    Q_D(KeyboardDatabase);
    QJson::Parser parser;

    foreach(QString database, d->databaseFiles) {
        QFile databaseFile(database);
        QVariant config = parser.parse(&databaseFile);
        if(config.toMap().contains(device->configID())) {
            QMap<QString, QVariant> thisKeyboardDatabase = config.toMap().value(device->configID()).toMap();
            d->database.insert(device->configID(), DatabaseEntry::fromConfig(thisKeyboardDatabase, this));
            qDebug() << "Database updated: " << d->database;
            return;
        }
    }
}

void KeyboardDatabase::deviceRemoved(DeviceInfo *device)
{
    Q_D(KeyboardDatabase);
    delete d->database.take(device->configID());
    qDebug() << "Database updated: " << d->database;
}



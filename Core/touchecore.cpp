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

#include "touchecore.h"
#include "domain/inputevent.h"
#include "domain/dumpkeys.h"
#include "domain/translatekeyevents.h"
#include "backend/hiddev/HiddevDevices.h"
#include "domain/deviceinfo.h"
#include "backend/config/keyboarddatabase.h"
#include "backend/config/bindingsconfig.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QProcessEnvironment>
#include <QSettings>
#include "domain/Device.h"
#include "domain/DevicesList.h"


class ToucheCorePrivate {
public:
    ToucheCorePrivate(const QStringList &options) : options(options) { }
    DumpKeys *dumpKeys;
    BindingsConfig *bindingsConfig;
    Device *device;
    KeyboardDatabase *keyboardDatabase;
    TranslateKeyEvents *translateEvents;
    QStringList const options;
};

class __RegisterMetatypes__ {
public:
    __RegisterMetatypes__() {
        qDebug() << "Registering InputEventP";
        qRegisterMetaType<InputEventP>();
    }
};

static __RegisterMetatypes__ __registerMetatypes;

ToucheCore::ToucheCore(Device *device, KeyboardDatabase *keyboardDatabase, const QStringList &options, QObject *parent) :
    QObject(parent), d_ptr(new ToucheCorePrivate(options) )
{
    Q_D(ToucheCore);
    d->bindingsConfig = new BindingsConfig(this);
    d->keyboardDatabase = keyboardDatabase;
    d->device = device;
}

ToucheCore::~ToucheCore()
{
    delete d_ptr;
}

void ToucheCore::start()
{
    Q_D(ToucheCore);
    d->translateEvents = new TranslateKeyEvents(d->keyboardDatabase, d->bindingsConfig, this);

    connect(d->bindingsConfig, SIGNAL(profileChanged(QString)), this, SIGNAL(profileChanged(QString)));
    connect(d->device, SIGNAL(connected(DeviceInfo*)), d->keyboardDatabase, SLOT(deviceAdded(DeviceInfo*)));
    connect(d->device, SIGNAL(disconnected(DeviceInfo*)), d->keyboardDatabase, SLOT(deviceRemoved(DeviceInfo*)));
    connect(d->device, SIGNAL(connected(DeviceInfo*)), this, SIGNAL(connected(DeviceInfo*)));
    connect(d->device, SIGNAL(disconnected(DeviceInfo*)), this, SIGNAL(disconnected(DeviceInfo*)));
    connect(d->translateEvents, SIGNAL(keyEvent(QString)), this, SIGNAL(inputEvent(QString)));
    connect(d->device, SIGNAL(inputEvent(InputEventP,DeviceInfo*)), d->translateEvents, SLOT(inputEvent(InputEventP, DeviceInfo*)));
    connect(d->device, SIGNAL(noMoreEvents(DeviceInfo*)), d->translateEvents, SLOT(noMoreEvents(DeviceInfo*)));
    resumeEventsTranslation();

    if(d->options.contains("--dump-events")) {
        d->dumpKeys = new DumpKeys(this);
        connect(d->device, SIGNAL(inputEvent(InputEventP,DeviceInfo*)), d->dumpKeys, SLOT(inputEvent(InputEventP)));
    }
}


void ToucheCore::suspendEventsTranslation()
{
    Q_D(ToucheCore);
    d->translateEvents->suspend();
}

void ToucheCore::resumeEventsTranslation()
{
    Q_D(ToucheCore);
    d->translateEvents->resume();
}


void ToucheCore::quit()
{
    Q_D(ToucheCore);
    d->device->stop();
}

QMap<QString,QString> ToucheCore::supportedOptions()
{
    QMap<QString, QString> m_supportedOptions;
    m_supportedOptions.insert("dump-events", "dumps raw events (json-formatted, so they can be easly added to keyboard database)");
    return m_supportedOptions;
}


QStringList ToucheCore::availableProfiles() const
{
    Q_D(const ToucheCore);
    return d->bindingsConfig->availableProfiles();
}


QString ToucheCore::currentProfile() const
{
    Q_D(const ToucheCore);
    return d->bindingsConfig->currentProfile();
}


void ToucheCore::setProfile(const QString &profileName)
{
    Q_D(ToucheCore);
    d->bindingsConfig->setCurrentProfile(profileName);
}


QSettings *Touche::settings(QObject *parent)
{
     return new QSettings("GuLinux", "Touche", parent);
}


KeyboardDatabase *ToucheCore::keyboardDatabase()
{
    Q_D(ToucheCore);
    return d->keyboardDatabase;
}

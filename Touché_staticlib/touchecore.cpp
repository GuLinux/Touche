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
#include "backend/hiddev/finddevices.h"
#include "domain/deviceinfo.h"
#include "backend/config/keyboarddatabase.h"
#include "backend/config/bindingsconfig.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QTimer>

#define DATABASE_FILES QStringList(QString("/usr/share/%1/keyboard_database.json").arg(qAppName()))

class ToucheCorePrivate {
public:
    ToucheCorePrivate(const QStringList &options) : options(options) {}
    DumpKeys *dumpKeys;
    BindingsConfig *bindingsConfig;
    FindDevices *findDevices;
    KeyboardDatabase *keyboardDatabase;
    TranslateKeyEvents *translateEvents;
    QStringList const options;
};

ToucheCore::ToucheCore(const QStringList &options, QObject *parent) :
    QObject(parent), d_ptr(new ToucheCorePrivate(options) )
{
}

ToucheCore::~ToucheCore()
{
    delete d_ptr;
}

void ToucheCore::start()
{
    Q_D(ToucheCore);

    if(d->options.contains("--help") || d->options.contains("-h")) {
        QTextStream out(stdout);
        out<< "Usage: " << qAppName() << " [--dump-events]\n";
        out<<"\t--dump-events: dumps raw events (json-formatted, so they can be easly added to keyboard database\n";
        QTimer::singleShot(0, qApp, SLOT(quit()));
        return;
    }

    d->bindingsConfig = new BindingsConfig(this);
    d->keyboardDatabase = new KeyboardDatabase(DATABASE_FILES, this);
    d->translateEvents = new TranslateKeyEvents(d->keyboardDatabase, d->bindingsConfig, this);
    d->findDevices = new FindDevices(this);

    connect(d->findDevices, SIGNAL(connected(DeviceInfo*)), d->keyboardDatabase, SLOT(deviceAdded(DeviceInfo*)));
    connect(d->findDevices, SIGNAL(disconnected(DeviceInfo*)), d->keyboardDatabase, SLOT(deviceRemoved(DeviceInfo*)));
    connect(d->findDevices, SIGNAL(connected(DeviceInfo*)), this, SIGNAL(connected(DeviceInfo*)));
    connect(d->findDevices, SIGNAL(disconnected(DeviceInfo*)), this, SIGNAL(disconnected(DeviceInfo*)));
    connect(d->findDevices, SIGNAL(event(InputEvent*,DeviceInfo*)), d->translateEvents, SLOT(event(InputEvent*, DeviceInfo*)));

    if(d->options.contains("--dump-events")) {
        d->dumpKeys = new DumpKeys(this);
        connect(d->findDevices, SIGNAL(event(InputEvent*,DeviceInfo*)), d->dumpKeys, SLOT(event(InputEvent*)));
    }
}

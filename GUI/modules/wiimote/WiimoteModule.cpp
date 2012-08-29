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

#include "WiimoteModule.h"
#include <KLocale>
#include <KMenu>
#include <QAction>
#include "domain/DevicesList.h"
#include "modules/wiimote/WiimoteDevice.h"

#define HAVE_CWIID
#ifndef HAVE_CWIID

WiimoteModule::WiimoteModule(ToucheCore *toucheCore, KMenu *parentMenu, DevicesList *devicesList, QObject *parent) : QObject(parent) {
    Q_UNUSED(parentMenu)
    Q_UNUSED(toucheCore)
    Q_UNUSED(devicesList)
}
void WiimoteModule::setEnabled(bool enabled) { Q_UNUSED(enabled) }
void WiimoteModule::disconnected() {}
void WiimoteModule::connected() {}
void WiimoteModule::connectWiimote() {}
void WiimoteModule::disconnectWiimote() {}
#else
#include "wiimotemanager.h"

class WiimoteModulePrivate {
public:
    WiimoteModulePrivate(ToucheCore *toucheCore, KMenu *parentMenu)
        : parentMenu(parentMenu), toucheCore(toucheCore) {}
    void enableActions(bool enable) {
        connectAction->setEnabled(enable);
        disconnectAction->setEnabled(enable);
    }

    void toggleActionsVisibility(bool connected) {
        connectAction->setVisible(!connected);
        disconnectAction->setVisible(connected);
    }

    KMenu *parentMenu;
    QAction *titleAction;
    QAction *connectAction;
    QAction *disconnectAction;
    bool isConnected;
    WiimoteManager *wiimoteManager;
    ToucheCore *toucheCore;
};

WiimoteModule::~WiimoteModule()
{
    delete d_ptr;
}

WiimoteModule::WiimoteModule(ToucheCore *toucheCore, KMenu *parentMenu, DevicesList *devicesList, QObject *parent) :
    QObject(parent), d_ptr(new WiimoteModulePrivate(toucheCore, parentMenu))
{
    Q_D(WiimoteModule);
    d->titleAction = parentMenu->addTitle(QIcon::fromTheme("wiimote"), i18n("Wiimote"));
    d->connectAction = parentMenu->addAction(QIcon::fromTheme("network-connect"), i18n("Connect"), this, SLOT(connectWiimote()));
    d->disconnectAction = parentMenu->addAction(QIcon::fromTheme("network-disconnect"), i18n("Disconnect"), this, SLOT(disconnectWiimote()));
    d->disconnectAction->setVisible(false);
    d->wiimoteManager = new WiimoteManager(this);
    connect(d->wiimoteManager, SIGNAL(connected()), this, SLOT(connected()));
    connect(d->wiimoteManager, SIGNAL(disconnected()), this, SLOT(disconnected()));

    devicesList->add(new WiimoteDevice(d->wiimoteManager, this));
}


void WiimoteModule::setEnabled(bool enabled)
{
    Q_D(WiimoteModule);
    if(d->isConnected && enabled) return;
    if(!enabled)
        d->wiimoteManager->disconnect();
    d->parentMenu->removeAction(d->titleAction);

    if(enabled) {
        d->parentMenu->insertAction(d->connectAction, d->titleAction);
    }
    d->titleAction->setVisible(enabled);
    d->connectAction->setVisible(enabled);
    d->disconnectAction->setVisible(false);
}



void WiimoteModule::disconnected()
{
    Q_D(WiimoteModule);
    d->enableActions(true);
    d->toggleActionsVisibility(false);
    d->isConnected = false;
}


void WiimoteModule::connected()
{
    Q_D(WiimoteModule);
    d->enableActions(true);
    d->toggleActionsVisibility(true);
    d->isConnected = true;
}


void WiimoteModule::connectWiimote()
{
    Q_D(WiimoteModule);
    d->enableActions(false);
    d->wiimoteManager->connectWiimote();
}


void WiimoteModule::disconnectWiimote()
{
    Q_D(WiimoteModule);
    d->enableActions(false);
    d->wiimoteManager->disconnectWiimote();
}

#endif // HAVE_CWIID







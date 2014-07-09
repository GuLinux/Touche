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
#include <KAction>
#include "domain/DevicesList.h"
#include "modules/wiimote/WiimoteDevice.h"
#include "touchecore.h"
#include <KActionCollection>
#include <KNotification>
#include <QDBusInterface>
#include <QDebug>

#ifndef HAVE_CWIID

WiimoteModule::WiimoteModule(ToucheCore *, KMenu *, DevicesList *, KActionCollection *, QObject *parent)
    : QObject(parent), d_ptr(0) {}
WiimoteModule::~WiimoteModule() {}
void WiimoteModule::setEnabled(bool enabled) { Q_UNUSED(enabled) }
void WiimoteModule::disconnected(const QString &) {}
void WiimoteModule::connected(const QString &) {}
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
    KAction *connectAction;
    QAction *disconnectAction;
    bool isConnected = false;
    WiimoteManager *wiimoteManager;
    ToucheCore *toucheCore;
};

WiimoteModule::~WiimoteModule()
{
    delete d_ptr;
}

void WiimoteModule::profileChanged(int profileNumber)
{
  Q_D(WiimoteModule);
  qDebug() << "wiimote: profile changed: " << profileNumber;
  if(!d->isConnected)
    return;
  d->wiimoteManager->setLeds(std::min(0xF, profileNumber+1));
}


WiimoteModule::WiimoteModule(ToucheCore *toucheCore, KMenu *parentMenu, DevicesList *devicesList, KActionCollection *actionCollection, QObject *parent) :
    QObject(parent), d_ptr(new WiimoteModulePrivate(toucheCore, parentMenu))
{
    Q_D(WiimoteModule);
    d->titleAction = parentMenu->addTitle(i18n("Wiimote"));

    d->connectAction = new KAction(KIcon("network-connect", KIconLoader::global()), i18n("Connect Wiimote"), this);
    d->connectAction->setObjectName("Wiimote_Connect");
    d->connectAction->setGlobalShortcut(KShortcut("Meta+W"));
    parentMenu->addAction(d->connectAction);
    actionCollection->addAction(d->connectAction->objectName(), d->connectAction);
    connect(d->connectAction, SIGNAL(triggered()), this, SLOT(connectWiimote()));

    d->disconnectAction = new KAction(KIcon("network-disconnect", KIconLoader::global()), i18n("Disconnect Wiimote"), this);
    d->disconnectAction->setObjectName("Wiimote_Disconnect");
    parentMenu->addAction(d->disconnectAction);
    actionCollection->addAction(d->disconnectAction->objectName(), d->disconnectAction);
    connect(d->disconnectAction, SIGNAL(triggered()), this, SLOT(disconnectWiimote()));

    d->disconnectAction->setVisible(false);
    d->wiimoteManager = new WiimoteManager(this);
    connect(d->wiimoteManager, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
    connect(d->wiimoteManager, SIGNAL(disconnected(QString)), this, SLOT(disconnected(QString)));

    devicesList->add(new WiimoteDevice(d->wiimoteManager, toucheCore->keyboardDatabase(), this));
    QDBusInterface *upower = new QDBusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
    connect(upower, SIGNAL(Sleeping()), d->wiimoteManager, SLOT(disconnectWiimote()));
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



void WiimoteModule::disconnected(const QString &address)
{
    Q_D(WiimoteModule);
    d->enableActions(true);
    d->toggleActionsVisibility(false);
    d->isConnected = false;
    if(address == QString()) {
        QString title = i18n("%1: Wiimote Connection Failed").arg(i18n(Touche::displayName()));
        QString message = i18n("%1 could not find any Wiimote.\nPlease remember to press 1+2 to pair your Wiimote.").arg(i18n(Touche::displayName()));
        KNotification::event("deviceMessage", title, message);
    }
    qDebug() << "Wiimote disconnected: " << address;
}


void WiimoteModule::connected(const QString &address)
{
    Q_D(WiimoteModule);
    d->enableActions(true);
    d->toggleActionsVisibility(true);
    d->isConnected = true;
    qDebug() << "Wiimote connected: " << address;
}


void WiimoteModule::connectWiimote()
{
    Q_D(WiimoteModule);
    d->enableActions(false);
    QString title = i18n("%1: Connecting Wiimote").arg(i18n(Touche::displayName() ));
    QString message = i18n("Wiimote connection in progress.\nPress 1+2 on your Wiimote.");
    KNotification *notification = KNotification::event("deviceMessage", title, message, QPixmap(), 0L, KNotification::Persistent);
    connect(d->wiimoteManager, SIGNAL(disconnected(QString)), notification, SLOT(close()));

    d->wiimoteManager->connectWiimote();
}


void WiimoteModule::disconnectWiimote()
{
    Q_D(WiimoteModule);
    d->enableActions(false);
    d->wiimoteManager->disconnectWiimote();
}

#endif // HAVE_CWIID



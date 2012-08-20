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

#include <QDebug>
#include "touchesystemtray.h"
#include <QMenu>
#include <QCoreApplication>
#include "keysconfigurationdialog.h"
#include "domain/deviceinfo.h"
#include "touchecore.h"
#include "traymanager.h"

class ToucheSystemTrayPrivate {
public:
    ToucheSystemTrayPrivate(QMenu *systemTrayMenu, QAction *separator, TrayManager *trayManager, ToucheCore *toucheCore)
        : systemTrayMenu(systemTrayMenu), separator(separator), trayManager(trayManager), aboutToQuit(false), toucheCore(toucheCore) {}
    QMenu *systemTrayMenu;
    QAction *separator;
    QMap<DeviceInfo*, QAction*> actions;
    TrayManager *trayManager;
    bool aboutToQuit;
    ToucheCore *toucheCore;
};

ToucheSystemTray::ToucheSystemTray(ToucheCore *toucheCore, QMenu *systemTrayMenu, QAction *separator, TrayManager *trayManager) :
    QObject(toucheCore), d_ptr(new ToucheSystemTrayPrivate(systemTrayMenu, separator, trayManager, toucheCore))
{
    connect(toucheCore, SIGNAL(connected(DeviceInfo*)), SLOT(deviceConnected(DeviceInfo*)));
    connect(toucheCore, SIGNAL(disconnected(DeviceInfo*)), SLOT(deviceDisconnected(DeviceInfo*)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
    connect(qApp, SIGNAL(aboutToQuit()), toucheCore, SLOT(quit()));
    updateTooltip();
}

ToucheSystemTray::~ToucheSystemTray()
{
    delete d_ptr;
}

void ToucheSystemTray::showConfigurationDialog()
{
    Q_D(ToucheSystemTray);
    QAction *action = dynamic_cast<QAction*>(sender());
    DeviceInfo *deviceInfo = d->actions.key(action);
    KeysConfigurationDialog configDialog(deviceInfo);
    connect(d->toucheCore, SIGNAL(event(QString)), &configDialog, SLOT(keyEvent(QString)));
    d->toucheCore->suspendEventsTranslation();
    configDialog.exec();
    d->toucheCore->resumeEventsTranslation();
}

void ToucheSystemTray::deviceConnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    QString messageTitle = tr("%1: Device Connected!", "device connected tray popup").arg(qAppName());
    d->trayManager->showMessage(messageTitle, deviceInfo->name(), "input-keyboard");
    QAction *deviceAction = d->trayManager->createAction(deviceInfo->name(), d->systemTrayMenu);
    connect(deviceAction, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
    d->systemTrayMenu->insertAction(d->separator, deviceAction);
    d->actions.insert(deviceInfo, deviceAction);
    updateTooltip();
}

void ToucheSystemTray::deviceDisconnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    qDebug() << "about to quit: " << d->aboutToQuit;
    if(d->aboutToQuit)
        return;
    QString messageTitle = QString("<b>%1</b>: %2").arg(qAppName()).arg(tr("Device Disconnected!", "device disconnected tray popup"));
    d->trayManager->showMessage(messageTitle, deviceInfo->name(), "input-keyboard");
    QAction *action = d->actions.take(deviceInfo);
    d->systemTrayMenu->removeAction(action);
    delete action;
    updateTooltip();

}

void ToucheSystemTray::updateTooltip()
{
    Q_D(ToucheSystemTray);
    QStringList devices;
    foreach(DeviceInfo* deviceInfo, d->actions.keys()) {
        devices << deviceInfo->name();
    }
    d->trayManager->updateTooltip(devices.join("\n"));
}

void ToucheSystemTray::aboutToQuit()
{
    Q_D(ToucheSystemTray);
    d->aboutToQuit=true;
}



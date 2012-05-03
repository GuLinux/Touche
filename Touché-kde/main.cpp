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

#include <QApplication>

#include <kstatusnotifieritem.h>
#include <kmenu.h>
#include <QMenu>
#include <QMap>
#include "touchecore.h"
#include "domain/deviceinfo.h"

class TrayManager : public QObject {
    Q_OBJECT
public:
    TrayManager(KStatusNotifierItem *tray, KMenu *connectedDevices) : QObject(0), tray(tray), connectedDevices(connectedDevices) {}
private:
    KStatusNotifierItem *tray;
    KMenu *connectedDevices;
    QMap<DeviceInfo*, QAction*> actions;
public slots:
    void connected(DeviceInfo *deviceInfo) {
        tray->showMessage(qAppName().prepend("<b>") + "</b>: Device Connected!", deviceInfo->name(), "input-keyboard");
        QAction *action = connectedDevices->addAction(deviceInfo->name());
        actions.insert(deviceInfo, action);
        updateTooltip();

    }

    void disconnected(DeviceInfo *deviceInfo) {
        tray->showMessage(qAppName().prepend("<b>") + "</b>: Device Disconnected!", deviceInfo->name(), "input-keyboard");
        QAction *action = actions.take(deviceInfo);
        connectedDevices->removeAction(action);
        delete action;
        updateTooltip();
    }

    void updateTooltip() {
        QStringList devices;
        foreach(DeviceInfo* deviceInfo, actions.keys()) {
            devices << deviceInfo->name();
        }
        tray->setToolTipSubTitle(devices.join("\n"));
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList arguments = a.arguments();

    ToucheCore toucheCore(arguments);


    KStatusNotifierItem tray;
    tray.setIconByName("input-keyboard");

    KMenu trayMenu;
    KMenu connectedDevices;
    connectedDevices.setTitle("Connected devices");
    trayMenu.addMenu(&connectedDevices);
    trayMenu.addSeparator();
    tray.setContextMenu(&trayMenu);
    tray.setCategory(KStatusNotifierItem::Hardware);

    TrayManager trayManager(&tray, &connectedDevices);
    tray.setToolTipTitle(qAppName());
    tray.setTitle(qAppName());

    trayManager.connect(&toucheCore, SIGNAL(connected(DeviceInfo*)), SLOT(connected(DeviceInfo*)));
    trayManager.connect(&toucheCore, SIGNAL(disconnected(DeviceInfo*)), SLOT(disconnected(DeviceInfo*)));

    toucheCore.start();
    return a.exec();
}

#include "main.moc"

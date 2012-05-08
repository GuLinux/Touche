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

#include <kuniqueapplication.h>

#include <kstatusnotifieritem.h>
#include <kmenu.h>
#include <QMenu>
#include <QMap>
#include "touchecore.h"
#include "domain/deviceinfo.h"
#include <kaction.h>
#include "toucheconfiguration.h"
#include <QDebug>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <QTimer>
#include <QDialog>

class TrayManager : public QObject {
    Q_OBJECT
public:
    TrayManager(KStatusNotifierItem *tray, KMenu *connectedDevices, QAction *beforeAction, QObject *parent=0) : QObject(parent), tray(tray), connectedDevices(connectedDevices), beforeAction(beforeAction) {
    }

private:
    KStatusNotifierItem *tray;
    KMenu *connectedDevices;
    QAction *beforeAction;
    QMap<DeviceInfo*, KAction*> actions;
    ToucheConfiguration toucheConfiguration;

public slots:
    void connected(DeviceInfo *deviceInfo) {
        tray->showMessage(qAppName().prepend("<b>") + "</b>: Device Connected!", deviceInfo->name(), "input-keyboard");
        KAction *action = new KAction(deviceInfo->name(), connectedDevices);
        connect(action, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
        connectedDevices->insertAction(beforeAction, action);
        actions.insert(deviceInfo, action);
        updateTooltip();
    }

    void showConfigurationDialog() {
        KAction *action = dynamic_cast<KAction*>(sender());
        DeviceInfo *deviceInfo = actions.key(action);
        toucheConfiguration.showConfigurationDialog(deviceInfo);
    }

    void disconnected(DeviceInfo *deviceInfo) {
        tray->showMessage(qAppName().prepend("<b>") + "</b>: Device Disconnected!", deviceInfo->name(), "input-keyboard");
        KAction *action = actions.take(deviceInfo);
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
    KAboutData about("touche", 0, ki18n("Touché"), "0.1");
    KCmdLineArgs::init(argc, argv, &about);
//    KCmdLineArgs::addCmdLineOptions( myCmdOptions );
    //KUniqueApplication::addCmdLineOptions();

//    qDebug() << "Calling start";
//    if (!KUniqueApplication::start()) {
//       qDebug() << "myAppName is already running!";
//       return 0;
//    }
    KApplication a;
//    KUniqueApplication a;
//    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QStringList arguments = a.arguments();

    ToucheCore toucheCore(arguments);

    KStatusNotifierItem *tray = new KStatusNotifierItem(&toucheCore);
    tray->setIconByName("input-keyboard");

    // not a great approach, but having it autodelete on exit seems to make the app crash.
    // it is however worth pointing out that memory is cleared on application exit, so it's not a real memory leak.
    KMenu *trayMenu = new KMenu(0);

    trayMenu->addTitle(QIcon::fromTheme("input-keyboard"), qAppName());
    tray->setContextMenu(trayMenu);
    tray->setCategory(KStatusNotifierItem::Hardware);

    TrayManager *trayManager = new TrayManager(tray, trayMenu, trayMenu->addSeparator(), &toucheCore);
    tray->setToolTipTitle(qAppName());
    tray->setTitle(qAppName());

    trayManager->connect(&toucheCore, SIGNAL(connected(DeviceInfo*)), SLOT(connected(DeviceInfo*)));
    trayManager->connect(&toucheCore, SIGNAL(disconnected(DeviceInfo*)), SLOT(disconnected(DeviceInfo*)));

    a.connect(&a, SIGNAL(aboutToQuit()), &toucheCore, SLOT(quit()));
    toucheCore.start();
    return a.exec();
}

#include "main.moc"

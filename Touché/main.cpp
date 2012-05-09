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

#include <QSystemTrayIcon>
#include <QMenu>
#include <QMap>
#include <QTextStream>
#include "touchecore.h"
#include "domain/deviceinfo.h"
#include "toucheconfiguration.h"
#include "trayIcon/touchesystemtray.h"
#include "trayIcon/traymanager.h"


class QtTrayManager : public TrayManager {
public:
    QtTrayManager(QSystemTrayIcon *tray) : tray(tray) {}
    virtual QAction *createAction(const QString &text, QObject *parent=0) {
        return new QAction(text, parent);
    }

    virtual void showMessage(const QString &title, const QString &text, const QString &iconTheme = QString()) {
        Q_UNUSED(iconTheme);
        tray->showMessage(title, text);
    }

    void updateTooltip(const QString &tooltip) {
        Q_UNUSED(tooltip);
        tray->setToolTip(qAppName());
    }

private:
    QSystemTrayIcon *tray;
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(false);
    QStringList arguments = a.arguments();


    if(arguments.contains("--help") || arguments.contains("-h")) {
        QTextStream out(stdout);
        out<< qAppName() << " supported options: \n";
        foreach(QString option, ToucheCore::supportedOptions().keys()) {
            out << "\t--" << a.translate("command line option", option.toLatin1()) << "\t\t" << a.translate("command line option description", ToucheCore::supportedOptions().value(option).toLatin1()) << endl;
        }
        out << endl;
        return 0;
    }


    ToucheCore toucheCore(arguments);

    QSystemTrayIcon tray(QIcon::fromTheme("input-keyboard"));

    QMenu trayMenu;
    QMenu connectedDevices;
    connectedDevices.setTitle(connectedDevices.tr("tray.menu.ConnectedDevices"));
    trayMenu.addMenu(&connectedDevices);
    trayMenu.addSeparator();
    trayMenu.addAction(QIcon::fromTheme("application-exit"), "Quit", qApp, SLOT(quit()));
    tray.setContextMenu(&trayMenu);

    QtTrayManager trayManager(&tray);
    tray.show();
    //tray.setToolTip(qAppName());

    new ToucheSystemTray(&toucheCore, &connectedDevices, 0, &trayManager);
    toucheCore.start();
    return a.exec();
}


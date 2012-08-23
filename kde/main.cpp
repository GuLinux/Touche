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
#include <QDebug>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <QtCore/QTimer>
#include <QDialog>
#include <KAboutApplicationDialog>
#include "trayIcon/touchesystemtray.h"
#include "trayIcon/traymanager.h"

class KDETrayManager : public TrayManager {
public:
    KDETrayManager(KStatusNotifierItem *tray) : tray(tray) {}
    virtual QAction *createAction(const QString &text, QObject *parent=0) {
        return new KAction(text, parent);
    }

    virtual void showMessage(const QString &title, const QString &text, const QString &iconTheme = QString()) {
        tray->showMessage(title, text, iconTheme);
    }

    void updateTooltip(const QString &tooltip) {
        tray->setToolTip(QIcon::fromTheme("input-keyboard"), qAppName(), QString());
        tray->setToolTipSubTitle(tooltip);
    }

private:
    KStatusNotifierItem *tray;
};


int main(int argc, char *argv[])
{
    KAboutData about("touche", 0, ki18n("Touché"), "0.2",
                     ki18n("Special key recognizer for Linux Desktops"),
                     KAboutData::License_GPL_V3,
                     KLocalizedString(),
                     KLocalizedString(),
                     "http://rockman81.blogspot.it/p/touche.html",
                     "marco.gulino@kde.org"
                     );
    about.addAuthor(ki18n("Marco Gulino"), ki18n("main developer"),
                    "marco.gulino@gmail.com", "http://rockman81.blogspot.it/");
    about.setProgramIconName("input-keyboard");
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineOptions options;
    foreach(QString option, ToucheCore::supportedOptions().keys()) {
        options.add(option.toLatin1(), ki18n(ToucheCore::supportedOptions().value(option).toLatin1()));
    }

    KCmdLineArgs::addCmdLineOptions(options);

#ifdef QT_NO_DEBUG
#warning "Release mode: enabling KUniqueApplication"
    KUniqueApplication::addCmdLineOptions();
    KUniqueApplication a;
#else
#warning "Debug mode: disabling KUniqueApplication"
    KApplication a;
#endif

    KAboutApplicationDialog aboutTouche(&about);
    a.setQuitOnLastWindowClosed(false);
    QStringList arguments = KCmdLineArgs::allArguments();

    ToucheCore toucheCore(arguments);

    KStatusNotifierItem *tray = new KStatusNotifierItem(&toucheCore);
    tray->setIconByName("input-keyboard");

    // not a great approach, but having it autodelete on exit seems to make the app crash.
    // it is however worth pointing out that memory is cleared on application exit, so it's not a real memory leak.
    KMenu *trayMenu = new KMenu(0);
    KMenu *profilesMenu = new KMenu(0);
    trayMenu->addTitle(QIcon::fromTheme("input-keyboard"), qAppName());
    trayMenu->addAction(ki18n("About Touché").toString(), &aboutTouche, SLOT(exec()));
    tray->setContextMenu(trayMenu);
    trayMenu->addMenu(profilesMenu);
    tray->setCategory(KStatusNotifierItem::Hardware);
    tray->setTitle(qAppName());
    KDETrayManager trayManager(tray);

    new ToucheSystemTray(&toucheCore, trayMenu, profilesMenu, trayMenu->addSeparator(), &trayManager);

    toucheCore.start();
    return a.exec();
}


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
#include <k4aboutdata.h>
#include <kcmdlineargs.h>
#include <QTimer>
#include <QDialog>
#include <KAboutApplicationDialog>
#include "touchesystemtray.h"
#include "domain/DevicesList.h"
#include "backend/hiddev/HiddevDevices.h"
#include "backend/config/keyboarddatabase.h"

int main(int argc, char *argv[])
{
    K4AboutData about(Touche::appName().toUtf8(), 0, ki18n(Touche::displayName().toUtf8().constData()),
                     Touche::appVersion(),
                     ki18n("Special key recognizer for Linux Desktops"),
                     K4AboutData::License_GPL_V3,
                     KLocalizedString(),
                     KLocalizedString(),
                     "https://github.com/GuLinux/Touche",
                     "marco.gulino@kde.org"
                     );
    about.addAuthor(ki18n("Marco Gulino"), ki18n("main developer"),
                    "marco.gulino@gmail.com", "http://rockman81.blogspot.it/");
    about.setProgramIconName(Touche::iconName() );
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

    KAboutApplicationDialog aboutTouche(about);
    a.setQuitOnLastWindowClosed(false);
    QStringList arguments = KCmdLineArgs::allArguments();
    KeyboardDatabase database(Touche::keyboardDatabases());
    DevicesList devicesList;
    devicesList.add(new HiddevDevices(&database));
    ToucheCore toucheCore(&devicesList, &database, arguments);
    new ToucheSystemTray(&toucheCore, &aboutTouche, &devicesList);

    toucheCore.start();
    return a.exec();
}


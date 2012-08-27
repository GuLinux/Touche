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
#include <QMessageBox>
#include <KLocale>
#include <KStatusNotifierItem>
#include "EditProfilesDialog.h"
#include <KAction>
#include <KMenu>
#include <KAboutApplicationDialog>

class ToucheSystemTrayPrivate {
public:
    ToucheSystemTrayPrivate(ToucheCore *toucheCore)
        : aboutToQuit(false), toucheCore(toucheCore) {}
    KMenu *systemTrayMenu;
    QMap<DeviceInfo*, KAction*> actions;
    bool aboutToQuit;
    ToucheCore *toucheCore;
    KStatusNotifierItem *tray;
    QAction *afterProfiles;
    QAction *afterDevices;
};

ToucheSystemTray::ToucheSystemTray(ToucheCore *toucheCore, KAboutApplicationDialog *aboutDialog) :
    QObject(toucheCore), d_ptr(new ToucheSystemTrayPrivate(toucheCore))
{
    Q_D(ToucheSystemTray);
    d->tray = new KStatusNotifierItem(toucheCore);
    d->tray->setIconByName(Touche::iconName());

    // not a great approach, but having it autodelete on exit seems to make the app crash.
    // it is however worth pointing out that memory is cleared on application exit, so it's not a real memory leak.
    d->systemTrayMenu = new KMenu(0);
    d->systemTrayMenu->addTitle(QIcon::fromTheme(Touche::iconName()), i18n(Touche::displayName()));
    d->systemTrayMenu->addAction(ki18n("About Touché").toString(), aboutDialog, SLOT(exec()));
    d->tray->setContextMenu(d->systemTrayMenu);
    d->tray->setCategory(KStatusNotifierItem::Hardware);
    d->tray->setTitle(i18n(Touche::displayName() ));


    connect(d->toucheCore, SIGNAL(connected(DeviceInfo*)), SLOT(deviceConnected(DeviceInfo*)));
    connect(d->toucheCore, SIGNAL(disconnected(DeviceInfo*)), SLOT(deviceDisconnected(DeviceInfo*)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
    connect(qApp, SIGNAL(aboutToQuit()), toucheCore, SLOT(quit()));
    connect(d->toucheCore, SIGNAL(profileChanged(QString)), this, SLOT(updateProfilesList()));
    connect(d->toucheCore, SIGNAL(profileChanged(QString)), this, SLOT(profileChanged(QString)));
    d->systemTrayMenu->addTitle(i18n("Profiles"));
    d->systemTrayMenu->addAction(i18n("Edit Profiles..."), this, SLOT(editProfiles()));
    d->afterProfiles = d->systemTrayMenu->addSeparator();
    d->systemTrayMenu->addTitle(i18n("Devices"));
    d->afterDevices = d->systemTrayMenu->addSeparator();
    updateTooltip();
    updateProfilesList();
}

ToucheSystemTray::~ToucheSystemTray()
{
    delete d_ptr;
}

void ToucheSystemTray::showConfigurationDialog()
{
    Q_D(ToucheSystemTray);
    if(d->toucheCore->currentProfile().isEmpty() || ! d->toucheCore->availableProfiles().contains(d->toucheCore->currentProfile() )) {
        QMessageBox::warning(0, i18n("Profile missing"), "Error! You have to add and select a profile first.");
        return;
    }
    KAction *action = dynamic_cast<KAction*>(sender());
    DeviceInfo *deviceInfo = d->actions.key(action);
    KeysConfigurationDialog *configDialog = new KeysConfigurationDialog(deviceInfo, d->toucheCore->currentProfile());
    connect(d->toucheCore, SIGNAL(disconnected(DeviceInfo*)), configDialog, SLOT(reject()));
    connect(d->toucheCore, SIGNAL(inputEvent(QString)), configDialog, SLOT(keyEvent(QString)));
    d->toucheCore->suspendEventsTranslation();
    configDialog->exec();
    d->toucheCore->resumeEventsTranslation();
    delete configDialog;
}

void ToucheSystemTray::deviceConnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    QString messageTitle = QString("<b>%1</b>: %2")
            .arg(i18n(Touche::displayName() ))
            .arg(i18nc("device connected tray popup", "Device Connected!"));
    d->tray->showMessage(messageTitle, deviceInfo->name(), Touche::iconName() );
    KAction *deviceAction = new KAction(deviceInfo->name(), d->systemTrayMenu);
    connect(deviceAction, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
    d->systemTrayMenu->insertAction(d->afterDevices, deviceAction);
    d->actions.insert(deviceInfo, deviceAction);
    updateTooltip();
}

void ToucheSystemTray::deviceDisconnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    qDebug() << "about to quit: " << d->aboutToQuit;
    if(d->aboutToQuit)
        return;
    QString messageTitle = QString("<b>%1</b>: %2")
            .arg(i18n(Touche::displayName() ))
            .arg(i18nc("device disconnected tray popup", "Device Disconnected!"));
    d->tray->showMessage(messageTitle, deviceInfo->name(), Touche::iconName() );
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

    d->tray->setToolTip(QIcon::fromTheme(Touche::iconName() ), i18n(Touche::displayName() ), QString());
    d->tray->setToolTipSubTitle(devices.join("\n"));

}

void ToucheSystemTray::aboutToQuit()
{
    Q_D(ToucheSystemTray);
    d->aboutToQuit=true;
}




void ToucheSystemTray::updateProfilesList()
{
    Q_D(ToucheSystemTray);
//    d->profilesMenu->clear();
//    d->profilesMenu->addAction(i18n("Edit Profiles..."), this, SLOT(editProfiles()));
//    QAction *nextProfile = d->profilesMenu->addAction(i18n("Next Profile"),
//        this, SLOT(switchToNextProfile()));
//    nextProfile->setShortcutContext(Qt::ApplicationShortcut);
//    d->profilesMenu->addSeparator();
    foreach(QAction *action, d->systemTrayMenu->actions()) {
        if(action->objectName().startsWith("profile_"))
            d->systemTrayMenu->removeAction(action);
    }

    foreach(QString profile, d->toucheCore->availableProfiles()) {
        QAction *profileAction = new KAction(profile, this);
        d->systemTrayMenu->insertAction(d->afterProfiles, profileAction);
        profileAction->setObjectName(QString("profile_%1").arg(profile));
        profileAction->setProperty("profile", profile);
        profileAction->setCheckable(true);
        profileAction->setChecked(profile == d->toucheCore->currentProfile());
        connect(profileAction, SIGNAL(triggered()), this, SLOT(setProfile()));
    }
}


void ToucheSystemTray::setProfile()
{
    Q_D(ToucheSystemTray);
    QAction *profileAction = (QAction*) sender();
    d->toucheCore->setProfile(profileAction->property("profile").toString());
}


void ToucheSystemTray::editProfiles()
{
    Q_D(ToucheSystemTray);
    EditProfilesDialog editProfilesDialog(d->toucheCore);
    editProfilesDialog.exec();
    updateProfilesList();
}


void ToucheSystemTray::switchToNextProfile()
{
    Q_D(ToucheSystemTray);
    QStringList profiles = d->toucheCore->availableProfiles();
    QList<QString>::const_iterator i;
    QString newProfile;
    for (i = profiles.constBegin(); i != profiles.constEnd(); ++i) {
        if(*i == d->toucheCore->currentProfile()) {
            i++;
            if(i==profiles.constEnd())
                newProfile = profiles.first();
            else newProfile = *i;
            break;
        }
    }
    d->toucheCore->setProfile(newProfile);
}


void ToucheSystemTray::profileChanged(const QString &profile)
{
    Q_D(ToucheSystemTray);
    d->tray->showMessage(i18n("%1 Profile").arg(i18n(Touche::displayName() )),
                         i18n("Profile changed to %1").arg(profile),
                         Touche::iconName() );
}

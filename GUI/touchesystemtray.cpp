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

#include "touchesystemtray.h"
#include "touchecore.h"
#include "domain/deviceinfo.h"
#include "keysconfigurationdialog.h"
#include "SettingsDialog.h"
#include <QDebug>
#include <QMenu>
#include <QIcon>
#include <QCoreApplication>
#include <QMessageBox>
#include <KLocale>
#include <KStatusNotifierItem>
#include <KAction>
#include <KMenu>
#include <KAboutApplicationDialog>
#include <KStandardAction>
#include <KShortcutsDialog>
#include <KActionCollection>
#include <QSettings>
#include "kshortcut.h"
#include "modules/wiimote/WiimoteModule.h"
#include <KNotification>


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
    WiimoteModule *wiimoteModule;
    KAction *switchToNextProfile;
};

ToucheSystemTray::ToucheSystemTray(ToucheCore *toucheCore, KAboutApplicationDialog *aboutDialog, DevicesList *devicesList) :
    QObject(toucheCore), d_ptr(new ToucheSystemTrayPrivate(toucheCore))
{
    Q_D(ToucheSystemTray);
    d->tray = new KStatusNotifierItem(toucheCore);
    d->tray->setIconByName(Touche::iconName());
    connect(d->tray, SIGNAL(activateRequested(bool,QPoint)), this, SLOT(trayActivateRequest(bool,QPoint)));

    // not a great approach, but having it autodelete on exit seems to make the app crash.
    // it is however worth pointing out that memory is cleared on application exit, so it's not a real memory leak.
    d->systemTrayMenu = new KMenu(0);
    d->systemTrayMenu->addTitle(QIcon::fromTheme(Touche::iconName()), i18n(Touche::displayName().toUtf8().constData()));
    d->systemTrayMenu->addAction(KStandardAction::preferences(this, SLOT(editProfiles()), this) );
    d->systemTrayMenu->addAction(KStandardAction::keyBindings(this, SLOT(configureShortcuts()), this));
    d->systemTrayMenu->addAction(KStandardAction::aboutApp(aboutDialog, SLOT(exec()), this));
    d->tray->setContextMenu(d->systemTrayMenu);
    d->tray->setCategory(KStatusNotifierItem::Hardware);
    d->tray->setTitle(i18n(Touche::displayName() .toUtf8().constData()));


    connect(d->toucheCore, SIGNAL(connected(DeviceInfo*)), SLOT(deviceConnected(DeviceInfo*)));
    connect(d->toucheCore, SIGNAL(disconnected(DeviceInfo*)), SLOT(deviceDisconnected(DeviceInfo*)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
    connect(qApp, SIGNAL(aboutToQuit()), toucheCore, SLOT(quit()));
    connect(d->toucheCore, SIGNAL(profileChanged(QString)), this, SLOT(updateProfilesList()));
    connect(d->toucheCore, SIGNAL(profileChanged(QString)), this, SLOT(profileChanged(QString)));
    d->systemTrayMenu->addTitle(i18n("Profiles"));
    d->switchToNextProfile = new KAction(i18n("Next Profile"), d->systemTrayMenu);
    d->switchToNextProfile->setIcon(QIcon::fromTheme("tab-duplicate"));
    d->switchToNextProfile->setObjectName("SwitchToNextProfile");
    d->switchToNextProfile->setGlobalShortcut(KShortcut("Meta+P"));
    d->tray->addAction(d->switchToNextProfile->objectName(), d->switchToNextProfile);

    connect(d->switchToNextProfile, SIGNAL(triggered()), this, SLOT(switchToNextProfile()));
    d->systemTrayMenu->addAction(d->switchToNextProfile);
    d->systemTrayMenu->addSeparator();
    d->afterProfiles = d->systemTrayMenu->addSeparator();
    d->systemTrayMenu->addTitle(i18n("Configure Bindings"));

    d->afterDevices = d->systemTrayMenu->addSeparator();

    auto actionCollection = new KActionCollection(d->tray);
    actionCollection->addActions(d->tray->actionCollection());

#ifdef CWIID_FOUND
    d->wiimoteModule = new WiimoteModule(d->toucheCore, d->systemTrayMenu,
                                         devicesList, actionCollection, this);
#endif

    updateTooltip();
    updateProfilesList();
    updateModulesList();
}

ToucheSystemTray::~ToucheSystemTray()
{
    delete d_ptr;
}

void ToucheSystemTray::showConfigurationDialog()
{
    Q_D(ToucheSystemTray);
    if(d->toucheCore->currentProfile().isEmpty() || ! d->toucheCore->availableProfiles().contains(d->toucheCore->currentProfile() )) {
        QMessageBox::warning(0, i18n("Profile missing"), i18n("Error! You have to add and select a profile first."));
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
            .arg(i18n(Touche::displayName() .toUtf8().constData()))
            .arg(i18nc("device connected tray popup", "Device Connected!"));
    KNotification::event("deviceConnected", messageTitle, deviceInfo->name());
    KAction *deviceAction = new KAction(deviceInfo->name(), d->systemTrayMenu);
    connect(deviceAction, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
    d->systemTrayMenu->insertAction(d->afterDevices, deviceAction);
    d->actions.insert(deviceInfo, deviceAction);
    updateTooltip();
#ifdef CWIID_FOUND
    d->wiimoteModule->profileChanged(d->toucheCore->availableProfiles().indexOf(d->toucheCore->currentProfile()));
#endif
}

void ToucheSystemTray::deviceDisconnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    if(d->aboutToQuit || !deviceInfo) return;
    QString messageTitle = QString("<b>%1</b>: %2")
            .arg(i18n(Touche::displayName() .toUtf8().constData()))
            .arg(i18nc("device disconnected tray popup", "Device Disconnected!"));
    KNotification::event("deviceConnected", messageTitle, deviceInfo->name());
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

    d->tray->setToolTip(QIcon::fromTheme(Touche::iconName() ), i18n(Touche::displayName().toUtf8().constData()), QString());
    d->tray->setToolTipSubTitle(devices.join("\n"));

}

void ToucheSystemTray::aboutToQuit()
{
    Q_D(ToucheSystemTray);
    d->aboutToQuit=true;
}


void ToucheSystemTray::updateModulesList()
{
    Q_D(ToucheSystemTray);
    QSettings *toucheSettings = Touche::settings(this);
    toucheSettings->sync();
#ifdef CWIID_FOUND
    bool wiimoteEnabled = toucheSettings->value("wiimote_enabled", false).toBool();
    d->wiimoteModule->setEnabled(wiimoteEnabled);
#endif
    delete toucheSettings;
}

void ToucheSystemTray::updateProfilesList()
{
    Q_D(ToucheSystemTray);
    foreach(QAction *action, d->systemTrayMenu->actions()) {
        if(action->objectName().startsWith("profile_"))
            d->systemTrayMenu->removeAction(action);
    }
    QStringList availProfiles = d->toucheCore->availableProfiles();
    d->switchToNextProfile->setEnabled(availProfiles.size()>0);

    foreach(QString profile, availProfiles) {
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
    SettingsDialog settings(d->toucheCore);
    settings.exec();
    updateProfilesList();
    updateModulesList();
}


void ToucheSystemTray::switchToNextProfile()
{
    Q_D(ToucheSystemTray);
    QStringList profiles = d->toucheCore->availableProfiles();
    if(profiles.isEmpty()) return;
    if(!profiles.contains(d->toucheCore->currentProfile())) {
        d->toucheCore->setProfile(profiles.first());
        return;
    }
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
#ifdef CWIID_FOUND
    d->wiimoteModule->profileChanged(d->toucheCore->availableProfiles().indexOf(profile));
#endif
    KNotification *notification = KNotification::event("profileChanged", i18n("%1 Profile").arg(i18n(Touche::displayName() .toUtf8().constData())),
                         i18n("Profile changed to %1").arg(profile));
    connect(d->toucheCore, SIGNAL(profileChanged(QString)), notification, SLOT(close()));

}


void ToucheSystemTray::configureShortcuts()
{
    Q_D(ToucheSystemTray);
    auto actionCollection = new KActionCollection(d->tray);
    actionCollection->addActions(d->tray->actionCollection());
    KShortcutsDialog::configure(actionCollection);
}



void ToucheSystemTray::trayActivateRequest(bool active, const QPoint &pos)
{
    Q_D(ToucheSystemTray);
    d->systemTrayMenu->popup(pos);
}

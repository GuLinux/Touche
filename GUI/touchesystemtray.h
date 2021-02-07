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

#ifndef TOUCHESYSTEMTRAY_H
#define TOUCHESYSTEMTRAY_H

#include <QObject>
#include <QPoint>

class QMenu;
class DeviceInfo;
class ToucheSystemTrayPrivate;
class TrayManager;
class ToucheCore;
class QAction;
class QSettings;
class KEditListBox;
class KAboutApplicationDialog;
class DevicesList;
class KNotification;

class ToucheSystemTray : public QObject
{
    Q_OBJECT
public:
    explicit ToucheSystemTray(ToucheCore *toucheCore, KAboutApplicationDialog *aboutDialog, DevicesList *devicesList);
    ~ToucheSystemTray();
signals:
    
public slots:
    void updateTooltip();
    void deviceConnected(DeviceInfo *deviceInfo);
    void deviceDisconnected(DeviceInfo *deviceInfo);
    void showConfigurationDialog();
    void aboutToQuit();
    void updateProfilesList();
    void setProfile();
    void editProfiles();
    void switchToNextProfile();
    void profileChanged(const QString &profile);
    void configureShortcuts();
    void updateModulesList();
    void trayActivateRequest(bool active, const QPoint &pos);

private:
    ToucheSystemTrayPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ToucheSystemTray)
    
};

#endif // TOUCHESYSTEMTRAY_H

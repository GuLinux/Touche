#include "touchesystemtray.h"
#include <QMenu>
#include <QCoreApplication>
#include "toucheconfiguration.h"
#include "domain/deviceinfo.h"
#include "touchecore.h"
#include "traymanager.h"

class ToucheSystemTrayPrivate {
public:
    ToucheSystemTrayPrivate(QMenu *systemTrayMenu, QAction *separator, TrayManager *trayManager)
        : systemTrayMenu(systemTrayMenu), separator(separator), trayManager(trayManager), aboutToQuit(false) {}
    QMenu *systemTrayMenu;
    QAction *separator;
    ToucheConfiguration *toucheConfiguration;
    QMap<DeviceInfo*, QAction*> actions;
    TrayManager *trayManager;
    bool aboutToQuit;
};

ToucheSystemTray::ToucheSystemTray(ToucheCore *toucheCore, QMenu *systemTrayMenu, QAction *separator, TrayManager *trayManager) :
    QObject(toucheCore), d_ptr(new ToucheSystemTrayPrivate(systemTrayMenu, separator, trayManager))
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
    d->toucheConfiguration->showConfigurationDialog(deviceInfo);
}

void ToucheSystemTray::deviceConnected(DeviceInfo *deviceInfo)
{
    Q_D(ToucheSystemTray);
    QString messageTitle = tr("%1: Device connected!", "device connected tray popup").arg(qAppName());
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



#include <QApplication>
#include "domain/inputevent.h"
#include "domain/dumpkeys.h"
#include "domain/translatekeyevents.h"
#include "backend/hiddev/finddevices.h"
#include "domain/deviceinfo.h"
#include "backend/config/keyboarddatabase.h"
#include "backend/config/bindingsconfig.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMap>

class TrayManager : public QObject {
    Q_OBJECT
public:
    TrayManager(QSystemTrayIcon *tray, QMenu *connectedDevices) : QObject(0), tray(tray), connectedDevices(connectedDevices) {}
private:
    QSystemTrayIcon *tray;
    QMenu *connectedDevices;
    QMap<DeviceInfo*, QAction*> actions;
public slots:
    void connected(DeviceInfo *deviceInfo) {
        tray->showMessage("Device Connected!", deviceInfo->name());
        QAction *action = connectedDevices->addAction(deviceInfo->name());
        actions.insert(deviceInfo, action);
    }

    void disconnected(DeviceInfo *deviceInfo) {
        QAction *action = actions.take(deviceInfo);
        connectedDevices->removeAction(action);
        delete action;
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DumpKeys dumpKeys;
    KeyboardDatabase db(QStringList("/usr/share/hiddev_kbd_helper/keyboard_database.json"));
    BindingsConfig bindingsConfig;
    TranslateKeyEvents translateEvents(&db, &bindingsConfig);
    //hiddev.start();
    FindDevices findDevices;
    QObject::connect(&findDevices, SIGNAL(connected(DeviceInfo*)), &db, SLOT(deviceAdded(DeviceInfo*)));
    QObject::connect(&findDevices, SIGNAL(disconnected(DeviceInfo*)), &db, SLOT(deviceRemoved(DeviceInfo*)));
    QObject::connect(&findDevices, SIGNAL(event(InputEvent*,DeviceInfo*)), &dumpKeys, SLOT(event(InputEvent*)));
    QObject::connect(&findDevices, SIGNAL(event(InputEvent*,DeviceInfo*)), &translateEvents, SLOT(event(InputEvent*, DeviceInfo*)));
    QSystemTrayIcon tray(QIcon::fromTheme("input-keyboard"));

    QMenu trayMenu;
    QMenu connectedDevices;
    connectedDevices.setTitle("Connected devices");
    trayMenu.addMenu(&connectedDevices);
    trayMenu.addSeparator();
    trayMenu.addAction(QIcon::fromTheme("application-exit"), "Quit", qApp, SLOT(quit()));
    tray.setContextMenu(&trayMenu);

    TrayManager trayManager(&tray, &connectedDevices);
    tray.show();
    tray.setToolTip(qAppName());

    trayManager.connect(&findDevices, SIGNAL(connected(DeviceInfo*)), SLOT(connected(DeviceInfo*)));
    trayManager.connect(&findDevices, SIGNAL(disconnected(DeviceInfo*)), SLOT(disconnected(DeviceInfo*)));
    return a.exec();
}

#include "main.moc"

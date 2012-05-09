#ifndef TOUCHESYSTEMTRAY_H
#define TOUCHESYSTEMTRAY_H

#include <QObject>

class QMenu;
class DeviceInfo;
class ToucheSystemTrayPrivate;
class TrayManager;
class ToucheCore;
class QAction;
class ToucheSystemTray : public QObject
{
    Q_OBJECT
public:
    explicit ToucheSystemTray(ToucheCore *toucheCore, QMenu *systemTrayMenu, QAction *separator, TrayManager *trayManager);
    ~ToucheSystemTray();
signals:
    
public slots:
    void updateTooltip();
    void deviceConnected(DeviceInfo *deviceInfo);
    void deviceDisconnected(DeviceInfo *deviceInfo);
    void showConfigurationDialog();
    void aboutToQuit();

private:
    ToucheSystemTrayPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ToucheSystemTray)
    
};

#endif // TOUCHESYSTEMTRAY_H

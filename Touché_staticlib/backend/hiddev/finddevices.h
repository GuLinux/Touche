#ifndef FINDDEVICES_H
#define FINDDEVICES_H

#include <QObject>

class FindDevicesPrivate;
class DeviceInfo;
class InputEvent;
class FindDevices : public QObject
{
    Q_OBJECT
public:
    explicit FindDevices(QObject *parent = 0);
    ~FindDevices();
signals:
    void event(InputEvent *keyEvent, DeviceInfo *deviceInfo);
    void connected(DeviceInfo *deviceInfo);
    void disconnected(DeviceInfo *deviceInfo);

public slots:
    void deviceRemoved(DeviceInfo *deviceInfo);
    void deviceChanged();
private:
    FindDevicesPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(FindDevices)
};

#endif // FINDDEVICES_H

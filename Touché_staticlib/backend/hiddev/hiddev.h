#ifndef HIDDEV_H
#define HIDDEV_H

#include <QObject>

class DeviceInfo;
class HidDevPrivate;
class InputEvent;

class HidDev : public QObject
{
    Q_OBJECT
public:
    explicit HidDev(const QString &path, QObject *parent = 0);
    ~HidDev();
    
signals:
    void event(InputEvent *keyEvent, DeviceInfo *deviceInfo);
    void removed(DeviceInfo *deviceInfo);
    void connected(DeviceInfo * deviceInfo);
public slots:
    void start();

private:
    HidDevPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(HidDev)
};

#endif // HIDDEV_H

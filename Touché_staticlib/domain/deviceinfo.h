#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>

class DeviceInfoPrivate;
class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = 0);
    ~DeviceInfo();
    DeviceInfo * name(const QString &name);
    DeviceInfo * path(const QString &path);
    DeviceInfo * vendor(quint32 vendor);
    DeviceInfo * productID(quint32 productID);
    DeviceInfo * version(quint32 version);
    DeviceInfo * bus(quint32 bus);
    DeviceInfo * deviceNumber(quint32 deviceNumber);
    DeviceInfo * interfaceNumber(quint32 interfaceNumber);

    QString name();
    QString path();
    QString configID();
    quint32 vendor();
    quint32 productID();
    quint32 version();
    quint32 bus();
    quint32 deviceNumber();
    quint32 interfaceNumber();
signals:
    
public slots:
private:
    DeviceInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(DeviceInfo)
};

#endif // DEVICEINFO_H

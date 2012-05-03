#include "deviceinfo.h"

class DeviceInfoPrivate {
public:
    QString name, path;
    quint32 vendor, productID, version;
    quint32 bus, deviceNumber, interfaceNumber;
};

DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent), d_ptr(new DeviceInfoPrivate())
{
}

DeviceInfo::~DeviceInfo()
{
    delete d_ptr;
}

DeviceInfo *DeviceInfo::name(const QString &name)
{
    Q_D(DeviceInfo);
    d->name=name;
    return this;
}

DeviceInfo *DeviceInfo::path(const QString &path)
{
    Q_D(DeviceInfo);
    d->path=path;
    return this;
}

DeviceInfo *DeviceInfo::vendor(quint32 vendor)
{
    Q_D(DeviceInfo);
    d->vendor=vendor;
    return this;
}

DeviceInfo *DeviceInfo::productID(quint32 productID)
{
    Q_D(DeviceInfo);
    d->productID=productID;
    return this;
}

DeviceInfo *DeviceInfo::version(quint32 version)
{
    Q_D(DeviceInfo);
    d->version=version;
    return this;
}

DeviceInfo *DeviceInfo::bus(quint32 bus)
{
    Q_D(DeviceInfo);
    d->bus=bus;
    return this;
}

DeviceInfo *DeviceInfo::deviceNumber(quint32 deviceNumber)
{
    Q_D(DeviceInfo);
    d->deviceNumber=deviceNumber;
    return this;
}

DeviceInfo *DeviceInfo::interfaceNumber(quint32 interfaceNumber)
{
    Q_D(DeviceInfo);
    d->interfaceNumber=interfaceNumber;
    return this;
}

QString DeviceInfo::name()
{
    Q_D(DeviceInfo);
    return d->name;
}

QString DeviceInfo::path()
{
    Q_D(DeviceInfo);
    return d->path;
}

QString DeviceInfo::configID()
{
    return QString("0x%1:0x%2").arg(vendor(), 0, 16).arg(productID(), 0, 16);
}

quint32 DeviceInfo::vendor()
{
    Q_D(DeviceInfo);
    return d->vendor;
}

quint32 DeviceInfo::productID()
{
    Q_D(DeviceInfo);
    return d->productID;
}

quint32 DeviceInfo::version()
{
    Q_D(DeviceInfo);
    return d->version;
}

quint32 DeviceInfo::bus()
{
    Q_D(DeviceInfo);
    return d->bus;
}

quint32 DeviceInfo::deviceNumber()
{
    Q_D(DeviceInfo);
    return d->deviceNumber;
}

quint32 DeviceInfo::interfaceNumber()
{
    Q_D(DeviceInfo);
    return d->interfaceNumber;
}

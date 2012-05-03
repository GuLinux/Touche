#include "inputregister.h"
#include <QDebug>

class InputRegisterPrivate {
public:
    InputRegisterPrivate(uint hid, uint value) : hid(hid), value(value){}
    uint hid;
    uint value;
};

InputRegister::InputRegister(uint hid, uint value, QObject *parent) :
    QObject(parent), d_ptr(new InputRegisterPrivate(hid, value))
{
}

InputRegister::~InputRegister()
{
    delete d_ptr;
}

uint InputRegister::hid()
{
    Q_D(InputRegister);
    return d->hid;
}

uint InputRegister::value()
{
    Q_D(InputRegister);
    return d->value;
}

QHash<QString, QVariant> InputRegister::toHash()
{
    QHash<QString, QVariant> hash;
    hash.insert("hid", hid());
    hash.insert("value", QVariant(value()));
    return hash;
}

bool InputRegister::operator ==(InputRegister &other)
{
    qDebug() << "Operator == with this=[" << hid() << "," << value() << "] and other=[" << other.hid() << "," << other.value() << "]";
    return other.value() == value() && other.hid() == hid();
}


#include "inputevent.h"
#include <QMap>
#include "inputregister.h"
#include <QDebug>
#include <QStringList>

class InputEventPrivate {
public:
    InputEventPrivate() {}
    QMap<uint, uint> registers;
};

InputEvent::InputEvent(QObject *parent) :
    QObject(parent), d_ptr(new InputEventPrivate())
{
}

InputEvent::~InputEvent()
{
    qDebug() << "InputEvent deleted";
    delete d_ptr;
}

void InputEvent::addRegister(uint hid, uint value)
{
    Q_D(InputEvent);
    d->registers.insert(hid, value);
}

QString InputEvent::asJSON()
{
    Q_D(InputEvent);
    QStringList registers;
    foreach(uint key, d->registers.keys()) {
        registers << QString("{ \"hid\":%1, \"value\":%2 }").arg(key, 10).arg(d->registers.value(key), 10);
    }
    return QString("[\n%1\n]\n").arg(registers.join(",\n"));
}

bool InputEvent::matches(InputEvent *other)
{
    Q_D(InputEvent);
    foreach(uint key, d->registers.keys()) {
        if(!other->hasRegister(key))
            return false;
        if(! (registerAt(key) == other->registerAt(key)))
            return false;
    }
    return true;
}

uint InputEvent::registerAt(uint hid)
{
    Q_D(InputEvent);
    return d->registers.value(hid);
}

bool InputEvent::hasRegister(uint hid)
{
    Q_D(InputEvent);
    return d->registers.contains(hid);
}

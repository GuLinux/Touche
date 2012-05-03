#include "inputevent.h"
#include <QMultiMap>
#include "inputregister.h"
#include <QDebug>
#include <QStringList>
#include <QPair>

class InputEventPrivate {
public:
    InputEventPrivate() {}
    QMultiMap<uint, RegisterValue> registers;

    QList<uint> valuesFor(const QList<RegisterValue> registerValues) {
        QList<uint> result;
        foreach(RegisterValue registerValue, registerValues) {
            result << registerValue.first;
        }
        return result;
    }

    bool registersAreDifferent(const RegisterValue &mine, const QList<RegisterValue> &other) {
        return !valuesFor(other).contains(mine.first);
    }
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

void InputEvent::addRegister(uint hid, uint value, uint index)
{
    Q_D(InputEvent);
    d->registers.insert(hid, RegisterValue(value, index));
}

QString InputEvent::asJSON()
{
    Q_D(InputEvent);
    QStringList registers;
    QList<RegisterValue> values = d->registers.values();
    auto sortByIndex = [](const RegisterValue &first, const RegisterValue &second) {return first.second<second.second;};
    qSort(values.begin(), values.end(), sortByIndex );
    foreach(RegisterValue value, values) {
        registers << QString("{ \"hid\":%1, \"value\":%2, \"index\":%3 }").arg(d->registers.key(value), 10).arg(value.first, 10).arg(value.second, 3, 10, QChar('0'));
    }
    return QString("[\n%1\n]\n").arg(registers.join(",\n"));
}

bool InputEvent::matches(InputEvent *other)
{
    Q_D(InputEvent);
    foreach(uint key, d->registers.keys()) {
        if(!other->hasRegister(key))
            return false;
        if( d->registersAreDifferent(registersFor(key).first(), other->registersFor(key) )) // we are on ConfigRegister, therefore we assume we've only one value
            return false;
    }
    return true;
}

QList<RegisterValue> InputEvent::registersFor(uint hid)
{
    Q_D(InputEvent);
    return d->registers.values(hid);
}

bool InputEvent::hasRegister(uint hid)
{
    Q_D(InputEvent);
    return d->registers.contains(hid);
}

/***********************************************************************
Copyright (c) 2012 "Marco Gulino <marco.gulino@gmail.com>"

This file is part of Touché: https://github.com/rockman81/Touche

Touché is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details (included the COPYING file).

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "hid_inputevent.h"
#include <QMultiMap>
#include <QDebug>
#include <QStringList>
#include <QPair>

class HidInputEventPrivate {
public:
    HidInputEventPrivate() {}
    QMultiMap<uint, RegisterValue> registers;

    QList<uint> valuesFor(const QList<QVariant> registerValues) {
        QList<uint> result;
        foreach(QVariant registerValue, registerValues) {
            result << registerValue.toUInt();
        }
        return result;
    }

    bool registersAreDifferent(const RegisterValue &mine, const QList<QVariant> &other) {
        return !valuesFor(other).contains(mine.first);
    }
};

HidInputEvent::HidInputEvent(QObject *parent) :
    QObject(parent), d_ptr(new HidInputEventPrivate())
{
}

HidInputEvent::~HidInputEvent()
{
    delete d_ptr;
}

void HidInputEvent::addRegister(uint hid, uint value, uint index)
{
    Q_D(HidInputEvent);
    d->registers.insert(hid, RegisterValue(value, index));
}

QString HidInputEvent::asJSON()
{
    Q_D(HidInputEvent);
    QStringList registers;
    QList<RegisterValue> values = d->registers.values();
    auto sortByIndex = [](const RegisterValue &first, const RegisterValue &second) {return first.second<second.second;};
    qSort(values.begin(), values.end(), sortByIndex );
    foreach(RegisterValue value, values) {
        registers << QString("{ \"hid\":%1, \"value\":%2, \"index\":%3 }").arg(d->registers.key(value), 10).arg(value.first, 10).arg(value.second, 3, 10, QChar('0'));
    }
    return QString("[\n%1\n]\n").arg(registers.join(",\n"));
}


bool HidInputEvent::matches(const QVariantMap &payload)
{
    Q_D(HidInputEvent);
    const QVariantMap registers = payload.value("registers").toMap();
    if(registers.isEmpty()) {
        return false;
    }
    foreach(QString key, registers.keys()) {
        uint hid = key.toUInt();
        if(!hasRegister(hid))
            return false;
        if(d->registersAreDifferent(registersFor(hid).first(), registers.values(key)))
            return false;
    }
    return true;
}


QList<RegisterValue> HidInputEvent::registersFor(uint hid)
{
    Q_D(HidInputEvent);
    return d->registers.values(hid);
}

bool HidInputEvent::hasRegister(uint hid)
{
    Q_D(HidInputEvent);
    return d->registers.contains(hid);
}

uint HidInputEvent::registersCount()
{
    Q_D(HidInputEvent);
    return d->registers.values().count();
}



HidInputEvent::operator QString()
{
    return asJSON();
}

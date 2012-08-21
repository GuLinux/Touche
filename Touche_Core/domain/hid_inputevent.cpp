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
    QMultiMap<uint, uint> registers;
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
    Q_UNUSED(index);
    d->registers.insert(hid, value);
}

QString HidInputEvent::asJSON()
{
    Q_D(HidInputEvent);
    QStringList registers;
    foreach(uint hid, d->registers.keys()) {
        foreach(uint value, d->registers.values(hid))
            registers << QString("{ \"hid\":%1, \"value\":%2 }")
                     .arg(hid, 10)
                     .arg(value, 10);
    }
    return QString("[\n%1\n]\n").arg(registers.join(",\n"));
    return QString();
}


bool HidInputEvent::matches(const QVariantMap &payload)
{
    Q_D(HidInputEvent);

    const QVariantList registers = payload.value("hiddev_registers").toList();
    if(registers.isEmpty()) {
        return false;
    }
    foreach(QVariant hidRegister, registers) {
        uint hid = hidRegister.toMap().value("hid").toUInt();
        uint value = hidRegister.toMap().value("value").toUInt();

        if(!d->registers.contains(hid))
            return false;
        if(!d->registers.values(hid).contains(value))
            return false;
    }
    return true;
}

HidInputEvent::operator QString()
{
    return asJSON();
}

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


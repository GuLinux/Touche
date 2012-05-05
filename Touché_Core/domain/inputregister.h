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

#ifndef KEYREGISTER_H
#define KEYREGISTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>

class InputRegisterPrivate;
class InputRegister : public QObject
{
    Q_OBJECT
public:
    explicit InputRegister(uint hid, uint value, QObject *parent = 0);
    ~InputRegister();
    uint hid();
    uint value();
    QHash<QString, QVariant> toHash();
    virtual bool operator ==(InputRegister &other);

private:
    InputRegisterPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(InputRegister)
    
};

#endif // KEYREGISTER_H

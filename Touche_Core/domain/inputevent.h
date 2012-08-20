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

#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <QtCore/QObject>
#include <QPair>
#include <QList>

class InputEventPrivate;

typedef QPair<uint, uint> RegisterValue;

class InputEvent : public QObject
{
    Q_OBJECT
public:
    explicit InputEvent(QObject *parent = 0);
    ~InputEvent();
    void addRegister(uint hid, uint value, uint index);
    QString asJSON();
    uint registersCount();
    QList<RegisterValue> registersFor(uint hid);
    bool hasRegister(uint hid);
    virtual bool matches(InputEvent *other);
signals:
    
public slots:
    
private:
    InputEventPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(InputEvent)
};

#endif // KEYEVENT_H

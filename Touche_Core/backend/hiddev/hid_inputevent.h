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

#ifndef HID_INPUTEVENT_H
#define HID_INPUTEVENT_H

#include <QtCore/QObject>
#include <QPair>
#include <QList>
#include "domain/inputevent.h"

class HidInputEventPrivate;

class HidInputEvent : public QObject, public InputEvent
{
    Q_OBJECT
public:
    explicit HidInputEvent(QObject *parent = 0);
    ~HidInputEvent();
    void addRegister(uint hid, uint value, uint index);
    QString asJSON();
    virtual bool matches(const QVariantMap &payload);
    virtual operator QString();
signals:
    
public slots:
    
private:
    HidInputEventPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(HidInputEvent)
};

#endif // HID_INPUTEVENT_H

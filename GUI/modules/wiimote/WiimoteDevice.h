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

#ifndef WIIMOTEDEVICE_H
#define WIIMOTEDEVICE_H
#include "domain/Device.h"
#include <QStringList>
#include "domain/inputevent.h"
#include <QMap>
#include <QDateTime>
#include <iostream>
#include <QDebug>

class WiimoteManager;
class DeviceInfo;
class KeyboardDatabase;

class WiimoteInputEvent : public QObject, public InputEvent {
    Q_OBJECT
public:
    WiimoteInputEvent(const QString &key, const QString &event, QObject *parent =0);
    virtual bool matches(const QVariantMap &payload);
    inline virtual QObject *nearestQObject() { return this; }
    inline virtual operator QString() { return key; }
    friend QDebug operator<<(QDebug d, const WiimoteInputEvent &e);
private:
    const QString key;
    const QString event;
};


class WiimoteDevice : public Device
{
    Q_OBJECT
public:
    explicit WiimoteDevice(WiimoteManager *wiimoteManager, KeyboardDatabase *keyboardDatabase, QObject *parent = 0);
    virtual ~WiimoteDevice();

public slots:
    virtual void deviceRemoved(DeviceInfo *deviceInfo);
    virtual void deviceChanged();
    virtual void stop();
private slots:
    void wiimoteConnected(const QString &address);
    void wiimoteDisconnected(const QString &address);
    void buttonsDown(const QStringList &buttons);
private:
    WiimoteManager *wiimoteManager;
    DeviceInfo *deviceInfo;
    QStringList m_buttons;
    KeyboardDatabase *keyboardDatabase;
    QMap<QString,QDateTime> keyReleaseEvents;
};

#endif // WIIMOTEDEVICE_H

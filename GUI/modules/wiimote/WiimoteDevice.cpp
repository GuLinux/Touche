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

#include "WiimoteDevice.h"
#include "wiimotemanager.h"
#include "domain/deviceinfo.h"
#include <KDebug>
#include <QTimer>
#include "backend/config/keyboarddatabase.h"

WiimoteDevice::WiimoteDevice(WiimoteManager *wiimoteManager, KeyboardDatabase *keyboardDatabase, QObject *parent) :
    Device(parent), wiimoteManager(wiimoteManager), deviceInfo(0), keyboardDatabase(keyboardDatabase)
{
    connect(wiimoteManager, SIGNAL(connected(QString)), this, SLOT(wiimoteConnected(QString)));
    connect(wiimoteManager, SIGNAL(disconnected(QString)), this, SLOT(wiimoteDisconnected(QString)));
    connect(wiimoteManager, SIGNAL(buttonsDown(QStringList)), this, SLOT(buttonsDown(QStringList)));
}


WiimoteDevice::~WiimoteDevice()
{
}


void WiimoteDevice::deviceRemoved(DeviceInfo *deviceInfo)
{
    Q_UNUSED(deviceInfo)
}


void WiimoteDevice::deviceChanged()
{
}


void WiimoteDevice::stop()
{
}


void WiimoteDevice::wiimoteDisconnected(const QString &address)
{
    Q_UNUSED(address)
    emit disconnected(deviceInfo);
    delete deviceInfo;
    deviceInfo = 0;
}


void WiimoteDevice::wiimoteConnected(const QString &address)
{
    deviceInfo = new DeviceInfo(this);
    deviceInfo->name("Wiimote");
    deviceInfo->path(address);
    deviceInfo->configurationIdentifier("WiimoteModule");
    deviceInfo->keyboardDatabaseEntry(keyboardDatabase->deviceConfiguration(deviceInfo));
    emit connected(deviceInfo);
}


void WiimoteDevice::buttonsDown(const QStringList &buttons)
{
    foreach(const QString button, m_buttons) {
        if(!buttons.contains(button)) {
            WiimoteInputEvent *keyEvent = new WiimoteInputEvent(button, "keyrelease", this);
            emit inputEvent(keyEvent, deviceInfo);
            QTimer::singleShot(30000, keyEvent, SLOT(deleteLater()));
            m_buttons.removeAll(button);
        }
    }

    foreach(const QString button, buttons) {
        if(!m_buttons.contains(button)) {
            WiimoteInputEvent *keyEvent = new WiimoteInputEvent(button, "keypress", this);
            emit inputEvent(keyEvent, deviceInfo);
            QTimer::singleShot(30000, keyEvent, SLOT(deleteLater()));
            m_buttons << button;
        }
    }
}


WiimoteInputEvent::WiimoteInputEvent(const QString &key, const QString &event, QObject *parent)
    : QObject(parent), key(key), event(event)
{
}


bool WiimoteInputEvent::matches(const QVariantMap &payload)
{
    kDebug() << "Payload: " << payload;
    kDebug() << "myself: " << key << "_" << event;
    return QString("%1_%2").arg(key).arg(event) == payload.value("keyname").toString();
}

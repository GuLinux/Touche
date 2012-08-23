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

#include "cfgkey.h"
#include "cfgkeyevent.h"

CfgKey::CfgKey(const QString &keyName, const QMap<QString, QVariant> &eventsMap, QObject *parent) :
    QObject(parent), m_name(keyName)
{
    foreach(const QString key, eventsMap.keys()) {
        m_keyEvents << new CfgKeyEvent(key, this);
    }
}

QString CfgKey::keyName() const
{
    return m_name;
}

QString CfgKey::label() const
{
    return keyName();
}




CfgKeyListModel::CfgKeyListModel(const QList<CfgKey*> &keys, QObject *parent) :
    QAbstractListModel(parent), m_items(keys)
{
}

int CfgKeyListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant CfgKeyListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_items.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_items[index.row()]->label();
    else
        return QVariant();
}

CfgKey *CfgKeyListModel::item(const QModelIndex &index) const
{
    return m_items[index.row()];
}

QList<CfgKeyEvent*> CfgKey::cfgKeyEvents()
{
    return m_keyEvents;
}

QModelIndex CfgKeyListModel::findKey(const QString &keyName)
{
    for(int m_index=0; m_index<m_items.size(); m_index++) {
        CfgKey *cfgKey = m_items[m_index];
        if(cfgKey->keyName() == keyName)
            return index(m_index);
    }
    return QModelIndex();
}

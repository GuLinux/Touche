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

#include "cfgkeyevent.h"
#include "cfgkey.h"

CfgKeyEvent::CfgKeyEvent(const QString &keyEventName, CfgKey *parent) :
    QObject(parent), m_keyEventName(keyEventName), cfgKey(parent)
{
}

CfgKeyEventListModel::CfgKeyEventListModel(const QList<CfgKeyEvent*> &keys, QObject *parent) :
    QAbstractListModel(parent), m_items(keys)
{
}

int CfgKeyEventListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant CfgKeyEventListModel::data(const QModelIndex &index, int role) const
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

CfgKeyEvent *CfgKeyEventListModel::item(const QModelIndex &index) const
{
    return m_items[index.row()];
}

QString CfgKeyEvent::label() const
{
    return m_keyEventName;
}

QString CfgKeyEvent::configKey() const {
    return QString("%1/%2").arg(cfgKey->label(), label());
}

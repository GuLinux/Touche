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

#ifndef CFGKEY_H
#define CFGKEY_H

#include <QObject>
#include <QDebug>
#include <QList>
#include "cfgkeyevent.h"
#include <QAbstractListModel>

class CfgKey;

class CfgKeyListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CfgKeyListModel(const QList<CfgKey*> &items , QObject *parent = 0);
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    CfgKey *item(const QModelIndex &index) const;
private:
    QList<CfgKey*> const m_items;
signals:

public slots:

};


class CfgKey : public QObject
{
    Q_OBJECT
public:
    explicit CfgKey(const QString &keyName, const QMap<QString, QVariant> &eventsMap, QObject *parent = 0);
    QString keyName() const;
    virtual QString label() const;
    QList<CfgKeyEvent *> cfgKeyEvents();

signals:
    
public slots:

private:
    QString const m_name;
    QList<CfgKeyEvent*> m_keyEvents;
    
public:
    inline friend QDebug operator<< (QDebug s, const CfgKey* cfgKey) {
        s.nospace() << "CfgKey(" << cfgKey->m_name << ", events: " << cfgKey->m_keyEvents << ")";
        return s.space();
    }
    inline friend QDebug operator<< (QDebug s, const CfgKey& cfgKey) {
        return s << &cfgKey;
    }
};

#endif // CFGKEY_H

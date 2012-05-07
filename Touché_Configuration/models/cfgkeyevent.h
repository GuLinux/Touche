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

#ifndef CFGKEYEVENT_H
#define CFGKEYEVENT_H

#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
class CfgKeyEvent;
class CfgKey;
class CfgKeyEventListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CfgKeyEventListModel(const QList<CfgKeyEvent*> &items , QObject *parent = 0);
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    CfgKeyEvent *item(const QModelIndex &index) const;
private:
    QList<CfgKeyEvent*> const m_items;
signals:

public slots:

};

class CfgKeyEvent : public QObject
{
    Q_OBJECT
public:
    explicit CfgKeyEvent(const QString &keyEventName, CfgKey *parent);
    QString label() const;
signals:
    
public slots:
private:
    QString const m_keyEventName;
    CfgKey *cfgKey;
public:
    inline friend QDebug operator<< (QDebug s, const CfgKeyEvent* cfgKeyEvent) {
        s.nospace() << "CfgKeyEvent(" << cfgKeyEvent->m_keyEventName << ")";
        return s.space();
    }
    inline friend QDebug operator<< (QDebug s, const CfgKeyEvent& cfgKeyEvent) {
        return s << &cfgKeyEvent;
    }
    
    QString configKey() const;
};

#endif // CFGKEYEVENT_H

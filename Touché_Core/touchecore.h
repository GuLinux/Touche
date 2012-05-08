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

#ifndef TOUCHECORE_H
#define TOUCHECORE_H

#include <QObject>
#include <QMap>
class DeviceInfo;
class ToucheCorePrivate;
class ToucheCore : public QObject
{
    Q_OBJECT
public:
    explicit ToucheCore(const QStringList &supportedOptions, QObject *parent = 0);
    ~ToucheCore();
    static QMap<QString, QString> supportedOptions();
signals:
    void connected(DeviceInfo*);
    void disconnected(DeviceInfo*);

public slots:
    void start();
    void quit();

private:
    ToucheCorePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ToucheCore)
    
};

#endif // TOUCHECORE_H

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

#include <QtCore/QObject>
#include <QMap>
#include <KLocale>
#include <KStandardDirs>
#include <KGlobal>
class DeviceInfo;
class ToucheCorePrivate;

class Touche {
public:
    static const inline char* appName() { return I18N_NOOP2("Application internal name", "Touche"); }
    static const inline char* displayName() { return I18N_NOOP2("Application display name", "Touché"); }
    static const inline char* iconName() { return "input-keyboard" ; }
    static inline QString configFile() { return KStandardDirs::locateLocal("config", "ToucheBindings"); }
    static inline QStringList keyboardDatabases()
        { return KGlobal::dirs()->findAllResources("data", "Touche/keyboard_database.json"); }
};

class ToucheCore : public QObject
{
    Q_OBJECT
public:
    explicit ToucheCore(const QStringList &supportedOptions, QObject *parent = 0);
    ~ToucheCore();
    static QMap<QString, QString> supportedOptions();
    QStringList availableProfiles() const;
    QString currentProfile() const;
signals:
    void connected(DeviceInfo*);
    void disconnected(DeviceInfo*);
    void inputEvent(const QString &eventName);
    void profileChanged(const QString &profileName);

public slots:
    void start();
    void quit();
    void suspendEventsTranslation();
    void resumeEventsTranslation();
    void setProfile(const QString &profileName);

private:
    ToucheCorePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ToucheCore)
    
};

#endif // TOUCHECORE_H

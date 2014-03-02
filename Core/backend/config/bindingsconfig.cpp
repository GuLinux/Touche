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

#include "bindingsconfig.h"
#include "domain/binding.h"
#include "domain/runcommandbinding.h"
#include "domain/tokeybinding.h"
#include "domain/donothingbinding.h"
#include <QMap>
#include <QSettings>
#include <QCoreApplication>
#include "touchecore.h"
#include <KDebug>
#include <functional>

#define BINDING_DO_NOTHING "DoNothing"
#define BINDING_RUN_COMMAND "RunCommand"
#define BINDING_TO_KEY "TranslateToKey"
#define BINDING_NOT_FOUND "****NotFound****"

typedef std::function<Binding*(QObject*, const QString&, const QString&)> BindingFactory;


class BindingsConfigPrivate {
public:
    BindingsConfigPrivate(BindingsConfig *parent)
        : settings(Touche::settings(parent)) {}

    QMap<QString, BindingFactory> bindings;
    QSettings *settings;
    QString profile;
    DoNothingBinding doNothingBinding;

    Binding* moveToThreadAndReparent(QObject *newParent, QObject *object) {
        object->moveToThread(newParent->thread());
        object->setParent(newParent);
        return dynamic_cast<Binding*>(object);
    }

    void loadBindings() {
        if(settings->group() != QString() )
            settings->endGroup();
        settings->setValue("last_profile", profile);
        settings->beginGroup(profile);
        const QString params = QString("%1/%2/%3");

        bindings[BINDING_RUN_COMMAND] = [this,params](QObject* p, const QString& eventName, const QString& group) {
            settings->beginGroup(group);
            QString commandName = settings->value(params.arg(eventName, BINDING_RUN_COMMAND, "ApplicationName"), "true").toString();
            QStringList arguments = settings->value(params.arg(eventName, BINDING_RUN_COMMAND, "Arguments"), QStringList()).toStringList();
            settings->endGroup();
            return moveToThreadAndReparent(p, new RunCommandBinding(commandName, arguments));
        };
        bindings[BINDING_TO_KEY] =[this,params](QObject* p, const QString& eventName, const QString& group) {
            settings->beginGroup(group);
            QString keySymbol = settings->value(params.arg(eventName, BINDING_TO_KEY, "keysymbol"), QString()).toString();
            QString isKeypress = settings->value(params.arg(eventName,BINDING_TO_KEY, "eventtype"), "keypress").toString();
            settings->endGroup();
            return moveToThreadAndReparent(p, new ToKeyBinding(keySymbol, isKeypress == "keypress", true));
        };
    }
};

BindingsConfig::BindingsConfig(QObject *parent) :
    QObject(parent), d_ptr(new BindingsConfigPrivate(this))
{
    Q_D(BindingsConfig);
    d->bindings[BINDING_DO_NOTHING] = [d](QObject* p, const QString& e, const QString& g) {
            Q_UNUSED(e);
            Q_UNUSED(g);
            Q_UNUSED(p);
            return &d->doNothingBinding; };

    QString firstProfile = "Default";
    foreach(QString profile, availableProfiles()) {
        if(firstProfile == "Default" ) firstProfile = profile;
        if(QString("bindings_%1").arg(profile) == d->settings->value("last_profile"))
            setCurrentProfile(profile);
    }
    if(d->profile == QString() ) {
        setCurrentProfile(firstProfile);
    }
    kDebug() << "Current profile: " << d->profile;
}

BindingsConfig::~BindingsConfig()
{
    delete d_ptr;
}

Binding *BindingsConfig::bindingFor(const QString &eventName, QObject *parent)
{
    Q_D(BindingsConfig);
    kDebug() << __PRETTY_FUNCTION__ << ": " << eventName;
    d->settings->beginGroup(d->profile);
    QString bindingSetting = d->settings->value(eventName, BINDING_NOT_FOUND).toString();
    QString foundInGroup = d->settings->group();
    kDebug() << "Profile: " << d->settings->group() << ", event" << eventName << ", found binding (1st try): " << bindingSetting;
    d->settings->endGroup();
    if(bindingSetting == BINDING_NOT_FOUND) {
        d->settings->beginGroup("bindings_Default");
        bindingSetting = d->settings->value(eventName, BINDING_DO_NOTHING).toString();
        foundInGroup = d->settings->group();
        kDebug() << "Profile: " << d->settings->group() << ", event" << eventName << ", found binding (2st try): " << bindingSetting;
        d->settings->endGroup();
    }
    BindingFactory bindingFactory = d->bindings.value(bindingSetting, d->bindings.value(BINDING_DO_NOTHING));
    return bindingFactory(parent, eventName, foundInGroup);
}



void BindingsConfig::setCurrentProfile(const QString &profileName)
{
    Q_D(BindingsConfig);
    if(profileName == QString() ) return;
    d->profile = QString("bindings_%1").arg(profileName);
    d->settings->beginGroup(d->profile);
    d->settings->setValue("name", profileName);
    d->settings->endGroup();
    d->loadBindings();
    emit profileChanged(profileName);
}


QStringList BindingsConfig::availableProfiles() const
{
    Q_D(const BindingsConfig);
    if(d->settings->group() != QString() )
        d->settings->endGroup();
    QStringList allProfiles = d->settings->childGroups();
    foreach(QString group, allProfiles)
        if(!group.startsWith("bindings_")) allProfiles.removeAll(group);
    return allProfiles.replaceInStrings("bindings_", "");
}


QString BindingsConfig::currentProfile() const
{
    Q_D(const BindingsConfig);
    return QString(d->profile).replace("bindings_", "");
}

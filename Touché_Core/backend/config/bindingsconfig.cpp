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
#include <functional>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

#define BINDING_DO_NOTHING "DoNothing"
#define BINDING_RUN_COMMAND "RunCommand"
#define BINDING_TO_KEY "TranslateToKey"

typedef std::function<Binding*(QObject*, const QString&)> BindingFactory;


class BindingsConfigPrivate {
public:
    BindingsConfigPrivate(BindingsConfig *parent) : settings(new QSettings("GuLinux", qAppName(), parent)) { }

    QMap<QString, BindingFactory> bindings;
    QSettings *settings;
    DoNothingBinding doNothingBinding;

    Binding* moveToThreadAndReparent(QObject *newParent, QObject *object) {
        object->moveToThread(newParent->thread());
        object->setParent(newParent);
        return dynamic_cast<Binding*>(object);
    }
};

BindingsConfig::BindingsConfig(QObject *parent) :
    QObject(parent), d_ptr(new BindingsConfigPrivate(this))
{
    Q_D(BindingsConfig);
    d->settings->beginGroup("bindings");
    const QString params = QString("%1/%2/%3");

    d->bindings[BINDING_DO_NOTHING] = [d](QObject* p, const QString& e) {Q_UNUSED(e); Q_UNUSED(p); return &d->doNothingBinding; };
    d->bindings[BINDING_RUN_COMMAND] = [d,params](QObject* p, const QString& eventName) {
        QString commandName = d->settings->value(params.arg(eventName, BINDING_RUN_COMMAND, "ApplicationName"), "true").toString();
        QStringList arguments = d->settings->value(params.arg(eventName, BINDING_RUN_COMMAND, "Arguments"), QStringList()).toStringList();
        return d->moveToThreadAndReparent(p, new RunCommandBinding(commandName, arguments));
    };
    d->bindings[BINDING_TO_KEY] =[d,params](QObject* p, const QString& eventName) {
        QString keySymbol = d->settings->value(params.arg(eventName, BINDING_TO_KEY, "keysymbol"), QString()).toString();
        QString isKeypress = d->settings->value(params.arg(eventName,BINDING_TO_KEY, "eventtype"), "keypress").toString();
        return d->moveToThreadAndReparent(p, new ToKeyBinding(keySymbol, isKeypress == "keypress", true));
    };

    qDebug() << "Configured bindings: ";
    foreach(const QString key, d->settings->childKeys()) {
        qDebug() << key << " [" << d->settings->value(key).toString() << "]";
    }

}

BindingsConfig::~BindingsConfig()
{
    delete d_ptr;
}

Binding *BindingsConfig::bindingFor(const QString &eventName, QObject *parent)
{
    Q_D(BindingsConfig);
    QString bindingSetting = d->settings->value(eventName, BINDING_DO_NOTHING).toString();
    qDebug() << "Setting for event " << eventName << ": " << bindingSetting;
    BindingFactory bindingFactory = d->bindings.value(bindingSetting, d->bindings.value(BINDING_DO_NOTHING));
    return bindingFactory(parent, eventName);
}


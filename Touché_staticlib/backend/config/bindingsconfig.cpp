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

typedef std::function<Binding*(QObject*, const QString&)> BindingFactory;


class BindingsConfigPrivate {
public:
    BindingsConfigPrivate(BindingsConfig *parent) : settings(new QSettings("GuLinux", qAppName(), parent)) { }
    BindingFactory doNothingBindingFactory =  [this](QObject* p, const QString& e) {Q_UNUSED(e); return this->moveToThreadAndReparent(p, new DoNothingBinding()); };
    QMap<QString, BindingFactory> bindings;
    QSettings *settings;

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
    d->bindings["RunCommand"] = [d](QObject* p, const QString& eventName) {
        QString commandName = d->settings->value( eventName + "_RunCommand_ApplicationName", "true").toString();
        QStringList arguments = d->settings->value(eventName + "_RunCommand_Arguments", QStringList()).toStringList();
        return d->moveToThreadAndReparent(p, new RunCommandBinding(commandName, arguments));
    };
    d->bindings["TranslateToKey"] =[d](QObject* p, const QString& eventName) {
            qDebug() << "Creating TranslateToKey with parent object: " << p;
        QString keySymbol = d->settings->value(eventName + "_TranslateToKey_keysymbol", QString()).toString();
        QString iskeypress = d->settings->value(eventName + "_TranslateToKey_eventtype", "keypress").toString();
        return d->moveToThreadAndReparent(p, new ToKeyBinding(keySymbol, iskeypress == "keypress"));
    };
}

BindingsConfig::~BindingsConfig()
{
    delete d_ptr;
}

Binding *BindingsConfig::bindingFor(const QString &eventName, QObject *parent)
{
    Q_D(BindingsConfig);
    QString bindingSetting = d->settings->value(eventName, "DoNothing").toString();
    qDebug() << "Setting for event " << eventName << ": " << bindingSetting;
    BindingFactory bindingFactory = d->bindings.value(bindingSetting, d->doNothingBindingFactory);
    return bindingFactory(parent, eventName);
}


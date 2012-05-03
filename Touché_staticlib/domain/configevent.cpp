#include "configevent.h"
#include "inputevent.h"
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include "backend/config/bindingsconfig.h"
#include "domain/binding.h"

class BindingsConfig;
class ConfigEventPrivate {
public:
    QMap<QString, InputEvent*> cfgInputEvents;
};

ConfigEvent::ConfigEvent(QObject *parent) :
    QObject(parent), d_ptr(new ConfigEventPrivate())
{
}

ConfigEvent::~ConfigEvent()
{
    delete d_ptr;
}

bool ConfigEvent::matches(InputEvent *other, const QStringList &tags, BindingsConfig *bindingsConfig)
{
    Q_D(ConfigEvent);

    foreach(QString tag, tags) {
        InputEvent *cfgInputEvent = d->cfgInputEvents.value(tag);
        if(cfgInputEvent->matches(other)) {
            QString eventName = QString("%1_%2")
                    .arg(property("keyName").toString() )
                    .arg(tag);
            qDebug() << "Got match: " << eventName;
            bindingsConfig->bindingFor(eventName, other)->execute();
            return true;
        }
    }

    return false;
}

void ConfigEvent::addInputEvent(const QString &tag, InputEvent *inputEvent)
{
    Q_D(ConfigEvent);
    d->cfgInputEvents.insert(tag, inputEvent);
}

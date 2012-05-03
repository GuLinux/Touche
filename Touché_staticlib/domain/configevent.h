#ifndef CONFIGEVENT_H
#define CONFIGEVENT_H

#include <QObject>
#include <QPair>

class InputEvent;
class ConfigEventPrivate;
class BindingsConfig;
class ConfigEvent : public QObject
{
    Q_OBJECT
public:
    explicit ConfigEvent(QObject *parent = 0);
    ~ConfigEvent();
    void addInputEvent(const QString &tag, InputEvent *inputEvent);

    bool matches(InputEvent *other, const QStringList &tags, BindingsConfig *bindingsConfig);
signals:
    
public slots:

private:
    ConfigEventPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(ConfigEvent)
};

#endif // CONFIGEVENT_H

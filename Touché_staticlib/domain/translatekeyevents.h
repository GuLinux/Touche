#ifndef TRANSLATEKEYEVENTS_H
#define TRANSLATEKEYEVENTS_H

#include <QObject>

class InputEvent;
class TranslateKeyEventsPrivate;
class KeyboardDatabase;
class DeviceInfo;
class BindingsConfig;
class TranslateKeyEvents : public QObject
{
    Q_OBJECT
public:
    explicit TranslateKeyEvents(KeyboardDatabase *keyboardDatabase, BindingsConfig *bindingsConfig, QObject *parent = 0);
    ~TranslateKeyEvents();
signals:
    
public slots:
    void event(InputEvent *keyEvent, DeviceInfo *deviceInfo);

private:
    TranslateKeyEventsPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(TranslateKeyEvents)
};

#endif // TRANSLATEKEYEVENTS_H

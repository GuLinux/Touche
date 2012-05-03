#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <QObject>

class InputEventPrivate;
class InputRegister;

class InputEvent : public QObject
{
    Q_OBJECT
public:
    explicit InputEvent(QObject *parent = 0);
    ~InputEvent();
    void addRegister(uint hid, uint value);
    QString asJSON();
    uint registerAt(uint hid);
    bool hasRegister(uint hid);
    bool matches(InputEvent *other);
signals:
    
public slots:
    
private:
    InputEventPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(InputEvent)
};

#endif // KEYEVENT_H

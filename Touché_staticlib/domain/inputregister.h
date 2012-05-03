#ifndef KEYREGISTER_H
#define KEYREGISTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>

class InputRegisterPrivate;
class InputRegister : public QObject
{
    Q_OBJECT
public:
    explicit InputRegister(uint hid, uint value, QObject *parent = 0);
    ~InputRegister();
    uint hid();
    uint value();
    QHash<QString, QVariant> toHash();
    virtual bool operator ==(InputRegister &other);

private:
    InputRegisterPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(InputRegister)
    
};

#endif // KEYREGISTER_H

#ifndef BINDINGSCONFIG_H
#define BINDINGSCONFIG_H

#include <QObject>

class Binding;
class BindingsConfigPrivate;
class BindingsConfig : public QObject
{
    Q_OBJECT
public:
    explicit BindingsConfig(QObject *parent = 0);
    ~BindingsConfig();
    Binding* bindingFor(const QString &eventName, QObject *parent);
signals:
    
public slots:

private:
    BindingsConfigPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(BindingsConfig)
    
};

#endif // BINDINGSCONFIG_H

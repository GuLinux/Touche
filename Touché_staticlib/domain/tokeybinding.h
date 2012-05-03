#ifndef TOKEYBINDING_H
#define TOKEYBINDING_H

#include <QObject>
#include "domain/binding.h"
class ToKeyBindingPrivate;
class ToKeyBinding : public QObject, public Binding
{
    Q_OBJECT
public:
    explicit ToKeyBinding(const QString &keySymName, bool iskeypress, QObject *parent = 0);
    ~ToKeyBinding();
    virtual void execute();
signals:
    
public slots:
    
private:
    ToKeyBindingPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ToKeyBinding)
};



#endif // TOKEYBINDING_H

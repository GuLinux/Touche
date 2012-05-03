#ifndef RUNCOMMANDBINDING_H
#define RUNCOMMANDBINDING_H
#include "domain/binding.h"
#include <QObject>
#include <QStringList>

class RunCommandBindingPrivate;
class RunCommandBinding : public QObject, public Binding
{
    Q_OBJECT
public:
    explicit RunCommandBinding(QString command, QStringList arguments=QStringList(), QObject *parent = 0);
    ~RunCommandBinding();
    virtual void execute();
signals:
    
public slots:
private:
    RunCommandBindingPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RunCommandBinding)
    
};

#endif // RUNCOMMANDBINDING_H

#ifndef DONOTHINGBINDING_H
#define DONOTHINGBINDING_H

#include <QObject>
#include "domain/binding.h"

class DoNothingBinding : public QObject, public Binding
{
    Q_OBJECT
public:
    explicit DoNothingBinding(QObject *parent = 0);
    ~DoNothingBinding();
    virtual void execute();

signals:
    
public slots:
    
};

#endif // DONOTHINGBINDING_H

#include "donothingbinding.h"
#include <QDebug>

DoNothingBinding::DoNothingBinding(QObject *parent) :
    QObject(parent)
{
}

DoNothingBinding::~DoNothingBinding()
{
    qDebug() << "DoNothingBinding deleted";
}

void DoNothingBinding::execute()
{
    qDebug() << "Executing \"DoNothingBinding\"";
}

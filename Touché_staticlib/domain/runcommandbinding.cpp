#include "runcommandbinding.h"
#include <QProcess>
#include <QDebug>

class RunCommandBindingPrivate {
public:
    RunCommandBindingPrivate(const QString &command, const QStringList &arguments) : command(command), arguments(arguments) {}
    QString const command;
    QStringList const arguments;
};

RunCommandBinding::RunCommandBinding(QString command, QStringList arguments, QObject *parent) :
    QObject(parent), d_ptr(new RunCommandBindingPrivate(command, arguments))
{
}

RunCommandBinding::~RunCommandBinding()
{
    qDebug() << "RunCommandBinding deleted";
    delete d_ptr;
}

void RunCommandBinding::execute()
{
    Q_D(RunCommandBinding);
    qDebug() << "executing \"RunCommandBinding\": command `" << d->command << "` with arguments [" << d->arguments.join(" ") << "]";
    QProcess::startDetached(d->command, d->arguments);
}


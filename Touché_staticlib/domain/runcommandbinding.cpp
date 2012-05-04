/***********************************************************************
Copyright (c) 2012 "Marco Gulino <marco.gulino@gmail.com>"

This file is part of Touché: https://github.com/rockman81/Touche

Touché is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details (included the COPYING file).

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

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
    delete d_ptr;
}

void RunCommandBinding::execute()
{
    Q_D(RunCommandBinding);
    qDebug() << "executing \"RunCommandBinding\": command `" << d->command << "` with arguments [" << d->arguments.join(" ") << "]";
    QProcess::startDetached(d->command, d->arguments);
}


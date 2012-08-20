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

#include "dumpkeys.h"
#include <QtCore/QTextStream>
#include "domain/inputevent.h"


DumpKeys::DumpKeys(QObject *parent) :
    QObject(parent)
{
}

DumpKeys::~DumpKeys()
{
}

void DumpKeys::inputEvent(InputEvent *keyEvent)
{
    QTextStream out(stdout);
    out << keyEvent->asJSON();
    out << endl;
    out.flush();
}
// o << QString("index[%1]:hid=[%2]v=[%3]").arg(i, 2, 16, QChar('0')).arg(ev[i].hid, 8, 16, QChar('0')).arg((uint)ev[i].value, 8, 16, QChar('0')) << endl;

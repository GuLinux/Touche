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

#include "tokeybinding.h"
#include <QDebug>

#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include <QX11Info>

class ToKeyBindingPrivate {
public:
    ToKeyBindingPrivate(const QString &keysymname, bool isKeypress) : keySymName(keysymname), isKeypress(isKeypress) {}
    QString const keySymName;
    bool isKeypress;
};

ToKeyBinding::ToKeyBinding(const QString &keySymName, bool isKeypress, QObject *parent) :
    QObject(parent), d_ptr(new ToKeyBindingPrivate(keySymName, isKeypress))
{
}

ToKeyBinding::~ToKeyBinding()
{
    delete d_ptr;
}

void ToKeyBinding::execute()
{
    Q_D(ToKeyBinding);
    KeySym keysym = XStringToKeysym(d->keySymName.toLatin1());
    KeyCode keycode = XKeysymToKeycode(QX11Info::display(), keysym);
    qDebug() << "executing \"ToKeyBinding\": sending key" << (d->isKeypress ? "press" : "release") << " with keysym:" << keysym << "; keycode: " << keycode;
    XTestFakeKeyEvent(QX11Info::display(), keycode, d->isKeypress, 0 );
}

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
#include <KDebug>
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include <QX11Info>
#include <QtCore/QTextStream>
#include <QProcessEnvironment>
#include "backend/updatekeysymbolmapping.h"

class ToKeyBindingPrivate {
public:
    ToKeyBindingPrivate(const QString &keysymname, bool isKeypress, bool associateXModMap) : keySymName(keysymname), isKeypress(isKeypress), associateXModMap(associateXModMap) {}
    QString const keySymName;
    bool isKeypress;
    bool associateXModMap;
};

ToKeyBinding::ToKeyBinding(const QString &keySymName, bool isKeypress, bool associateXModMap, QObject *parent) :
    QObject(parent), d_ptr(new ToKeyBindingPrivate(keySymName, isKeypress, associateXModMap))
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
    if(!keycode) {
        QTextStream err(stderr);
        err << "****** Warning: keysym \"" << d->keySymName << "\" seems to not being associated with any keycode.\n";
        if(d->associateXModMap) {
            err.flush();
            (new UpdateKeySymbolMapping(d->keySymName, this))->bind();
        } else {
            err << "****** You can associate it with xmodmap.\n";
            err << "The following procedure can apply:\n";
            err << "- Run \"xmodmap -pke\"\n";
            err << "- Find the first line with a keycode not associated (highly reccomended choosing a keycode > 100)\n";
            err << "- Open the file " << QProcessEnvironment::systemEnvironment().value("HOME") << "/.Xmodmap (create it if it doesn't exist)\n";
            err << "- append the line \n\tkeycode nnn = " << d->keySymName << endl;
            err << "- run \"xmodmap " << QProcessEnvironment::systemEnvironment().value("HOME") << "/.Xmodmap\"\n";
            err << "you can also add the last command to Autostart if your distribution doesn't do it already.\n";
            err.flush();
        }
        return;
    }
    kDebug() << QString("executing \"ToKeyBinding\": sending key%1  with keysym: \"%2\" [0x%3]; keycode:  %4").arg(d->isKeypress ? "press" : "release").arg(d->keySymName).arg(keysym, 0,16).arg(keycode);
    XTestFakeKeyEvent(QX11Info::display(), keycode, d->isKeypress, 0 );
}

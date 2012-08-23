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

#include "updatekeysymbolmapping.h"
#include <QDebug>
#include <X11/XKBlib.h>
#include <QX11Info>
#include <QFile>
#include <QtCore/QTextStream>
#include <QProcessEnvironment>
#include <QProcess>
#include <QFileInfo>

class UpdateKeySymbolMappingPrivate {
public:
    UpdateKeySymbolMappingPrivate(const QString &keySymbol) : keySymbol(keySymbol) {}
    QString const keySymbol;
};

UpdateKeySymbolMapping::UpdateKeySymbolMapping(const QString &keySymbol, QObject *parent) :
    QObject(parent), d_ptr(new UpdateKeySymbolMappingPrivate(keySymbol))
{
}

UpdateKeySymbolMapping::~UpdateKeySymbolMapping()
{
    delete d_ptr;
}

int UpdateKeySymbolMapping::bind()
{
    Q_D(UpdateKeySymbolMapping);
    QTextStream out(stdout);
    QTextStream err(stderr);
    QString xmodmapPath = QProcessEnvironment::systemEnvironment().value("HOME") + "/.Xmodmap";
    for(int kcode=100; kcode<256; kcode++) {
        KeySym keysym = XkbKeycodeToKeysym(QX11Info::display(), kcode,0, 0);
        if(!keysym) {
            out << "free keycode " << QString("%1").arg(kcode, 3, 10, QChar('0')) << "; trying to bind to " << d->keySymbol << " in ~/.Xmodmap\n";
            QFile xmodmap(xmodmapPath);
            if(!xmodmap.open(QFile::Append | QFile::ReadWrite)) {
                err << "Error: Unable to open " << xmodmapPath << " for writing\n";
                err << xmodmap.errorString() << endl;
                return -1;
            }
            QTextStream xmodmapStream(&xmodmap);
            QString currentContent = xmodmapStream.readAll();
            if(currentContent.contains(d->keySymbol)) {
                out << d->keySymbol << " already mapped; exiting";
            }
            if(!currentContent.endsWith('\n')) xmodmapStream << '\n';
            xmodmapStream << QString("keycode %1 = %2\n").arg(kcode).arg(d->keySymbol);
            xmodmap.close();
            out << "~/.Xmodmap written; trying to run xmodmap.\nNext keypress of " << d->keySymbol << " should be recognized.";
            QProcess::startDetached("xmodmap", QStringList(xmodmapPath));
            return kcode;
        }
    }
    return -1;
}

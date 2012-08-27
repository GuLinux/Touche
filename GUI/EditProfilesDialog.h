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

#ifndef EDITPROFILESDIALOG_H
#define EDITPROFILESDIALOG_H

#include <KDialog>

class ToucheCore;
class QSettings;
class KEditListBox;
class EditProfilesDialog : public KDialog
{
    Q_OBJECT
public:
    explicit EditProfilesDialog(ToucheCore *core, QWidget *parent = 0);
signals:
    
public slots:
    void accept();
    
private:
    QSettings *settings;
    KEditListBox *profilesList;
};

#endif // EDITPROFILESDIALOG_H

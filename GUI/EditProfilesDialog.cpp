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

#include "EditProfilesDialog.h"
#include "touchecore.h"
#include <QSettings>
#include <QVBoxLayout>
#include <KEditListBox>
#include <QApplication>
#include <QLabel>
#include <QDebug>

EditProfilesDialog::EditProfilesDialog(ToucheCore *core, QWidget *parent) :
    KDialog(parent)
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(mainWidget);
    settings = new QSettings(Touche::configFile(), QSettings::NativeFormat, this);
    setCaption(QString("%1 profiles").arg(i18n(Touche::displayName() )));
    profilesList = new KEditListBox();
    vlayout->addWidget(new QLabel("You can add new profiles here.\nEach profile will have its own key bindings."));
    vlayout->addWidget(profilesList);
    profilesList->setButtons(KEditListBox::Add | KEditListBox::Remove);
    setMainWidget(mainWidget);
    profilesList->insertStringList(core->availableProfiles());
}

void EditProfilesDialog::accept()
{
    qDebug() << "Saving profiles list...";
    foreach(QString profile, settings->childGroups()) {
        if(!profile.startsWith("bindings_")) continue;
        if(profilesList->items().contains(QString(profile).replace("bindings_", ""))) continue;
        settings->beginGroup(profile);
        settings->remove("");
        settings->endGroup();
    }

    foreach(QString profile, profilesList->items()) {
        if(settings->childGroups().contains(QString("bindings_%1").arg(profile))) continue;
        settings->beginGroup(QString("bindings_%1").arg(profile));
        settings->setValue("name", profile);
        settings->endGroup();
    }

    QDialog::accept();
}

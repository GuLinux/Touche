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

#include "SettingsDialog.h"
#include "EditProfiles.h"
#include "touchecore.h"
#include "ModulesPage.h"
#include <KPageWidgetItem>
#include <KIconLoader>

SettingsDialog::SettingsDialog(ToucheCore *core, QWidget *parent) :
    KPageDialog(parent)
{
    setCaption(i18n("Touché Settings"));
    QSettings *settings = Touche::settings(this);
    editProfiles = new EditProfiles(core, settings, this);
    modulesPage = new ModulesPage(settings, this);

    KPageWidgetItem *editProfilesWidgetItem = new KPageWidgetItem(editProfiles ,i18n("Profiles"));
    editProfilesWidgetItem->setIcon(KIcon("tab-duplicate", KIconLoader::global()));
    KPageWidgetItem *modulesPageWidgetItem = new KPageWidgetItem(modulesPage ,i18n("Modules"));
    modulesPageWidgetItem->setIcon(KIcon("preferences-plugin", KIconLoader::global()));

    addPage(editProfilesWidgetItem);
    addPage(modulesPageWidgetItem);
}


void SettingsDialog::accept()
{
    editProfiles->accept();
    modulesPage->accept();
    KPageDialog::accept();
}

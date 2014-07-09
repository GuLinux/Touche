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

#include "ModulesPage.h"
#include "ui_ModulesPage.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QStandardItem>
#include <QSettings>

#define ConfigNameRole Qt::UserRole+1

ModulesPage::ModulesPage(QSettings *settings, QWidget *parent) :
    QWidget(parent),
    settings(settings),
    ui(new Ui::ModulesPage)
{
    model = new QStandardItemModel(this);
    ui->setupUi(this);
    ui->modulesList->setModel(model);
#ifdef HAVE_CWIID
    QStandardItem *wiimoteItem = new QStandardItem(QIcon::fromTheme("wiimote"), "Wiimote");
    wiimoteItem->setCheckable(true);
    wiimoteItem->setCheckState(settings->value("wiimote_enabled", false).toBool() ? Qt::Checked : Qt::Unchecked);
    wiimoteItem->setData("wiimote_enabled", ConfigNameRole);
    model->appendRow(wiimoteItem);
#endif //HAVE_CWIID
}

ModulesPage::~ModulesPage()
{
    delete ui;
}


void ModulesPage::accept()
{
    for(int row=0; row<model->rowCount(); row++) {
        QStandardItem *item = model->item(row);
        settings->setValue(item->data(ConfigNameRole).toString(), item->checkState() == Qt::Checked);
        settings->sync();
    }
}


void ModulesPage::modelDataChanged()
{
    qDebug() << "modules model data changed";
}

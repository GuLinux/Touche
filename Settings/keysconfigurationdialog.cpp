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

#include "keysconfigurationdialog.h"
#include "ui_keysconfigurationdialog.h"
#include "backend/config/databaseentry.h"
#include "domain/configevent.h"
#include <QDebug>
#include <QStringListModel>
#include "models/cfgdevice.h"
#include "keybindingconfiguration.h"
#include <QSettings>
#include <KLocale>

KeysConfigurationDialog::KeysConfigurationDialog(DeviceInfo *deviceInfo, const QString &profile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeysConfigurationDialog), deviceInfo(deviceInfo)
{
    ui->setupUi(this);
    ui->device_name_label->setText(i18nc("label for dialog conf; %1 is the device name, %2 profile name",
                                          "Configuration for %1, current profile: %2")
        .arg(deviceInfo->name())
        .arg(profile));

    CfgDevice *device = new CfgDevice(deviceInfo, this);

    cfgKeyListModel = new CfgKeyListModel(device->cfgKeys(), this);
    ui->keys_listview->setModel(cfgKeyListModel);

    connect(ui->keys_listview, SIGNAL(activated(QModelIndex)), this, SLOT(cfgKeySelected(QModelIndex)));
    connect(ui->keys_listview, SIGNAL(clicked(QModelIndex)), this, SLOT(cfgKeySelected(QModelIndex)));
    connect(ui->keys_listview, SIGNAL(pressed(QModelIndex)), this, SLOT(cfgKeySelected(QModelIndex)));
    connect(ui->keys_listview, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(cfgKeySelected(QModelIndex)));

    settings = new QSettings("GuLinux", qAppName(), this);
    qDebug() << "avail groups: " << settings->childGroups();
    settings->beginGroup(QString("bindings_%1").arg(profile));
}

KeysConfigurationDialog::~KeysConfigurationDialog()
{
    delete ui;
}

void KeysConfigurationDialog::cfgKeySelected(const QModelIndex &index)
{
    CfgKey *cfgKey = cfgKeyListModel->item(index);
    if(!configuration_dialogs.contains(cfgKey)) {
        KeyBindingConfiguration *bindingConfig = new KeyBindingConfiguration(cfgKey, settings, this);
        configuration_dialogs.insert(cfgKey, bindingConfig);
        ui->action_settings_stack->addWidget(bindingConfig);
    }
    ui->action_settings_stack->setCurrentWidget(configuration_dialogs.value(cfgKey));
}

void KeysConfigurationDialog::accept()
{
    foreach(KeyBindingConfiguration *keyBindingConfiguration, configuration_dialogs.values()) {
        keyBindingConfiguration->saveConfig();
    }
    settings->sync();
    QDialog::accept();
}

void KeysConfigurationDialog::keyEvent(const QString &keyName)
{
    qDebug() << "keyEvent: " << keyName;
    QModelIndex index = cfgKeyListModel->findKey(keyName);
    ui->keys_listview->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    cfgKeySelected(index);
}


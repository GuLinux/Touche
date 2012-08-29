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

#include "keybindingconfiguration.h"
#include "models/cfgkey.h"
#include "ui_keybindingconfiguration.h"
#include <QSettings>
#include "ui_RunCommand_cfg.h"
#include "bindingconfigurationwidget.h"
#include "bindingsGui/ToKeyConfig.h"
#include "bindingsGui/RunCommandConfig.h"
#include <KLocale>

KeyBindingConfiguration::KeyBindingConfiguration(CfgKey *cfgKey, QSettings *settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBindingConfiguration), m_cfgKey(cfgKey), settings(settings)
{
    widgetsFactories.insert("TranslateToKey", new ToKeyConfigFactory());
    widgetsFactories.insert("RunCommand", new RunCommandConfigFactory());

    ui->setupUi(this);
    connect(ui->bindingTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bindingChanged(int)));

    ui->bindingTypelabel->setText(i18nc("label for associated event; %1 is keyevent name", "Associated Event for Key %1:").arg(cfgKey->keyName()) );
    ui->bindingTypeComboBox->addItem(i18nc("Do Nothing Action", "DoNothing"), "DoNothing");
    ui->bindingTypeComboBox->addItem(i18nc("Translate to Key Action", "TranslateToKey"), "TranslateToKey");
    ui->bindingTypeComboBox->addItem(i18nc("Run Command Action", "RunCommand"), "RunCommand");
    kDebug() << "ConfigKey: " << cfgKey->cfgKeyEvents().first()->configKey();
    ui->bindingTypeComboBox->setCurrentIndex(ui->bindingTypeComboBox->findData(settings->value(cfgKey->cfgKeyEvents().first()->configKey(), "DoNothing")));
}

KeyBindingConfiguration::~KeyBindingConfiguration()
{
    foreach(BindingConfigurationWidgetFactory *factory, widgetsFactories.values())
        delete factory;
    delete ui;
}

void KeyBindingConfiguration::bindingChanged(int index)
{
    QString configBindingKey = ui->bindingTypeComboBox->itemData(index).toString();
    foreach(const QString eventName, tabsWidgets.keys()) {
        BindingConfigurationWidget *widget = tabsWidgets.take(eventName);
        ui->bindingEvents->removeTab(ui->bindingEvents->indexOf(widget));
        delete widget;
    }

    if(configBindingKey == "DoNothing") return;
    kDebug() << "configBindingKey: " << configBindingKey;
    SiblingsList *siblingsList = new SiblingsList(this);
    foreach(CfgKeyEvent* event, m_cfgKey->cfgKeyEvents()) {
        BindingConfigurationWidget *widget = widgetsFactories.value(configBindingKey)
                ->build(settings, configBindingKey, event, siblingsList, this);
        siblingsList->addSibling(widget);
        tabsWidgets.insert(event->label(), widget);
        ui->bindingEvents->addTab(widget, event->label());
    }
}

void KeyBindingConfiguration::saveConfig()
{
    settings->remove(m_cfgKey->label());
    if(tabsWidgets.size()==0) return;
    settings->setValue(m_cfgKey->label(), QStringList(tabsWidgets.keys()));
    foreach(BindingConfigurationWidget *widget, tabsWidgets.values()) {
        widget->saveConfiguration();
    }
}

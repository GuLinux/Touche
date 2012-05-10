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
#include "ui_ToKey_cfg.h"
#include "bindingconfigurationwidget.h"
#include "x11keysymbolscompleter.h"

KeyBindingConfiguration::KeyBindingConfiguration(CfgKey *cfgKey, QSettings *settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBindingConfiguration), m_cfgKey(cfgKey), settings(settings)
{
    ui->setupUi(this);
    connect(ui->bindingTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bindingChanged(int)));

    ui->bindingTypelabel->setText(tr("Associated Event for Key %1:", "label for associated event; %1 is keyevent name").arg(cfgKey->keyName()) );
    ui->bindingTypeComboBox->addItem(tr("DoNothing", "Do Nothing Action"), "DoNothing");
    ui->bindingTypeComboBox->addItem(tr("TranslateToKey", "Translate to Key Action"), "TranslateToKey");
    ui->bindingTypeComboBox->addItem(tr("RunCommand", "Run Command Action"), "RunCommand");
    ui->bindingTypeComboBox->setCurrentIndex(ui->bindingTypeComboBox->findData(settings->value(cfgKey->cfgKeyEvents().first()->configKey(), "DoNothing")));
}

KeyBindingConfiguration::~KeyBindingConfiguration()
{
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

    foreach(CfgKeyEvent* event, m_cfgKey->cfgKeyEvents()) {

        BindingConfigurationWidget *widget =0;
        QString bindingSettingsKey = QString("%1/%2/%3").arg(event->configKey(), configBindingKey);

        if(configBindingKey=="TranslateToKey") {
            Ui_ToKeyConfig *cfgUI = new Ui_ToKeyConfig();
            BindUI bindUI = [cfgUI,this,bindingSettingsKey,event](BindingConfigurationWidget* widget){ cfgUI->setupUi(widget);
                connect(cfgUI->cfg_eventtype_keypress, SIGNAL(toggled(bool)), widget, SLOT(radioButtonChanged(bool)));
                connect(cfgUI->cfg_eventtype_keyrelease, SIGNAL(toggled(bool)), widget, SLOT(radioButtonChanged(bool)));
                connect(cfgUI->cfg_keysymbol, SIGNAL(textChanged(QString)), widget, SLOT(stringChanged(QString)));
                cfgUI->cfg_keysymbol->setCompleter(new X11KeySymbolsCompleter(widget));
                foreach(const QString keySym, X11KeySymbolsCompleter::keySymbols()) {
                    cfgUI->cfg_keysymbol->addItem(keySym, keySym);
                }

                QString eventtype = settings->value(bindingSettingsKey.arg("eventtype"), event->label()).toString();
                cfgUI->cfg_keysymbol->lineEdit()->setText(settings->value(bindingSettingsKey.arg("keysymbol")).toString());
                cfgUI->cfg_eventtype_keypress->setChecked(eventtype=="keypress");
                cfgUI->cfg_eventtype_keyrelease->setChecked(eventtype=="keyrelease");
            };
            DeleteUI deleteUI = [cfgUI]() { delete cfgUI; };
            widget=new BindingConfigurationWidget(settings, configBindingKey, event->configKey(), bindUI, deleteUI, this);
        }

        if(configBindingKey=="RunCommand") {
            Ui_RunCommandConfig *cfgUI = new Ui_RunCommandConfig();
            BindUI bindUI = [cfgUI,this,bindingSettingsKey](BindingConfigurationWidget* widget){ cfgUI->setupUi(widget);
                connect(cfgUI->cfg_Arguments, SIGNAL(stringListChanged(QStringList)), widget, SLOT(stringListChanged(QStringList)));
                connect(cfgUI->cfg_ApplicationName, SIGNAL(textChanged(QString)), widget, SLOT(stringChanged(QString)));
                cfgUI->cfg_Arguments->setStringList(settings->value(bindingSettingsKey.arg("Arguments")).toStringList());
                cfgUI->cfg_ApplicationName->setText(settings->value(bindingSettingsKey.arg("ApplicationName")).toString());
            };
            DeleteUI deleteUI = [cfgUI]() { delete cfgUI; };
            widget=new BindingConfigurationWidget(settings, configBindingKey, event->configKey(), bindUI, deleteUI, this);
        }
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

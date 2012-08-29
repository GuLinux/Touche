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


#include "ToKeyConfig.h"
#include "ui_ToKey_cfg.h"
#include "bindingsGui/x11keysymbolscompleter.h"
#include "models/cfgkeyevent.h"
#include <QSettings>
#include <QLineEdit>

BindingConfigurationWidget *ToKeyConfigFactory::build(QSettings *settings, const QString &bindingType, CfgKeyEvent *event, SiblingsList *siblingsList, QWidget *parent)
{
    return new ToKeyConfig(settings, bindingType, event, siblingsList, parent);
}


ToKeyConfig::~ToKeyConfig()
{
    delete ui;
}


ToKeyConfig::ToKeyConfig(QSettings *settings, const QString &bindingType, CfgKeyEvent *event, SiblingsList *siblingsList, QWidget *parent)
    : BindingConfigurationWidget(settings, bindingType, event->configKey(), parent),
      siblingsList(siblingsList)
{
    ui = new Ui_ToKeyConfig();
    const QString m_bindingSettingsKey = BindingConfigurationWidgetFactory::bindingSettingsKey(event, bindingType);
    ui->setupUi(this);
    connect(ui->cfg_eventtype_keypress, SIGNAL(toggled(bool)), this, SLOT(radioButtonChanged(bool)));
    connect(ui->cfg_eventtype_keyrelease, SIGNAL(toggled(bool)), this, SLOT(radioButtonChanged(bool)));
    connect(ui->cfg_keysymbol, SIGNAL(textChanged(QString)), this, SLOT(stringChanged(QString)));
    ui->cfg_keysymbol->setCompleter(new X11KeySymbolsCompleter(this));
    foreach(const QString keySym, X11KeySymbolsCompleter::keySymbols()) {
        ui->cfg_keysymbol->addItem(keySym, keySym);
    }

    connect(ui->cfg_keysymbol, SIGNAL(editTextChanged(QString)), this, SLOT(updateSiblings(QString)));
    QString eventtype = settings->value(m_bindingSettingsKey.arg("eventtype"), event->label()).toString();
    keySymbol = settings->value(m_bindingSettingsKey.arg("keysymbol")).toString();
    ui->cfg_keysymbol->lineEdit()->setText(keySymbol);
    ui->cfg_eventtype_keypress->setChecked(eventtype=="keypress");
    ui->cfg_eventtype_keyrelease->setChecked(eventtype=="keyrelease");
}


void ToKeyConfig::updateSiblings(const QString &text)
{
    foreach(BindingConfigurationWidget *g_sibling, siblingsList->siblings(this)) {
        ToKeyConfig *sibling = (ToKeyConfig*) g_sibling;
        QString siblingKeySymbol = sibling->ui->cfg_keysymbol->lineEdit()->text();
        if((siblingKeySymbol.isEmpty() || siblingKeySymbol == keySymbol) && siblingKeySymbol != text)
            sibling->ui->cfg_keysymbol->lineEdit()->setText(text);
    }
    keySymbol = text;
}

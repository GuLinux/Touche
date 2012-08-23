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

#include "RunCommandConfig.h"
#include "ui_RunCommand_cfg.h"
#include <QSettings>
#include "models/cfgkeyevent.h"


BindingConfigurationWidget *RunCommandConfigFactory::build(QSettings *settings, const QString &bindingType, CfgKeyEvent *event, SiblingsList *, QWidget *parent)
{
    return new RunCommandConfig(settings, bindingType, event, parent);
}


RunCommandConfig::RunCommandConfig(QSettings *settings, const QString &bindingType, CfgKeyEvent *event, QWidget *parent)
    : BindingConfigurationWidget(settings, bindingType, event->configKey(), parent)
{
    ui = new Ui_RunCommandConfig();
    const QString m_bindingSettingsKey = BindingConfigurationWidgetFactory::bindingSettingsKey(event, bindingType);

    ui->setupUi(this);
    connect(ui->cfg_Arguments, SIGNAL(stringListChanged(QStringList)), this, SLOT(stringListChanged(QStringList)));
    connect(ui->cfg_ApplicationName, SIGNAL(textChanged(QString)), this, SLOT(stringChanged(QString)));
    ui->cfg_Arguments->setStringList(settings->value(m_bindingSettingsKey.arg("Arguments")).toStringList());
    ui->cfg_ApplicationName->setText(settings->value(m_bindingSettingsKey.arg("ApplicationName")).toString());

}


RunCommandConfig::~RunCommandConfig()
{
    delete ui;
}

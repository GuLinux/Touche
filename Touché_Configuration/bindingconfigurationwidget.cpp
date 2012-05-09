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

#include "bindingconfigurationwidget.h"
#include <QDebug>
#include <QSettings>

BindingConfigurationWidget::BindingConfigurationWidget(QSettings *settings, const QString &bindingType, const QString &configurationName, const BindUI &bindUI, const DeleteUI &deleteUI, QWidget *parent) :
    QWidget(parent), settings(settings), deleteUI(deleteUI), configurationName(configurationName), configurationSubKey(QString(configurationName).append("/%1/%2").arg(bindingType))
{
    configuration.insert(configurationName, bindingType);
    bindUI(this);
}

BindingConfigurationWidget::~BindingConfigurationWidget()
{
    deleteUI();
}

void BindingConfigurationWidget::stringListChanged(const QStringList &stringList)
{
    QString configItemName = senderConfigName();
    if(configItemName.isEmpty()) return;
    configuration.insert(configurationSubKey.arg(configItemName), stringList);
}

void BindingConfigurationWidget::stringChanged(const QString &string)
{

    QString configItemName = senderConfigName();
    if(configItemName.isEmpty()) return;
    configuration.insert(configurationSubKey.arg(configItemName), string);
}

void BindingConfigurationWidget::radioButtonChanged(bool enabled)
{
    QString configItemName = senderConfigName();
    if(configItemName.isEmpty() || !enabled) return;
    QStringList configEntry = configItemName.split("_");

    configuration.insert(configurationSubKey.arg(configEntry.first()), configEntry.last());
}

QString BindingConfigurationWidget::senderConfigName()
{
    QString configItemName = sender()->property("objectName").toString();
    if(!configItemName.startsWith("cfg_")) return QString();
    configItemName = configItemName.replace("cfg_", "");
    return configItemName;
}

void BindingConfigurationWidget::saveConfiguration()
{
    foreach(const QString &key, configuration.keys()) {
        QVariant value = configuration.value(key);
        switch(value.type()) {
        case QVariant::String:
            settings->setValue(key, value.toString());
            break;
        case QVariant::StringList:
            settings->setValue(key, value.toStringList());
            break;
        default:
            settings->setValue(key, value);
        }
    }
}

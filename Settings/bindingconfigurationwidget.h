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

#ifndef BINDINGCONFIGURATIONWIDGET_H
#define BINDINGCONFIGURATIONWIDGET_H

#include <QWidget>
#include "functional"
#include <QHash>
#include <QVariant>
class QSettings;
class BindingConfigurationWidget;

typedef std::function<void(BindingConfigurationWidget*)> BindUI;
typedef std::function<void()> DeleteUI;

class BindingConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BindingConfigurationWidget(QSettings *settings, const QString &bindingType, const QString &configurationName, const BindUI &bindUI, const DeleteUI &deleteUI, QWidget *parent=0);
    ~BindingConfigurationWidget();
signals:
    
public slots:
    void stringChanged(const QString &string);
    void stringListChanged(const QStringList &stringList);
    void radioButtonChanged(bool enabled);
    void saveConfiguration();
private:
    QSettings *settings;
    DeleteUI deleteUI;
    QString const configurationName;
    QString const configurationSubKey;
    QHash<QString, QVariant> configuration;
    QString senderConfigName();
};

#endif // BINDINGCONFIGURATIONWIDGET_H

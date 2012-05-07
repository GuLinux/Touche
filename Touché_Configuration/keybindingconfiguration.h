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

#ifndef KEYBINDINGCONFIGURATION_H
#define KEYBINDINGCONFIGURATION_H
#include <QHash>
#include <QWidget>
class CfgKey;
class BindingConfigurationWidget;
class QSettings;
namespace Ui {
class KeyBindingConfiguration;
}

class KeyBindingConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit KeyBindingConfiguration(CfgKey *cfgKey, QSettings *settings, QWidget *parent = 0);
    ~KeyBindingConfiguration();
    
public slots:
    void bindingChanged(int index);
    void saveConfig();
private:
    Ui::KeyBindingConfiguration *ui;
    CfgKey *m_cfgKey;
    QSettings *settings;
    QHash<QString, BindingConfigurationWidget*> tabsWidgets;
};

#endif // KEYBINDINGCONFIGURATION_H

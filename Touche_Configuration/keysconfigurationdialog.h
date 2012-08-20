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

#ifndef KEYSCONFIGURATIONDIALOG_H
#define KEYSCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QHash>
class CfgKeyListModel;
class CfgKey;
class CfgKeyEventListModel;
class DeviceInfo;
class QSettings;
class KeyBindingConfiguration;
namespace Ui {
class KeysConfigurationDialog;
}

class KeysConfigurationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit KeysConfigurationDialog(DeviceInfo *deviceInfo, QWidget *parent = 0);
    ~KeysConfigurationDialog();
    
public slots:
    void cfgKeySelected(const QModelIndex & index );
    void accept();
    void keyEvent(const QString &keyName);

private:
    Ui::KeysConfigurationDialog *ui;
    DeviceInfo *deviceInfo;
    QHash<CfgKey*, KeyBindingConfiguration*> configuration_dialogs;
    CfgKeyListModel *cfgKeyListModel;
    QSettings *settings;
};

#endif // KEYSCONFIGURATIONDIALOG_H

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

#ifndef QSTRINGLISTEDIT_H
#define QSTRINGLISTEDIT_H

#include <QWidget>
#include <QModelIndex>

class QStringListModel;
namespace Ui {
class QStringListEdit;
}

class QStringListEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit QStringListEdit(QWidget *parent = 0, bool disableDelete = false);
    ~QStringListEdit();
    QStringList stringList() const;
    void setStringList(const QStringList &stringList);
private slots:
    void on_qsle_delete_clicked();
    void on_qsle_add_clicked();
    void on_qsle_lineedit_textChanged(const QString &arg1);
    void itemSelected(const QModelIndex &index);
signals:
    void stringListChanged(const QStringList &stringList);
    void stringListEdited(const QStringList &stringList);
private:
    Ui::QStringListEdit *ui;
    QStringListModel *model;
};

#endif // QSTRINGLISTEDIT_H

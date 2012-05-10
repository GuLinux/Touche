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

#include "qstringlistedit.h"
#include "ui_qstringlistedit.h"

#include <QStringListModel>
#include <QDebug>

QStringListEdit::QStringListEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QStringListEdit), model(new QStringListModel(this))
{
    ui->setupUi(this);
    ui->qsle_listview->setModel(model);
    connect(ui->qsle_listview, SIGNAL(activated(QModelIndex)), SLOT(itemSelected(QModelIndex)));
    connect(ui->qsle_listview, SIGNAL(clicked(QModelIndex)), SLOT(itemSelected(QModelIndex)));
    connect(ui->qsle_listview, SIGNAL(doubleClicked(QModelIndex)), SLOT(itemSelected(QModelIndex)));
    connect(ui->qsle_listview, SIGNAL(pressed(QModelIndex)), SLOT(itemSelected(QModelIndex)));
}

QStringListEdit::~QStringListEdit()
{
    delete ui;
}


void QStringListEdit::setStringList(const QStringList &stringList) {
    model->setStringList(stringList);
    emit stringListChanged(stringList);
}



void QStringListEdit::on_qsle_delete_clicked()
{
    qDebug() << ui->qsle_listview->currentIndex();
    QStringList stringList = this->stringList();
    stringList.removeAt(ui->qsle_listview->currentIndex().row());
    setStringList(stringList);
    emit stringListEdited(this->stringList());
}

void QStringListEdit::on_qsle_add_clicked()
{
    if(ui->qsle_lineedit->text().isEmpty()) return;
    setStringList(model->stringList() << ui->qsle_lineedit->text());
    ui->qsle_lineedit->setText(QString());
    emit stringListEdited(stringList());
}

void QStringListEdit::on_qsle_lineedit_textChanged(const QString &arg1)
{
    ui->qsle_add->setEnabled(!arg1.isEmpty());
}


void QStringListEdit::itemSelected(const QModelIndex &index) {
    Q_UNUSED(index);
    ui->qsle_delete->setEnabled(ui->qsle_listview->currentIndex().isValid());
}

QStringList QStringListEdit::stringList() const
{
    return model->stringList();
}


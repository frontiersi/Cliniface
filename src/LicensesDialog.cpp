/************************************************************************
 * Copyright (C) 2017 Richard Palmer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <LicensesDialog.h>
#include <ui_LicensesDialog.h>
using Cliniface::LicensesDialog;


LicensesDialog::LicensesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LicensesDialog)
{
    ui->setupUi(this);
    this->setModal(false);
    connect( ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
    ui->textBrowser->setOpenExternalLinks(true);
}   // end ctor


LicensesDialog::~LicensesDialog()
{
    delete ui;
}   // end dtor

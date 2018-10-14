/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <HelpDialog.h>
#include <ui_HelpDialog.h>
using Cliniface::HelpDialog;


HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    this->setModal(false);
    connect( ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
}   // end ctor


HelpDialog::~HelpDialog()
{
    delete ui;
}   // end dtor

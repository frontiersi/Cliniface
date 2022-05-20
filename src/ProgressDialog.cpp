/************************************************************************
 * Copyright (C) 2022 SIS Research Ltd & Richard Palmer
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

#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
using Cliniface::ProgressDialog;


ProgressDialog::ProgressDialog( QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint), _ui(new Ui::ProgressDialog), _nfiles(0)
{
    _ui->setupUi(this);
    setModal(true);
    setFixedSize( geometry().width(), geometry().height());
    connect( _ui->buttonBox, &QDialogButtonBox::rejected, this, &ProgressDialog::onCancelled);
}   // end ctor


ProgressDialog::~ProgressDialog() { delete _ui;}


void ProgressDialog::reset( size_t n)
{
    _nfiles = n;
    const QString m0 = tr("Parsing metadata from %1 files.").arg(n);
    static const QString m1 = tr("Please wait...");
    _ui->label->setText( QString("<html><head/><body><p>%1</p><p>%2</p></body></html>").arg(m0,m1));
    _ui->progressBar->setValue(0);
    _ui->buttonBox->setStandardButtons( QDialogButtonBox::Cancel);
}   // end reset


void ProgressDialog::doOnPercentProgress( float v)
{
    _ui->progressBar->setValue( int(v));
    if ( v >= 100.0f)
    {
        _ui->label->setText( tr("<p>Imported metadata from %1 files.</p>").arg(_nfiles));
        _ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok);
    }   // end if
}   // end doOnPercentProgress


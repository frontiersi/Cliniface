/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#include <UpdatesDialog.h>
#include <ui_UpdatesDialog.h>
#include <Cliniface_Config.h>
#include <FaceTools/Action/FaceActionManager.h>
using Cliniface::UpdatesDialog;
using FaceTools::Action::ActionNewerVersionFinder;

UpdatesDialog::UpdatesDialog( QWidget *parent) :
    QDialog(parent),
    _ui( new Ui::UpdatesDialog),
    _finder( new ActionNewerVersionFinder( "Check for updates", QIcon(":/icons/UPGRADE")))
{
    _ui->setupUi(this);
    setWindowModality( Qt::NonModal);
    _finder->setManifestUrl( QUrl( APP_MANIFEST_URL));
    QTools::VersionInfo cvers;
    cvers.setMajor( APP_VERSION_MAJOR);
    cvers.setMinor( APP_VERSION_MINOR);
    cvers.setPatch( APP_VERSION_PATCH);
    _finder->setCurrentVersion( cvers);
    connect( _finder, &ActionNewerVersionFinder::onFoundNewerVersion,
                this, &UpdatesDialog::_doOnFoundNewerVersion);
    FAM::registerAction( _finder);

    connect( _ui->updateButton, &QPushButton::pressed,
             this, &UpdatesDialog::_doOnUpdateButtonPushed);

    connect( _ui->donateButton, &QPushButton::pressed,
             this, &UpdatesDialog::_doOnDonateButtonPushed);
}   // end ctor


UpdatesDialog::~UpdatesDialog()
{
    delete _finder;
    delete _ui;
}   // end dtor


void UpdatesDialog::_doOnFoundNewerVersion( bool found)
{
    const QTools::VersionInfo &v = _finder->updatedVersion();
    _ui->updateDetails->setMarkdown( v.details());
    _ui->downloadProgressBar->setValue(0);
    _ui->downloadProgressBar->setEnabled(false);
    _ui->updateButton->setEnabled( found);
    if ( found)
        this->show();
}   // end _doOnFoundNewerVersion


void UpdatesDialog::_doOnUpdateButtonPushed()
{
    _ui->downloadProgressBar->setEnabled(true);
    std::cerr << "Clicked update" << std::endl;
}   // end _doOnUpdateButtonPushed


void UpdatesDialog::_doOnDonateButtonPushed()
{
    std::cerr << "Clicked donate" << std::endl;
}   // end _doOnDonateButtonPushed

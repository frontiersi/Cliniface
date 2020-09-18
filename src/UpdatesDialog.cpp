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
#include <AppUpdater.h>
#include <FaceTools/MiscFunctions.h>
#include <FaceTools/Action/FaceActionManager.h>
#include <QDesktopServices>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QDir>
using Cliniface::UpdatesDialog;
using FaceTools::Action::ActionFindUpdate;

// static definitions
bool UpdatesDialog::s_checkUpdateAtStart(false);
void UpdatesDialog::setCheckUpdateAtStart( bool v) { s_checkUpdateAtStart = v;}
bool UpdatesDialog::checkUpdateAtStart() { return s_checkUpdateAtStart;}


UpdatesDialog::UpdatesDialog( QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint),
    _ui( new Ui::UpdatesDialog),
    _finder( new ActionFindUpdate( "Check for updates", QIcon(":/icons/UPGRADE"), QUrl( APP_MANIFEST_URL))),
    _updateFile(nullptr)
{
    _ui->setupUi(this);
    setWindowModality( Qt::NonModal);
    AppUpdater::recordAppExe();
#ifdef __linux__    // Disallow updates if this is Linux and we're not running the AppImage version
    _finder->setLocked( !AppUpdater::isAppImage());
#endif

    QTools::UpdateMeta cmeta;
    cmeta.setMajor( APP_VERSION_MAJOR);
    cmeta.setMinor( APP_VERSION_MINOR);
    cmeta.setPatch( APP_VERSION_PATCH);
    _finder->setCurrentMeta( cmeta);
    connect( _finder, &ActionFindUpdate::onFoundUpdate, this, &UpdatesDialog::_doOnFoundUpdate);
    connect( _finder, &ActionFindUpdate::onDownloadProgress, this, &UpdatesDialog::_doOnDownloadProgress);
    connect( _finder, &ActionFindUpdate::onUpdateDownloaded, this, &UpdatesDialog::_doOnUpdateDownloaded);
    connect( _finder, &ActionFindUpdate::onError, this, &UpdatesDialog::_doOnError);
    FAM::registerAction( _finder);

    connect( _ui->updateButton, &QPushButton::pressed, this, &UpdatesDialog::_doOnUpdateButtonPushed);
    connect( _ui->donateButton, &QPushButton::pressed, [](){ QDesktopServices::openUrl( QUrl( APP_DONATE_URL));});

    _ui->updateButton->setEnabled( false);
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), QGuiApplication::primaryScreen()->geometry()));
}   // end ctor


UpdatesDialog::~UpdatesDialog()
{
    _deleteDownloadedUpdateFile();
    delete _finder;
    delete _ui;
}   // end dtor


void UpdatesDialog::reject()
{
    if ( _finder->isEnabled() && _updateFile == nullptr)
        QDialog::reject();
}   // end reject


void UpdatesDialog::closeEvent( QCloseEvent *evt)   // Ignore close events if still working
{
    if ( !_finder->isEnabled() || _updateFile != nullptr)
        evt->ignore();
}   // end closeEvent


QSize UpdatesDialog::sizeHint() const { return QSize(455,355);}


void UpdatesDialog::_doOnFoundUpdate( bool found)
{
    const int cmj = APP_VERSION_MAJOR;
    const int cmn = APP_VERSION_MINOR;
    const int cpt = APP_VERSION_PATCH;
    const int mj = _finder->updateMeta().major();
    const int mn = _finder->updateMeta().minor();
    const int pt = _finder->updateMeta().patch();
    static QString NO_UPDATE_MSG = QString( "**You already have the latest version %1.%2.%3.**\\").arg(mj).arg(mn).arg(pt);
    static QString UPDATE_MSG = QString( "**An update to version %1.%2.%3 is available! You have version %4.%5.%6.**\\").arg(mj).arg(mn).arg(pt).arg(cmj).arg(cmn).arg(cpt) +
                                FaceTools::loadTextFromFile(":/data/UPDATE_MSG");
    static QString SUPPORT_MSG = FaceTools::loadTextFromFile(":/data/SUPPORT_MSG");

    _ui->updateButton->setEnabled( found);
    _ui->downloadProgressBar->setEnabled( found);
    _ui->downloadProgressBar->setRange( 0, 1);
    _ui->downloadProgressBar->setValue( 1);

    if ( found)
    {
        _ui->updateDetails->setMarkdown( UPDATE_MSG + SUPPORT_MSG + _finder->updateMeta().details());
        _ui->downloadProgressBar->setFormat( "Ready to download");
    }   // end found
    else
    {
        _ui->updateDetails->setMarkdown( NO_UPDATE_MSG + SUPPORT_MSG);
        _ui->downloadProgressBar->setFormat( "No updated version available");
    }   // end else

    adjustSize();

    _finder->refresh( FaceTools::Action::Event::USER);
    if ( found || !checkUpdateAtStart())
        this->show();

    setCheckUpdateAtStart( false);
}   // end _doOnFoundUpdate


void UpdatesDialog::_doOnUpdateButtonPushed()
{
    _ui->downloadProgressBar->setFormat( "Starting download...");
    _ui->updateButton->setEnabled( false);
    _ui->buttonBox->setEnabled( false);

    _updateFile = new QTemporaryFile( this);
    if ( _updateFile->open())
        _finder->downloadUpdate( _updateFile->fileName());
    else
        _doOnError( "Unable to open temporary file!");
}   // end _doOnUpdateButtonPushed


void UpdatesDialog::_doOnDownloadProgress( qint64 br, qint64 bt)
{
    if ( bt >= 0)
    {
        _ui->downloadProgressBar->setFormat( "Downloaded %p%");
        _ui->downloadProgressBar->setMaximum( static_cast<int>(bt));
        _ui->downloadProgressBar->setValue( static_cast<int>(br));
    }   // end if
    else
    {
        const int kbs = static_cast<int>( br / 1024);
        _ui->downloadProgressBar->setFormat( "Downloaded %v kBs");
        _ui->downloadProgressBar->setRange( 0, kbs);
        _ui->downloadProgressBar->setValue( kbs);
    }   // end else
}   // end _doOnDownloadProgress


void UpdatesDialog::_doOnUpdateDownloaded()
{
    _ui->downloadProgressBar->setFormat( "Updating - please wait...");
    // Run the update in a separate thread
    AppUpdater *updater = new AppUpdater( _updateFile->fileName());
    connect( updater, &AppUpdater::onFinished, this, &UpdatesDialog::_doOnFinishedUpdate);
    connect( updater, &AppUpdater::finished, updater, &QObject::deleteLater);
    updater->start();
}   // end _doOnUpdateDownloaded


void UpdatesDialog::_doOnFinishedUpdate( QString err)
{
    if ( !err.isEmpty())
        _doOnError( err);
    else
    {
        _finder->setCurrentMeta(_finder->updateMeta());
        _ui->downloadProgressBar->setFormat( "Update Complete!");
        QMessageBox::information( this, tr("Update Complete!"), tr("Restart Cliniface to begin using the new version."));
    }   // end else
    _ui->buttonBox->setEnabled(true);
    _deleteDownloadedUpdateFile();
    _finder->refresh( FaceTools::Action::Event::USER);
    this->close();
}   // end _doOnFinishedUpdate


void UpdatesDialog::_doOnError( const QString &err)
{
    _ui->updateButton->setEnabled( false);
    _ui->downloadProgressBar->setFormat( "Update Error!");
    QMessageBox::warning( this, tr("Update Error!"), tr(err.toStdString().c_str()));
    _deleteDownloadedUpdateFile();
    _finder->refresh( FaceTools::Action::Event::USER);
    this->close();
}   // end _doOnError


void UpdatesDialog::_deleteDownloadedUpdateFile()
{
    if ( _updateFile)
        delete _updateFile;
    _updateFile = nullptr;
}   // end _deleteDownloadedUpdateFile

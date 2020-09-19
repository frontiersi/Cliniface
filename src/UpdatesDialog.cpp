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
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QDir>
using Cliniface::UpdatesDialog;
using QMB = QMessageBox;

// static definitions
bool UpdatesDialog::s_checkUpdateAtStart(false);
void UpdatesDialog::setCheckUpdateAtStart( bool v) { s_checkUpdateAtStart = v;}
bool UpdatesDialog::checkUpdateAtStart() { return s_checkUpdateAtStart;}


UpdatesDialog::UpdatesDialog( QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint),
    _ui( new Ui::UpdatesDialog),
    _nupdater( QUrl( APP_MANIFEST_URL), 10000, 5),
    _updateFile(nullptr),
    _allowChecking(true),
    _gettingManifest(false)
{
    _ui->setupUi(this);
    setWindowModality( Qt::NonModal);

    _cmeta.setMajor( APP_VERSION_MAJOR);
    _cmeta.setMinor( APP_VERSION_MINOR);
    _cmeta.setPatch( APP_VERSION_PATCH);

    connect( &_nupdater, &QTools::NetworkUpdater::onReplyFinished, this, &UpdatesDialog::_doOnReplyFinished);
    connect( &_nupdater, &QTools::NetworkUpdater::onDownloadProgress, this, &UpdatesDialog::_doOnDownloadProgress);

    AppUpdater::recordAppExe();
#ifdef __linux__    // On Linux, only allow updates for the AppImage version
    _allowChecking = AppUpdater::isAppImage();
#endif

    connect( _ui->updateButton, &QPushButton::pressed, this, &UpdatesDialog::_doOnUpdateButtonPushed);
    connect( _ui->donateButton, &QPushButton::pressed, [](){ QDesktopServices::openUrl( QUrl( APP_DONATE_URL));});

    _ui->updateButton->setEnabled( false);
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), QGuiApplication::primaryScreen()->geometry()));
}   // end ctor


UpdatesDialog::~UpdatesDialog()
{
    _deleteDownloadedUpdateFile();
    delete _ui;
}   // end dtor


void UpdatesDialog::checkForUpdate()
{
    _ui->updateButton->setEnabled( false);
    _ui->downloadProgressBar->setEnabled( false);
    _ui->downloadProgressBar->setRange( 0, 1);
    _ui->downloadProgressBar->setValue( 1);
    _ui->updateDetails->setMarkdown( tr("**Checking for update...**"));;
    _ui->downloadProgressBar->setFormat( "Checking for update...");
    std::cerr << "Refreshing manifest from " << _nupdater.manifestUrl().toDisplayString().toStdString() << std::endl;
    _gettingManifest = true;
    _nupdater.refreshManifest();
}   // end checkForUpdate


void UpdatesDialog::open()
{
    checkForUpdate();
    QDialog::open();
}   // end open


void UpdatesDialog::reject()    // Allow dialog close if not busy updating
{
    if ( !_nupdater.isBusy() && _updateFile == nullptr)
        QDialog::reject();
}   // end reject


void UpdatesDialog::closeEvent( QCloseEvent *evt)   // Ignore close events if still working
{
    if ( _nupdater.isBusy() || _updateFile != nullptr)
        evt->ignore();
}   // end closeEvent


QSize UpdatesDialog::sizeHint() const { return QSize(455,355);}


void UpdatesDialog::_doOnReplyFinished( bool v)
{
    if (!v)
        _handleError( _nupdater.error());
    else if ( _gettingManifest)
        _checkRefreshedManifest();
    else
        _startUpdate();
    _gettingManifest = false;
}   // end _doOnReplyFinished


void UpdatesDialog::_checkRefreshedManifest()
{
    const int cmj = APP_VERSION_MAJOR;
    const int cmn = APP_VERSION_MINOR;
    const int cpt = APP_VERSION_PATCH;

    const QTools::UpdateMeta &umeta = _nupdater.meta();
    const int mj = umeta.major();
    const int mn = umeta.minor();
    const int pt = umeta.patch();
    static QString NO_UPDATE_MSG = QString( "**You already have the latest version %1.%2.%3.**\\").arg(mj).arg(mn).arg(pt);
    static QString UPDATE_MSG = QString( "**An update to version %1.%2.%3 is available! You have version %4.%5.%6.**\\").arg(mj).arg(mn).arg(pt).arg(cmj).arg(cmn).arg(cpt) +
                                FaceTools::loadTextFromFile(":/data/UPDATE_MSG");
    static QString SUPPORT_MSG = FaceTools::loadTextFromFile(":/data/SUPPORT_MSG");

    const bool found = umeta > _cmeta;
    _ui->updateButton->setEnabled( found);
    _ui->downloadProgressBar->setEnabled( found);
    _ui->downloadProgressBar->setRange( 0, 1);
    _ui->downloadProgressBar->setValue( 1);

    if ( found)
    {
        std::cerr << "Update found" << std::endl;
        _ui->updateDetails->setMarkdown( UPDATE_MSG + SUPPORT_MSG + umeta.details());
        _ui->downloadProgressBar->setFormat( "Ready to download");
    }   // end found
    else
    {
        std::cerr << "No update found" << std::endl;
        _ui->updateDetails->setMarkdown( NO_UPDATE_MSG + SUPPORT_MSG);
        _ui->downloadProgressBar->setFormat( "No update available");
    }   // end else

    adjustSize();

    if ( found)
        this->show();
}   // end _checkRefreshedManifest


void UpdatesDialog::_startUpdate()
{
    _ui->downloadProgressBar->setFormat( "Updating - please wait...");
    // Run the update in a separate thread
    AppUpdater *updater = new AppUpdater( _updateFile->fileName());
    connect( updater, &AppUpdater::onFinished, this, &UpdatesDialog::_doOnFinishedUpdate);
    connect( updater, &AppUpdater::finished, updater, &QObject::deleteLater);
    updater->start();
}   // end _startUpdate


void UpdatesDialog::_doOnUpdateButtonPushed()
{
    _ui->downloadProgressBar->setFormat( "Starting download...");
    _ui->updateButton->setEnabled( false);
    _ui->buttonBox->setEnabled( false);

    _updateFile = new QTemporaryFile( this);
    if ( !_updateFile->open())
        _handleError( "Unable to open temporary file!");
    else
    {
        if ( _nupdater.downloadUpdate( _updateFile->fileName()))
        {
            const std::string url = _nupdater.meta().updateUrl().toDisplayString().toStdString();
            std::cerr << "Downloading update from " << url << std::endl;
        }   // end if
        else
            _handleError( _nupdater.error());
    }   // end else
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


void UpdatesDialog::_doOnFinishedUpdate( QString err)
{
    if ( !err.isEmpty())
        _handleError( err);
    else
    {
        _cmeta = _nupdater.meta();
        _ui->downloadProgressBar->setFormat( "Update Complete!");
        QMB::information( this, tr("Update Complete!"), tr("Restart Cliniface to begin using the new version."));
    }   // end else
    _ui->buttonBox->setEnabled(true);
    _deleteDownloadedUpdateFile();
    this->close();
}   // end _doOnFinishedUpdate


void UpdatesDialog::_handleError( const QString &err)
{
    std::cerr << err.toStdString() << std::endl;
    _ui->updateButton->setEnabled( false);
    _ui->downloadProgressBar->setFormat( "Update Error!");
    QMB::warning( this, tr("Update Error!"), tr(err.toStdString().c_str()));
    _deleteDownloadedUpdateFile();
    this->close();
}   // end _handleError


void UpdatesDialog::_deleteDownloadedUpdateFile()
{
    if ( _updateFile)
        delete _updateFile;
    _updateFile = nullptr;
}   // end _deleteDownloadedUpdateFile

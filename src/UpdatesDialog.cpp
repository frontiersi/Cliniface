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

#include <UpdatesDialog.h>
#include <ui_UpdatesDialog.h>
#include <Cliniface_Config.h>
#include <FaceTools/MiscFunctions.h>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QTimer>
#include <QDir>
using Cliniface::UpdatesDialog;
using QMB = QMessageBox;

// static definitions
bool UpdatesDialog::s_autoCheckUpdate(false);
QUrl UpdatesDialog::s_patchURL;
void UpdatesDialog::setAutoCheckUpdate( bool v) { s_autoCheckUpdate = v;}
bool UpdatesDialog::autoCheckUpdate() { return s_autoCheckUpdate;}
void UpdatesDialog::setPatchURL( const QString &v) { s_patchURL.setUrl(v);}

namespace {
QString CHECKING_FOR_UPDATE_MSG = QObject::tr("**Checking for updates...**\\");
QString NO_UPDATE_MSG = QObject::tr( "**No updates are available**\\");
QString UPDATE_MSG = QObject::tr( "**Updates are available!**\\");
QString NETWORK_BUSY_MSG = QObject::tr("**Network is currently busy!**\\");
QString UNKNOWN_ERROR = QObject::tr("**Unknown update error!**\\");
}   // end namespace


UpdatesDialog::UpdatesDialog( QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint),
    _ui( new Ui::UpdatesDialog), _nupdater( s_patchURL, 10000, 20)
{
    _ui->setupUi(this);
    setWindowModality( Qt::NonModal);

    using QTools::NetworkUpdater;
    connect( &_nupdater, &NetworkUpdater::onRefreshedManifest, this, &UpdatesDialog::_doOnRefreshedManifest);
    connect( &_nupdater, &NetworkUpdater::onDownloadProgress, this, &UpdatesDialog::_doOnDownloadProgress);
    connect( &_nupdater, &NetworkUpdater::onFinishedDownloading, this, &UpdatesDialog::_doOnFinishedDownloading);
    connect( &_nupdater, &NetworkUpdater::onFinishedUpdating, this, &UpdatesDialog::_doOnFinishedUpdating);
    connect( &_nupdater, &NetworkUpdater::onError, this, &UpdatesDialog::_doOnError);

    connect( _ui->updateButton, &QPushButton::pressed, this, &UpdatesDialog::_doOnUpdateButtonPushed);
    /*
    connect( _ui->donateButton, &QPushButton::pressed, [](){ QDesktopServices::openUrl( QUrl( APP_DONATE_URL));});
    _ui->donateButton->setVisible(false);
    _ui->donateButton->setEnabled(false);
    */

    //const QRect &screenRect = QGuiApplication::primaryScreen()->geometry();
    //setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), screenRect));

    static const QString SUPPORT_MSG = FaceTools::loadTextFromFile(":/data/SUPPORT_MSG");
    CHECKING_FOR_UPDATE_MSG += SUPPORT_MSG;
    NO_UPDATE_MSG += SUPPORT_MSG;
    UPDATE_MSG += SUPPORT_MSG + FaceTools::loadTextFromFile(":/data/UPDATE_MSG");
    NETWORK_BUSY_MSG += SUPPORT_MSG;
    UNKNOWN_ERROR += SUPPORT_MSG;

    if ( autoCheckUpdate())
        QTimer::singleShot( 4000, [this](){ this->checkForUpdate();});
}   // end ctor


UpdatesDialog::~UpdatesDialog() { delete _ui;}


void UpdatesDialog::checkForUpdate()
{
    // Ignore request if currently checking
    if ( _nupdater.isBusy() || isVisible())
        return;

    _ui->updateButton->setEnabled( false);
    _ui->progressBar->setEnabled( false);
    _ui->progressBar->setRange( 0, 1);
    _ui->progressBar->setValue( 1);
    _ui->buttonBox->setEnabled(false);

    QString msg;
    const bool isOkay = _nupdater.refreshManifest( APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);
    if ( isOkay)
    {
        msg = CHECKING_FOR_UPDATE_MSG;
        std::cerr << "Fetching manifest " << s_patchURL.toDisplayString().toStdString() << std::endl;
        _ui->progressBar->setFormat( tr("Checking for updates..."));
    }   // end if
    else
    {
        if ( _nupdater.isBusy())
            msg = NETWORK_BUSY_MSG;
        else
            msg = UNKNOWN_ERROR;
    }   // end else

    _ui->updateDetails->setMarkdown( msg);
    _ui->updateDetails->setAlignment( Qt::AlignCenter);
    if ( !isOkay)
        _doOnError( _nupdater.error());
}   // end checkForUpdate


void UpdatesDialog::open()
{
    checkForUpdate();
    QDialog::open();
}   // end open


void UpdatesDialog::reject()    // Allow dialog close if not busy
{
    if ( !_nupdater.isBusy())
        QDialog::reject();
}   // end reject


void UpdatesDialog::closeEvent( QCloseEvent *evt)   // Ignore close events if still working
{
    if ( _nupdater.isBusy())
        evt->ignore();
}   // end closeEvent


QSize UpdatesDialog::sizeHint() const { return QSize(455,355);}


void UpdatesDialog::_doOnRefreshedManifest()
{
    const bool found = _nupdater.isUpdateAvailable();
    _ui->updateButton->setEnabled( found);
    _ui->progressBar->setEnabled( found);
    _ui->progressBar->setRange( 0, 1);
    _ui->progressBar->setValue( 1);
    _ui->buttonBox->setEnabled(true);

    if ( found)
    {
        _ui->updateDetails->setMarkdown( UPDATE_MSG + _nupdater.updateDescription());
        _ui->progressBar->setFormat( tr("Ready to update"));
    }   // end found
    else
    {
        _ui->updateDetails->setMarkdown( NO_UPDATE_MSG);
        _ui->progressBar->setFormat( tr("No updates are available"));
    }   // end else

    _ui->updateDetails->setAlignment( Qt::AlignCenter);
    if ( found)
        this->show();
}   // end _doOnRefreshedManifest


void UpdatesDialog::_doOnFinishedDownloading()
{
    _ui->progressBar->setFormat( tr("Updating - please wait..."));
}   // end _doOnFinishedDownloading


void UpdatesDialog::_doOnFinishedUpdating()
{
    static const QString tit = tr("Update Finished!");
    static const QString msg = tr("Restart to begin using the new version.");
    _ui->progressBar->setFormat( tit);
    QMB::information( this, tit, QString("<p align='center'>%1</p>").arg(msg));
    _ui->buttonBox->setEnabled(true);
    this->close();
}   // end _doOnFinishedUpdating


void UpdatesDialog::_doOnError( const QString &err)
{
    std::cerr << err.toStdString() << std::endl;
    _ui->updateButton->setEnabled( false);
    static const QString msg = tr("Update Error!");
    _ui->progressBar->setFormat( msg);
    if ( isVisible())
    {
        QMB::warning( this, msg, tr(err.toStdString().c_str()));
        this->close();
    }   // end if
}   // end _doOnError


void UpdatesDialog::_doOnUpdateButtonPushed()
{
    std::cerr << "Starting download/update..." << std::endl;
    _ui->progressBar->setFormat( tr("Starting update..."));
    _ui->updateButton->setEnabled( false);
    _ui->buttonBox->setEnabled( false);
    if ( !_nupdater.updateApp())
        _doOnError( _nupdater.error());
}   // end _doOnUpdateButtonPushed


void UpdatesDialog::_doOnDownloadProgress( double pcnt)
{
    static const QString msg = tr("Progress");
    if ( pcnt >= 0.0)
    {
        _ui->progressBar->setFormat( QString("%1 %p%").arg(msg));
        _ui->progressBar->setMaximum( static_cast<int>(100.0));
        _ui->progressBar->setValue( static_cast<int>(pcnt));
    }   // end if
    else
    {
        _ui->progressBar->setFormat( tr("Updating..."));
        _ui->progressBar->setRange( 0, 0);
        _ui->progressBar->setValue( 0);
    }   // end else
}   // end _doOnDownloadProgress

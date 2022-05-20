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

#include <ImageBrowser.h>
#include <ui_ImageBrowser.h>
using Cliniface::ImageBrowser;
#include <Cliniface_Config.h>
#include <Options.h>

#include <FaceTools/FileIO/BulkMetadataReader.h>
#include <FaceTools/FileIO/FaceModelDatabase.h>
#include <FaceTools/FileIO/FaceModelManager.h>
#include <FaceTools/Ethnicities.h>
#include <FaceTools/ModelSelect.h>
#include <QTools/FileIO.h>
#include <QSignalBlocker>
#include <QStorageInfo>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <cassert>
using FMD = FaceTools::FileIO::FaceModelDatabase;
using FMM = FaceTools::FileIO::FaceModelManager;
using MS = FaceTools::ModelSelect;
using FaceTools::Ethnicities;
using FaceTools::FM;
using QMB = QMessageBox;

namespace {
bool isPhysicalDevice( const QString &path)
{
    const QStorageInfo volume( path);
#ifdef Q_OS_LINUX
    return QString::fromLatin1(volume.device()).startsWith(QLatin1String("/dev/"));
#endif
#ifdef Q_OS_WIN
    return QString::fromLatin1(volume.device()).startsWith(QLatin1String("\\\\?\\Volume"));
#endif
    return false;
}   // end isPhysicalDevice
}   // end namespace


// static
bool ImageBrowser::s_parseExamples(true);
void ImageBrowser::setParseExamples( bool v) { s_parseExamples = v;}
// Location of the database file (will be created if it doesn't yet exist).
const QString ImageBrowser::s_dbfile( QDir::home().filePath( QString(".%1/imagedb").arg(EXE_NAME)));


void ImageBrowser::_doOnCapturedFromDateChanged()
{
    const QDate date0 = _ui->capturedFromDateEdit->date();
    const QDate date1 = _ui->capturedToDateEdit->date();
    if ( date0 > date1)   // "From" date can't be ahead of "to" date
    {
        QSignalBlocker blocker( _ui->capturedFromDateEdit);
        _ui->capturedFromDateEdit->setDate( date1);
    }   // end if
    _doFilterOnCriteria();
}   // end _doOnCapturedFromDateChanged


void ImageBrowser::_doOnCapturedToDateChanged()
{
    const QDate date0 = _ui->capturedFromDateEdit->date();
    const QDate date1 = _ui->capturedToDateEdit->date();
    if ( date0 > date1)   // "From" date can't be ahead of "to" date
    {
        QSignalBlocker blocker( _ui->capturedToDateEdit);
        _ui->capturedToDateEdit->setDate( date0);
    }   // end if
    else if ( date1 > QDate::currentDate())
    {
        QSignalBlocker blocker( _ui->capturedToDateEdit);
        _ui->capturedToDateEdit->setDate( QDate::currentDate());
    }   // end if
    _doFilterOnCriteria();
}   // end _doOnCapturedToDateChanged


void ImageBrowser::_doOnBirthdateFromDateChanged()
{
    const QDate date0 = _ui->dobFromDateEdit->date();
    const QDate date1 = _ui->dobToDateEdit->date();
    if ( date0 > date1)   // "From" date can't be ahead of "to" date
    {
        QSignalBlocker blocker( _ui->dobFromDateEdit);
        _ui->dobFromDateEdit->setDate( date1);
    }   // end if
    _doFilterOnCriteria();
}   // end _doOnBirthdateFromDateChanged


void ImageBrowser::_doOnBirthdateToDateChanged()
{
    const QDate date0 = _ui->dobFromDateEdit->date();
    const QDate date1 = _ui->dobToDateEdit->date();
    if ( date0 > date1)   // "From" date can't be ahead of "to" date
    {
        QSignalBlocker blocker( _ui->dobToDateEdit);
        _ui->dobToDateEdit->setDate( date0);
    }   // end if
    else if ( date1 > QDate::currentDate())
    {
        QSignalBlocker blocker( _ui->dobToDateEdit);
        _ui->dobToDateEdit->setDate( QDate::currentDate());
    }   // end if
    _doFilterOnCriteria();
}   // end _doOnBirthdateToDateChanged


void ImageBrowser::_doOnResetFilters()
{
    QSignalBlocker b0( _ui->capturedFromDateEdit);
    QSignalBlocker b1( _ui->capturedToDateEdit);
    QSignalBlocker b2( _ui->dobFromDateEdit);
    QSignalBlocker b3( _ui->dobToDateEdit);
    QSignalBlocker b4( _ui->methnicityFilterComboBox);
    QSignalBlocker b5( _ui->methnicityFilterComboBox);
    QSignalBlocker b6( _ui->sexFilterComboBox);
    QSignalBlocker b7( _ui->subjectFilterLineEdit);
    QSignalBlocker b8( _ui->studyFilterLineEdit);
    QSignalBlocker b9( _ui->sourceFilterLineEdit);

    // Get min and max image dates
    QDate minImgDate, maxImgDate;
    FMD::minMaxImageDates( minImgDate, maxImgDate);
    _ui->capturedFromDateEdit->setDate( minImgDate);
    _ui->capturedToDateEdit->setDate( maxImgDate);
    // Get min and max birthdates from all subjects
    QDate minDobDate, maxDobDate;
    FMD::minMaxBirthDates( minDobDate, maxDobDate);
    _ui->dobFromDateEdit->setDate( minDobDate);
    _ui->dobToDateEdit->setDate( maxDobDate);
    // Reset sex and ethnicity widgets
    Ethnicities::resetComboBox( _ui->methnicityFilterComboBox);
    Ethnicities::resetComboBox( _ui->pethnicityFilterComboBox);
    _ui->sexFilterComboBox->setCurrentIndex( _ui->sexFilterComboBox->findData( FaceTools::UNKNOWN_SEX));
    // Clear subject, study, and source widgets
    _ui->subjectFilterLineEdit->clear();
    _ui->studyFilterLineEdit->clear();
    _ui->sourceFilterLineEdit->clear();

    _doFilterOnCriteria();
}   // end _doOnResetFilters

namespace {
QString _makeEthnicCodeFilter( const QString &txt, const QString &tag)
{
    using Ethn = FaceTools::Ethnicities;
    const int eth = Ethn::code( txt);
    QString filter;
    QTextStream ts(&filter);
    if ( Ethn::isBroad( eth))
        ts << "AND " << tag << " >= " << eth << " AND " << tag << " < " << (eth+1000);
    else if ( Ethn::isNarrow( eth))
        ts << "AND " << tag << " >= " << eth << " AND " << tag << " < " << (eth+100);
    else if ( Ethn::isMixed( eth))
    {
        ts << "AND " << tag << " IN (" << eth;
        for ( int ccode : Ethn::childCodes( eth))
            ts << ", " << ccode;
        ts << ")";
    }   // end else if
    else if ( eth > 0)
        ts << "AND " << tag << " = " << eth;
    return filter;
}   // end _makeEthnicCodeFilter

QString _makeLikeFilter( QString txt, const QString &tag)
{
    QString filter;
    if ( !txt.isEmpty())
    {
        txt.replace( "?", "_");
        txt.replace( "*", "%");
        filter = QString("AND %1 LIKE '%2'").arg(tag).arg(txt);
    }   // end if
    return filter;
}   // end _makeLikeFilter

}   // end namespace


// Returns the row index for the given filepath or -1 if not found.
int ImageBrowser::_tableRowForFilePath( const QString &fpath) const
{
    const size_t nrows = _model->rowCount();
    for ( size_t i = 0; i < nrows; ++i)
        if ( _model->record(i).value("filepath").toString() == fpath)
            return i;
    return -1;
}   // end _tableRowForFilePath


void ImageBrowser::_doFilterOnCriteria()
{
    if ( _filtering)
        return;

    _filtering = true;
    const QString fromDate = _ui->capturedFromDateEdit->date().toString("yyyy-MM-dd");
    const QString toDate = _ui->capturedToDateEdit->date().toString("yyyy-MM-dd");
    const QString fromDob = _ui->dobFromDateEdit->date().toString("yyyy-MM-dd");
    const QString toDob = _ui->dobToDateEdit->date().toString("yyyy-MM-dd");

    QString sexFilter;
    const int8_t sex = static_cast<int8_t>(_ui->sexFilterComboBox->currentData().toInt());
    if ( sex != FaceTools::UNKNOWN_SEX)
        sexFilter = QString("AND sex = %1").arg(sex);

    const QString methFilter = _makeEthnicCodeFilter( _ui->methnicityFilterComboBox->currentText(), "maternalethnicity");
    const QString pethFilter = _makeEthnicCodeFilter( _ui->pethnicityFilterComboBox->currentText(), "paternalethnicity");

    const QString sbjctFilter = _makeLikeFilter( _ui->subjectFilterLineEdit->text(), "identifier");
    const QString studyFilter = _makeLikeFilter( _ui->studyFilterLineEdit->text(), "study");
    const QString sorceFilter = _makeLikeFilter( _ui->sourceFilterLineEdit->text(), "source");

    _model->setFilter( QString(R"(capturedate >= '%1' AND capturedate <= '%2' AND birthdate >= '%3' AND birthdate <= '%4'
                %5 %6 %7 %8 %9 %10
                )").arg(fromDate, toDate, fromDob, toDob, sexFilter, methFilter, pethFilter, sbjctFilter, studyFilter, sorceFilter));
    _model->select();
    while ( _model->canFetchMore()) // Allow more than 256 rows to be loaded
        _model->fetchMore();

    const size_t nrows = _testAndSetEnableLoad();
    const int ridx = _tableRowForFilePath( FMM::filepath( *MS::selectedModel()));
    if ( ridx >= 0)
        _ui->tableView->selectRow( ridx);
    else if ( nrows > 0)
        _ui->tableView->selectRow( 0);
    else
        _setThumbnail();    // Blank thumbnail

    _filtering = false;
}   // end _doFilterOnCriteria


size_t ImageBrowser::_testAndSetEnableLoad()
{
    const size_t nrows = _model->rowCount();
    _ui->loadSelectedPushButton->setEnabled( nrows > 0 && !FMM::loadLimitReached());
    return nrows;
}   // end _testAndSetEnableLoad


void ImageBrowser::_doOnCreateAverage()
{
    std::cerr << "ImageBrowser::_doOnCreateAverage TODO" << std::endl;
}   // end _doOnCreateAverage


void ImageBrowser::updateSelected( const QString &fpath)
{
    if ( !_refreshing && !_filtering && !fpath.isEmpty())
    {
        const int ridx = _tableRowForFilePath( fpath);
        if ( ridx < 0)
            _doOnResetFilters();
        else
            _doFilterOnCriteria();
    }   // end if
    else
        _testAndSetEnableLoad();
}   // end updateSelected


void ImageBrowser::_doOnCurrentItemChanged( const QModelIndex &idx, const QModelIndex &prv)
{
    if ( idx.row() != prv.row())
    {
        QPixmap pmap;
        if ( idx.row() >= 0)
            pmap = FMD::imageThumbnail( _model->record(idx.row()).value("filepath").toString());
        _setThumbnail(pmap);
    }   // end if
}   // end _doOnCurrentItemChanged


void ImageBrowser::_setThumbnail( const QPixmap &pm)
{
    QPixmap pmap;
    if ( pm.isNull())
    {
        pmap = QPixmap( _ui->imageLabel->size());
        pmap.fill();    // With white
    }   // end if
    else
        pmap = pm.scaled( _ui->imageLabel->size(), Qt::AspectRatioMode::KeepAspectRatio);
    _ui->imageLabel->setPixmap( pmap);
}   // end _setThumbnail


// Left double-clicking a row
void ImageBrowser::_doOnItemActivated( const QModelIndex &idx)
{
    const QString filepath = _model->record(idx.row()).value("filepath").toString();
    const QFileInfo finfo( filepath);

    QString mbmsg, mbtitle;
    if ( FMM::isOpen( filepath))
    {
        mbtitle = tr("File Already Open");
        mbmsg = tr("The selected file is already open!");
    }   // end if
    else if ( FMM::loadLimitReached())
    {
        mbtitle = tr("Load Limit Reached");
        mbmsg = tr("A maximum of two models can be loaded at once; close one and try again!");
    }   // end else if
    else if ( !finfo.exists())
    {
        mbtitle = tr("File Not Found!");
        mbmsg = tr("The selected file wasn't found (has it been moved?) so it will be removed from the database until reloaded.");
        if ( FMD::removeImage( filepath))
            _doFilterOnCriteria();
        else
            std::cerr << "[ERR] Cliniface::ImageBrowser::_doOnItemActivated: Unable to remove image!\n";
    }   // end else if
    else if ( !finfo.isReadable())
    {
        mbtitle = tr("File Unreadable!");
        mbmsg = tr("The selected file cannot be read! Change its permissions and try again.");
    }   // end else if

    if ( !mbmsg.isEmpty())
        QMB::information( static_cast<QWidget*>(parent()), mbtitle, QString("<p align='center'>%1</p>").arg(mbmsg));
    else
    {
        _ui->loadSelectedPushButton->setEnabled( false);
        emit onLoad( filepath);
    }   // end else
}   // end _doOnItemActivated


void ImageBrowser::_set3DFsDir( const QString &dname)
{
    if ( dname.isEmpty())
    {
        std::cerr << "[ERR] Cliniface::ImageBrowser::_set3DFsDir: Empty directory passed in!\n";
        _clearDB = false;
        return;
    }   // end if

    if ( !isPhysicalDevice( dname))
    {
        static const QString msg = tr("This directory is on a network so parsing 3DF files may take a while! Do you wish to continue?");
        if ( QMB::No == QMB::question( this, tr("Network Directory Selected!"), QString("<p align='center'>%1</p>").arg(msg), QMB::Yes | QMB::No, QMB::No))
        {
            _setInterfaceEnabled(true);
            _clearDB = false;
            return;
        }   // end if
    }   // end if

    const QFileInfo finfo( dname);
    const bool foundDir = finfo.exists() && finfo.isDir() && finfo.isReadable();
    if ( foundDir)
    {
        _setInterfaceEnabled(false);
        const QString msg = tr("Looking for 3DF files in \"%1\"...").arg(dname);
        std::cerr << msg.toStdString() << "\n";
        MS::showStatus( msg);
        using BFF = QTools::FileIO::BackgroundFilesFinder;
        BFF *ffinder = new BFF( QDir(dname), {"*.3df"});
        connect( ffinder, &BFF::finished, ffinder, &BFF::deleteLater);
        connect( ffinder, &BFF::onFoundFiles, this, &ImageBrowser::_doOnFound3DFs);
        ffinder->start();   // Asynchronous start
    }   // end if
}   // end _set3DFsDir


void ImageBrowser::_doOnFound3DFs( const QDir &root, const QFileInfoList &files)
{
    QString msg = tr("No 3DF files were found in \"%1\"!").arg(root.path());

    // No 3DF files found? Show message and return.
    if ( files.isEmpty())
    {
        std::cerr << msg.toStdString() << std::endl;
        MS::showStatus( msg, 5000);
        QMB::information( static_cast<QWidget*>(parent()), tr("No Files Found!"), QString("<p align='center'>%1</p>").arg(msg));
        _setInterfaceEnabled(true);
        return;
    }   // end if

    const QString parsedDir = root.absolutePath();
    const size_t nfiles = files.size();

    msg = tr("Found %1 3DF files in \"%2\"; reading metadata...").arg(nfiles).arg(parsedDir);
    std::cerr << msg.toStdString() << std::endl;
    MS::showStatus( msg);
    using BMR = FaceTools::FileIO::BulkMetadataReader;
    BMR *bmr = new BMR( files, true/*also load thumbnails*/);
    connect( bmr, &BMR::finished, bmr, &BMR::deleteLater);
    connect( bmr, &BMR::onLoadedModels, this, &ImageBrowser::_doOnLoadedMetadata);
    connect( bmr, &BMR::onCancelled, this, &ImageBrowser::_doOnLoadMetadataCancelled);
    connect( bmr, &BMR::onPercentProgress, _pdialog, &ProgressDialog::doOnPercentProgress);
    connect( _pdialog, &ProgressDialog::onCancelled, bmr, &BMR::cancel);
    _pdialog->reset( nfiles);
    if ( QFileInfo( parsedDir) != QFileInfo(Options::exampleImagesDir()))
        _pdialog->open();
    bmr->start();	// Asynchronous start
}   // end _doOnFound3DFs


void ImageBrowser::_doOnLoadMetadataCancelled()
{
    const QString msg = tr( "Parsing of 3DF metadata cancelled!");
    std::cerr << msg.toStdString() << std::endl;
    MS::showStatus( msg, 5000);
    _finishLoadMetadata();
}   // end _doOnLoadMetadataCancelled


void ImageBrowser::_doOnLoadedMetadata( const QFileInfoList &files, const QList<FM*> &data)
{
    const size_t n = files.size();
    const QString msg0 = tr( "Finishing parsing %1 3DFs; refreshing database...").arg(n);
    std::cerr << msg0.toStdString() << std::endl;
    MS::showStatus( msg0);

    assert( n == data.size());
    size_t nadded = 0;

    if ( _clearDB)   // Clear the database first?
    {
        FMD::clear();
        _clearDB = false;
    }   // end if

    _refreshing = true;
    QSqlDatabase::database().transaction();

    for ( size_t i = 0; i < n; ++i)
    {
        const QFileInfo &finfo = files.at(i);
        const QString fpath = finfo.absoluteFilePath();
        FM *fm = data.at(i);
        if ( !fm)
		{
			//std::cerr << "[WARN] Cliniface::ImageBrowser: Unable to load file at \"" << fpath.toStdString() << "\"!\n";
            continue;
		}	// end if

        if ( FMD::refreshImage( *fm, fpath, "", false)) // Returns true if image added
        {
            nadded++;
            if ( !fm->isMetaSaved())
            {
                const QString msg = tr("Subject details in \"%1\" disagree with other images of the same subject so they've been updated for consistency; please confirm and save the changes!").arg(fpath);
                QMB::warning( this, tr("Subject Details Mismatch!"), QString("<p align='center'>%1</p>").arg(msg));
            }   // end if
        }   // end if

        delete fm;
    }   // end for

    QSqlDatabase::database().commit();
    if ( nadded > 0)
    {
        const QString msg = tr( "Added %1 images to database from %2 3DF files.").arg(nadded).arg(n);
        std::cerr << msg.toStdString() << std::endl;
        MS::showStatus( msg, 10000);
    }   // end if

    _finishLoadMetadata();
}   // end _doOnLoadedMetadata


void ImageBrowser::_finishLoadMetadata()
{
    _refreshing = false;
    _doOnResetFilters();
    _setInterfaceEnabled(true);
}   // end _finishLoadMetadata


void ImageBrowser::_setInterfaceEnabled( bool v)
{
    _ui->imagesPathButton->setEnabled(v);
    _ui->resetFiltersPushButton->setEnabled(v);
    _ui->loadSelectedPushButton->setEnabled(v);
    _ui->capturedFromDateEdit->setEnabled(v);
    _ui->capturedToDateEdit->setEnabled(v);
    _ui->dobFromDateEdit->setEnabled(v);
    _ui->dobToDateEdit->setEnabled(v);
    _ui->methnicityFilterComboBox->setEnabled(v);
    _ui->pethnicityFilterComboBox->setEnabled(v);
    _ui->sexFilterComboBox->setEnabled(v);
    _ui->subjectFilterLineEdit->setEnabled(v);
    _ui->studyFilterLineEdit->setEnabled(v);
    _ui->sourceFilterLineEdit->setEnabled(v);
    _ui->tableView->setEnabled(v);
}   // end _setInterfaceEnabled


void ImageBrowser::_doOnClickedSetImagesDirButton()
{
    if ( _fdialog->exec())
    {
        const QStringList paths = _fdialog->selectedFiles();
        if ( !paths.isEmpty())
        {
            _clearDB = true;
            _set3DFsDir( paths.first());
        }   // end if
    }   // end if
}   // end _doOnClickedSetImagesDirButton


ImageBrowser::ImageBrowser( QWidget *parent) :
    QMainWindow(parent), _ui(new Ui::ImageBrowser),
    _fdialog(nullptr), _pdialog(nullptr),
    _model(nullptr), _refreshing(false), _filtering(false), _clearDB(false)
{
    setWindowFlags( windowFlags() & ~Qt::WindowMinMaxButtonsHint);  // No minimise or maximise buttons
    _ui->setupUi(this);
    QString winTitle = "Image Browser";
    if ( parent)
        winTitle = parent->windowTitle() + " | " + winTitle;
    setWindowTitle( winTitle);

    // Initialise sex and ethnicity filter combo boxes
    QTools::TreeModel *emodel = Ethnicities::createComboBoxModel();
    _ui->methnicityFilterComboBox->setModel( emodel);
    _ui->pethnicityFilterComboBox->setModel( emodel);
    _ui->sexFilterComboBox->addItem( toLongSexString( FaceTools::UNKNOWN_SEX), FaceTools::UNKNOWN_SEX);
    _ui->sexFilterComboBox->addItem( toLongSexString( FaceTools::FEMALE_SEX), FaceTools::FEMALE_SEX);
    _ui->sexFilterComboBox->addItem( toLongSexString( FaceTools::MALE_SEX), FaceTools::MALE_SEX);

    connect( _ui->loadSelectedPushButton, &QPushButton::clicked, this, [this](){
        _doOnItemActivated(_ui->tableView->selectionModel()->selectedRows().first()); });
    connect( _ui->resetFiltersPushButton, &QPushButton::clicked, this, &ImageBrowser::_doOnResetFilters);
    _ui->makeAveragePushButton->setVisible(false);  // DISABLED FOR NOW
    connect( _ui->makeAveragePushButton, &QPushButton::clicked, this, &ImageBrowser::_doOnCreateAverage);

    // Trigger filtering when changing contents of any of the filter widgets
    connect( _ui->capturedFromDateEdit, &QDateEdit::dateChanged, this, &ImageBrowser::_doOnCapturedFromDateChanged);
    connect( _ui->capturedToDateEdit, &QDateEdit::dateChanged, this, &ImageBrowser::_doOnCapturedToDateChanged);
    connect( _ui->dobFromDateEdit, &QDateEdit::dateChanged, this, &ImageBrowser::_doOnBirthdateFromDateChanged);
    connect( _ui->dobToDateEdit, &QDateEdit::dateChanged, this, &ImageBrowser::_doOnBirthdateToDateChanged);
    connect( _ui->sexFilterComboBox, QOverload<int>::of(&QComboBox::activated), this, &ImageBrowser::_doFilterOnCriteria);
    // currentIndexChanged used because of selection mechanics of tree model.
    connect( _ui->methnicityFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageBrowser::_doFilterOnCriteria);
    connect( _ui->pethnicityFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageBrowser::_doFilterOnCriteria);
    connect( _ui->subjectFilterLineEdit, &QLineEdit::textEdited, this, &ImageBrowser::_doFilterOnCriteria);
    connect( _ui->studyFilterLineEdit, &QLineEdit::textEdited, this, &ImageBrowser::_doFilterOnCriteria);
    connect( _ui->sourceFilterLineEdit, &QLineEdit::textEdited, this, &ImageBrowser::_doFilterOnCriteria);

    // Table model, view, and control definitions
    _ui->tableView->setFocusPolicy( Qt::StrongFocus);
    _ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers);
    _ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows);
    _ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);
    _ui->tableView->setSortingEnabled(true);
    FMD::init( s_dbfile);    // Create database
    _model = FMD::createModel();
    _ui->tableView->setModel(_model);
    _ui->tableView->setItemDelegate( new QSqlRelationalDelegate( _ui->tableView));
    _ui->tableView->hideColumn(0);  // id (primary key)
    _ui->tableView->hideColumn(2);  // imageid
    _ui->tableView->hideColumn(3);  // thumbnail
    _ui->tableView->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents);
    QHeaderView *hheader = _ui->tableView->horizontalHeader();
    hheader->setSectionResizeMode( QHeaderView::ResizeToContents);
    hheader->setStretchLastSection(true);
    // Connect up table row selection and item activation (loading models)
    connect( _ui->tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ImageBrowser::_doOnCurrentItemChanged);
    connect( _ui->tableView, &QTableView::activated, this, &ImageBrowser::_doOnItemActivated);

    /*
    // Set refiltering upon changes to the database tables
    QSqlDriver *sqlDriver = QSqlDatabase::database().driver();
    sqlDriver->subscribeToNotification( "images");
    sqlDriver->subscribeToNotification( "subjects");
    connect( sqlDriver, QOverload<const QString &, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification),
            this, &ImageBrowser::_doFilterOnCriteria);
    */

    _pdialog = new ProgressDialog( this);
    winTitle = "Loading Metadata";
    if ( parent)
        winTitle = parent->windowTitle() + " | " + winTitle;
    _pdialog->setWindowTitle( winTitle);

    // Configure directory dialog for setting new 3DF file search path
    _fdialog = new QFileDialog( this, tr( "Recursively search the selected directory for 3DF files."));
    _fdialog->setFileMode( QFileDialog::Directory);
    _fdialog->setOption( QFileDialog::ShowDirsOnly, true);
    _fdialog->setViewMode( QFileDialog::Detail);
    connect( _ui->imagesPathButton, &QToolButton::clicked, this, &ImageBrowser::_doOnClickedSetImagesDirButton);

    //adjustSize();
    //setFixedSize( geometry().width(), geometry().height());
}   // end ctor


ImageBrowser::~ImageBrowser()
{
    delete _fdialog;
    delete _pdialog;
    delete _model;
    delete _ui;
}   // end dtor


// Search for 3DF files
void ImageBrowser::init()
{
	if ( s_parseExamples)
    {
        _clearDB = false;
        _set3DFsDir( Options().exampleImagesDir());
    }   // end if
}   // end init

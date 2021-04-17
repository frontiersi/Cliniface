/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QTemporaryFile>
#include <QSharedMemory>
#include <QTextStream>
#include <QSet>
#include <QDir>
#include <fstream>
#include <functional>

#include <FaceTools/FaceModelCurvatureStore.h>
#include <FaceTools/FaceModelSymmetryStore.h>
#include <FaceTools/FaceModelDeltaStore.h>

#include <FaceTools/Ethnicities.h>

#include <FaceTools/Action/ActionDetectFace.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/Action/ActionUpdateMeasurements.h>

#include <FaceTools/FileIO/FaceModelManager.h>
#include <FaceTools/FileIO/FaceModelAssImpFileHandlerFactory.h>
#include <FaceTools/FileIO/FaceModelOBJFileHandler.h>
#include <FaceTools/FileIO/FaceModelPLYFileHandler.h>
#include <FaceTools/FileIO/FaceModelU3DFileHandler.h>
#include <FaceTools/FileIO/FaceModelSTLFileHandler.h>
#include <FaceTools/FileIO/FaceModelFileData.h>

#include <FaceTools/Metric/Metric.h>
#include <FaceTools/Metric/GeneManager.h>
#include <FaceTools/Metric/MetricManager.h>
#include <FaceTools/Metric/SyndromeManager.h>
#include <FaceTools/Metric/PhenotypeManager.h>
#include <FaceTools/Metric/StatsManager.h>

#include <FaceTools/LndMrk/LandmarksManager.h>
#include <FaceTools/Report/ReportManager.h>
#include <FaceTools/U3DCache.h>

#include <QTools/FileIO.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ClinifaceApp.h>
#include <ClinifaceMain.h>
#include <UpdatesDialog.h>
#include <Preferences.h>

using LMAN = FaceTools::Landmark::LandmarksManager;
using RMAN = FaceTools::Report::ReportManager;
using FMM = FaceTools::FileIO::FaceModelManager;
using MM = FaceTools::Metric::MetricManager;
using FaceTools::FM;

namespace {

void initFileIO()
{
    using namespace FaceTools::FileIO;
    FMM::add( new FaceModelXMLFileHandler);    // Default (preferred)
    FMM::add( new FaceModelOBJFileHandler);
    FMM::add( new FaceModelPLYFileHandler);
    FMM::add( new FaceModelSTLFileHandler);
    FMM::add( new FaceModelU3DFileHandler);
    //FMM::add( FaceModelAssImpFileHandlerFactory::make("3mf"));    // Tested and found not to work
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("acc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ask"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ase"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("blend"));    // Only for <= 2.79
    FMM::add( FaceModelAssImpFileHandlerFactory::make("dae"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("dxf"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("enff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("fbx"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifczip"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lwo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lws"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lxo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("mot"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ms3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ndo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("nff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("off"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3s"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3o"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("raw"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("sib"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("smd"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("stp"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ter"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("vta"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("x3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("xgl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("zgl"));
}   // end initFileIO


void initBase()
{
    FaceTools::Ethnicities::load( ":/data/ETHNICITIES");
    LMAN::load( ":/data/LANDMARKS");
    const QDir appDir( QCoreApplication::applicationDirPath());
    LMAN::loadImages( appDir.filePath( LANDMARK_IMGS_DIR));
    QTools::FileIO::APP_IMAGE_TOOL = appDir.filePath( APPIMAGETOOL_NAME);
    QTools::FileIO::FILE_MOVE_TOOL = appDir.filePath( FILEMOVETOOL_NAME);
}   // end initBase


bool initReports()
{
    using FaceTools::Report::Report;
    Report::setLogoPath(":/logos/PDF_LOGO");
    Report::setHeaderAppName( APP_NAME);
    Report::setVersionString( APP_VERSION_STRING);
    return RMAN::load( QDir( QCoreApplication::applicationDirPath()).filePath( REPORTS_DIR)) >= 0;
}   // end initReports


void initMetrics()
{
    const QDir appDir( QCoreApplication::applicationDirPath());
    using namespace FaceTools::Metric;
    MM::load( appDir.filePath( METRICS_DIR));
    StatsManager::load( appDir.filePath( STATISTICS_DIR));
    //StatsManager::load( appDir.filePath( TEST_STATISTICS_DIR));
    PhenotypeManager::load( appDir.filePath( HPOS_DIR));
    GeneManager::load( ":/data/GENES");
    SyndromeManager::load( ":/data/SYNDROMES");
}   // end initMetrics


void listReports()
{
    std::cout << "Available reports (id| name):" << std::endl;
    int id = 0;
    for ( const QString &rname : RMAN::names())
    {
        std::cout << std::setw(3) << id << "| " << rname.toLocal8Bit().toStdString() << std::endl;
        id++;
    }   // end for
}   // end listReports


bool isValidInputFile( const QFileInfo &infile)
{
    return infile.exists() && FMM::canRead( infile.absoluteFilePath());
}   // isValidInputFile


QString removeExamplesLink()
{
#ifdef _WIN32
    QString examplesName = "examples.lnk";
#else
    QString examplesName = "examples";
#endif
    const QString linkTarget = QDir::home().filePath( QString(".%1/%2").arg(EXE_NAME).arg(examplesName));
    QFile::remove( linkTarget);
    return linkTarget;
}   // end removeExamplesLink


// Make shortcut/symlink from examples directory from installation/mount dir to the user's .cliniface
// directory. Do this every time because on Linux, the mount point changes with each execution (since
// Cliniface is run from AppImage).
void makeExamplesLink()
{
    const QString linkTarget = removeExamplesLink();
    QFile::link( QDir( QCoreApplication::applicationDirPath()).filePath( EXAMPLES_DIR), linkTarget);
}   // end makeExamplesLink


QString tr( const QString& m) { return QObject::tr( m.toStdString().c_str());}


bool openFileStream( const QString &fpath, std::ofstream &ofs)
{
    ofs.open( fpath.toLocal8Bit().toStdString());
    return ofs.is_open();
}   // end openFileStream


bool exportToCSV( const FM *fm, const QString &fpath)
{
    std::ofstream ofs;
    if ( openFileStream( fpath, ofs))
    {
        FaceTools::FileIO::FaceModelFileData( *fm).toCSV( ofs);
        ofs.close();
        return true;
    }   // end if
    return false;
}   // exportToCSV


bool exportToXML( const FM *fm, const QString &fpath)
{
    std::ofstream ofs;
    if ( openFileStream( fpath, ofs))
    {
        PTree tree;
        FaceTools::FileIO::exportMetaData( *fm, true/*export calliper measurement vertices*/, tree);
        boost::property_tree::write_xml( ofs, tree);
        ofs.close();
        return true;
    }   // end if
    return false;
}   // end exportToXML


bool exportToJSON( const FM *fm, const QString &fpath)
{
    std::ofstream ofs;
    if ( openFileStream( fpath, ofs))
    {
        PTree tree;
        FaceTools::FileIO::exportMetaData( *fm, true/*export calliper measurement vertices*/, tree);
        boost::property_tree::write_json( ofs, tree);
        ofs.close();
        return true;
    }   // end if
    return false;
}   // end exportToJSON


bool write( const FM *fm, const QFileInfo &finfo)
{
    bool writeOkay = false;
    QString fpath = finfo.absoluteFilePath();
    const QString suffix = finfo.suffix().toLower();
    if ( suffix == "csv")
        writeOkay = exportToCSV( fm, fpath);
    else if ( suffix == "xml")
        writeOkay = exportToXML( fm, fpath);
    else if ( suffix == "json")
        writeOkay = exportToJSON( fm, fpath);
    else
        writeOkay = FMM::write( *fm, fpath);
    return writeOkay;
}   // end write


bool exportReport( const FM *fm, int repId, const QFileInfo &finfo)
{
    assert( finfo.suffix().toLower() == "pdf");
    const QString repName = RMAN::names().at(repId);
    std::cout << "Generating report '" << repName.toStdString() << "'; please wait..." << std::endl;
    FaceTools::Report::Report::Ptr report = RMAN::report( repName);
    assert( report);
    if ( !FaceTools::U3DCache::refresh( *fm))
    {
        std::cerr << "Unable to cache U3D model!" << std::endl;
        return false;
    }   // end if

    if ( !report->setContent())
    {
        std::cerr << "Unable to set report content!" << std::endl;
        return false;
    }   // end if

    if ( !report->generate())
    {
        std::cerr << "Unable to generate PDF!" << std::endl;
        return false;
    }   // end if

    const QString abspath = finfo.absoluteFilePath();
    const std::string sabspath = abspath.toLocal8Bit().toStdString();
    if (!QFile::copy( report->pdffile(), abspath))
    {
        std::cerr << "Unable to copy generated report to '" << sabspath << "'" << std::endl;
        return false;
    }   // end if

    std::cout << "Report saved to '" << sabspath << "'" << std::endl;
    FaceTools::U3DCache::purge( *fm);
    return true;
}   // end exportReport


void cutOutFace( FM *fm)
{
    std::cout << "Extracting face..." << std::endl;
    r3d::Mesh::Ptr mesh = FaceTools::Action::ActionExtractFace::extract(*fm);
    fm->update( mesh, true, true, 1);   // Keep one manifold
}   // end cutOutFace


bool mapFace( FM *fm)
{
    std::cout << "Mapping face..." << std::endl;
    const IntSet &lmids = FaceTools::Landmark::LandmarksManager::ids();
    return FaceTools::Action::ActionDetectFace::detect( *fm, lmids, true/*align first*/);
}   // end mapFace


void printHeader()
{
#ifdef _WIN32
    std::cout << std::endl;
#endif
    qInfo() << " ======================================================================";
    qInfo( " Cliniface version %s <%s>", APP_VERSION_STRING, APP_WEBSITE);
    qInfo( " Copyright %s %s & %s", APP_CR_YEARS, APP_ORGANISATION, APP_AUTHOR_NAME);
    qInfo( " Developed by %s <%s>", APP_AUTHOR_NAME, APP_CONTACT_EMAIL);
    qInfo() << " ----------------------------------------------------------------------";
    qInfo() << " Cliniface is free software: you can redistribute it and/or modify it";
    qInfo() << " under the terms of the GNU General Public License as published by the";
    qInfo() << " Free Software Foundation, either version 3 of the License, or (at your";
    qInfo() << " option) any later version. Cliniface is distributed in the hope that";
    qInfo() << " it will be useful, but WITHOUT ANY WARRANTY; without even the implied";
    qInfo() << " warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For";
    qInfo() << " further information about this, go to <http://www.gnu.org/licenses/>.";
    qInfo() << " ======================================================================";
}   // end printHeader


void checkOpenGLVersion()
{
    const QSurfaceFormat sfmt = QSurfaceFormat::defaultFormat();
    std::cout << "Targetting OpenGL " << sfmt.majorVersion() << "." << sfmt.minorVersion() << std::endl;
    QOffscreenSurface surf;
    surf.create();
    QOpenGLContext ctx;
    ctx.create();
    ctx.makeCurrent( &surf);
    std::cout << "OS supports OpenGL version " << (const char*)ctx.functions()->glGetString(GL_VERSION) << std::endl;
}   // end checkOpenGLVersion


void makeConfigDir()
{
    const QFileInfo finfo( QDir::home().filePath( QString(".%1").arg(EXE_NAME)));
    if ( finfo.exists() && !finfo.isDir())  // If .cliniface exists as a file, remove it
        QFile::remove( finfo.filePath());
    // Make the directory if it doesn't exist already as well as the user's plugins directory
    if ( !finfo.exists())
    {
        std::cerr << QString("Initialising %1 directory").arg(finfo.filePath()).toStdString() << std::endl;
        QDir::home().mkpath( finfo.filePath());
        QDir::home().mkpath( finfo.filePath() + "/plugins");// Make the user plugins directory
    }   // end if
}   // end makeConfigDir


void makeExeLink()
{
    // Make link to bin/cliniface if it doesn't already exist within 
    QString exelnkpath = QCoreApplication::applicationDirPath() + "/../" + APP_NAME;
#ifdef _WIN32
    exelnkpath += ".lnk";
#endif
    if ( !QFile::exists( exelnkpath))
        QFile::link( QCoreApplication::applicationFilePath(), exelnkpath);
}   // end makeExeLink


#ifdef _WIN32
VOID ErrorExit(LPSTR lpszMessage) 
{ 
    fprintf(stderr, "%s\n", lpszMessage); 
    ExitProcess(0); 
}   // end ErrorExit

std::string getWindowsConsoleInput()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");
    DWORD fdwSaveOldMode;
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode) )
        ErrorExit("GetConsoleMode");
    if (!SetConsoleMode( hStdin, ENABLE_WINDOW_INPUT))
        ErrorExit("SetConsoleMode");
    DWORD cNumRead;
    INPUT_RECORD irInBuf;
    if (!ReadConsoleInput( hStdin, &irInBuf, 1, &cNumRead))
        ErrorExit("ReadConsoleInput");

    CHAR c;
    if ( irInBuf.EventType == KEY_EVENT)
    {
        KEY_EVENT_RECORD keyRec = irInBuf.Event.KeyEvent;
        c = keyRec.uChar.AsciiChar;
    }   // end if

    std::string ins;
    std::locale loc;
    ins = std::tolower( c, loc);

    SetConsoleMode( hStdin, fdwSaveOldMode);
    return ins;
}   // end getWindowsConsoleInput
#endif
}   // end namespace


using Cliniface::ClinifaceApp;

ClinifaceApp::ClinifaceApp()
    : _app(nullptr), _fm(nullptr),
      _cutopt( {"c", "cut"}, tr( "Cutout and centre the face (bounds estimated).")),
      _mapopt( {"m", "map"}, tr( "Map and align the face and landmarks (after cut).")),
      _mskopt( {"k", "mask"}, tr( "Export the mask from a 3DF (or a mapped model).")),
      _repopt( {"r", "report"}, tr( "Generate report with <id> from a 3DF (or a mapped model) or list available reports if <id> invalid."), tr("id"), "-1"),
      _forceopt( {"f", "force"}, tr( "Force save (overwrite existing file)."))
{
    QStringList hlp;
    hlp << "The output filepath (optional). By default, a 3DF containing all model and metadata is exported with the file"
        << "having the same path and basename as the input file."
        << "A file without an extension is treated as a directory and the file is saved into that directory"
        << "(the directory is created if it doesn't exist) and the saved file uses the same basename as the input file."
        << "To export to a different format, give the output file one of the extensions {3df,obj,ply,stl,u3d}."
        << "Note that landmarks and other metadata are only saved in the 3DF file format."
        << "To export only the mapped mask after passing -m or if the input file is a 3DF, set the -k switch."
        << "To export only metadata, specify an output file with one of the extensions {csv,xml,json}."
        << "Exporting metadata is allowed only if the input file is a 3DF or if the -m switch was passed.";

    _parser.setApplicationDescription( QString("%1 version %2 <%3>\n%4").arg(APP_NAME).arg(APP_VERSION_STRING).arg(APP_WEBSITE).arg(APP_DESCRIPTION));
    _parser.addHelpOption();
    _parser.addVersionOption();
    _parser.addPositionalArgument("input", tr( "Filepath of the 3D image to open."), tr("input"));
    _parser.addPositionalArgument("output", tr( hlp.join(" ")), tr("[output]"));
    _parser.addOption( _cutopt);
    _parser.addOption( _mapopt);
    _parser.addOption( _mskopt);
    _parser.addOption( _repopt);
    _parser.addOption( _forceopt);
}   // end ctor


ClinifaceApp::~ClinifaceApp()
{
    if ( _fm)
    {
        FaceTools::FaceModelCurvatureStore::purge(*_fm);
        FaceTools::FaceModelSymmetryStore::purge(_fm);
        FaceTools::FaceModelDeltaStore::purge(_fm);
        FMM::close(*_fm);
        _fm = nullptr;
    }   // end if

    if ( _app)
        delete _app;
}   // end dtor


int ClinifaceApp::start( int argc, char **argv)
{
    _app = new QApplication( argc, argv);
    _parser.process(*_app);

    makeConfigDir(); // Make the .cliniface directory in the user's home directory if it doesn't already exist
    if ( !Preferences::init())
    {
        std::cerr << "Unable to initialise preferences!" << std::endl;
        return -1;
    }   // end if

    makeExeLink();

    _inpath = _getFileInfo( 0); // Get the input file
    if ( !initReports())
    {
        std::cerr << "Unable to initialise reports!" << std::endl;
        return -1;
    }   // end if

    if ( _setReport() == -1)
    {
        listReports();
        return 1;
    }   // end if

    initFileIO();

    // Test for the presence of command switches that require an input file when one is not given
    if ( !_hasInputFile() && !_parser.optionNames().isEmpty())
    {
        std::cerr << "The command switch(es) passed also require an input file to be specified!" << std::endl;
        return -1;
    }   // end if

    // If an input file was given, test that it's an allowed format
    if ( _hasInputFile() && !isValidInputFile( _inpath))
    {
        std::cerr << QString("Unable to read in '%1'!").arg( _inpath.filePath()).toStdString() << std::endl;
        return -1;
    }   // end if

    initBase(); // Landmarks and Ethnicities
    initMetrics();

    int rval = -1;
    if ( !_isCommandLineOnly())
        rval = _openGUI();
    else if ( _setOutPath())
        rval = _runCommandLine();

    return rval;
}   // end start


bool ClinifaceApp::_hasInputFile() const { return !_inpath.filePath().isEmpty();}


bool ClinifaceApp::_isCommandLineOnly() const
{
    return !( _inpath.filePath().isEmpty() || ( (_parser.positionalArguments().size() <= 1) && !doCut() && !doMap() && !doMask() && _reportId < 0));
}   // end _isCommandLineOnly


int ClinifaceApp::_setReport()
{
    _reportId = -2;

    if ( _parser.isSet( _repopt))
    {
        bool okay = false;
        const int rid = _parser.value(_repopt).toInt( &okay);
        if ( !okay || rid < 0 || rid >= int(RMAN::count()))
            _reportId = -1;
        else
            _reportId = rid;    // Set to a valid report id for export
    }   // end else if

    return _reportId;
}   // end _setReport


int ClinifaceApp::_runCommandLine()
{
    Preferences::apply();

    _fm = FMM::read( _inpath.absoluteFilePath());
    if ( !_fm)
    {
        std::cerr << QString("Unable to open '%1'; %2").arg( _inpath.filePath(), FMM::error()).toStdString() << std::endl;
        return -1;
    }   // end if

    if ( doCut() || doMap())
    {
        FaceTools::FaceModelCurvatureStore::add(*_fm);
        //FaceTools::FaceModelSymmetryStore::add(_fm);
        if ( doCut())
            cutOutFace(_fm);
        if ( doMap() && !mapFace(_fm))
        {
            std::cerr << "Failed to detect and map landmarks!" << std::endl;
            return -1;
        }   // end if
    }   // end if

    if ( _fm->hasLandmarks())
    {
        FaceTools::Action::ActionUpdateMeasurements::updateAllMeasurements(_fm);
        FaceTools::Metric::StatsManager::updateStatsForModel(*_fm);
    }   // end if

    int rval = 1;

    if ( _reportId >= 0)
    {
        if ( !exportReport( _fm, _reportId, _outpath))
        {
            std::cerr << "Failed to export report to " << _outpath.filePath().toLocal8Bit().toStdString() << std::endl;
            rval = -1;
        }   // end if
    }   // end if
    else
    {
        if ( doMask())
        {
            assert( _fm->hasMask());
            r3d::Mesh::Ptr mask = _fm->mask().deepCopy();
            _fm->setMask(nullptr);
            _fm->update( mask, true, false/*don't settle landmarks*/, 1);
        }   // end if

        if ( !write( _fm, _outpath))
        {
            std::cerr << "Failed to save to " << _outpath.filePath().toLocal8Bit().toStdString() << std::endl;
            rval = -1;
        }   // end if
    }   // end else

    return rval;
}   // end _runCommandLine


int ClinifaceApp::_openGUI()
{
    printHeader();
    checkOpenGLVersion();

    int rval = 1;
    ClinifaceMain *mainWin = nullptr;

    std::function<void()> cleanUpFn = [&mainWin](){ 
        if ( mainWin)
        {
            removeExamplesLink();
            std::cerr << "-- Cleaning up --" << std::endl;
            delete mainWin;
            mainWin = nullptr;
        }   // end if
    };  // end cleanUpFn

    /*
    // Only allow for single instances of the GUI version to force opening in existing version.
    QSharedMemory singleInstance( QString("%1_%2").arg(APP_NAME).arg(APP_VERSION_STRING));
    if ( singleInstance.attach())   // Attempt to attach to existing instance
    {
        singleInstance.lock();
        mainWin = (ClinifaceMain*)singleInstance.data();
        singleInstance.unlock();
        assert(mainWin);
        mainWin->show();
    }   // end if
    else
    {
        if ( !singleInstance.create(sizeof(ClinifaceMain)))
        {
            std::cerr << singleInstance.errorString().toStdString() << std::endl;
            rval = -1;
        }   // end if
        else
        {
            mainWin = new ClinifaceMain;
            Preferences::apply();

            singleInstance.lock();
            ClinifaceMain *to = (ClinifaceMain*)singleInstance.data();
            memcpy( to, mainWin, sizeof(ClinifaceMain));
            singleInstance.unlock();

            mainWin->show();
            if ( !_inpath.filePath().isEmpty() && !mainWin->loadModel(_inpath.absoluteFilePath()))
                std::cerr << QString("Can't open '%1'; %2").arg( _inpath.filePath(), FMM::error()).toStdString() << std::endl;

            makeExamplesLink();
            QObject::connect( qApp, &QCoreApplication::aboutToQuit, cleanUpFn);
            rval = _app->exec();
        }   // end else
    }   // end else
    singleInstance.detach();
    */

    mainWin = new ClinifaceMain;
    Preferences::apply();
    mainWin->show();
    if ( !_inpath.filePath().isEmpty() && !mainWin->loadModel(_inpath.absoluteFilePath()))
        std::cerr << QString("Can't open '%1'; %2").arg( _inpath.filePath(), FMM::error()).toStdString() << std::endl;

    makeExamplesLink();
    if ( UpdatesDialog::autoCheckUpdate())
        QTimer::singleShot( 3000, [mainWin](){ mainWin->checkForUpdate();});

    QObject::connect( qApp, &QCoreApplication::aboutToQuit, cleanUpFn);
    rval = _app->exec();
    cleanUpFn();

    return rval;
}   // end _openGUI




bool ClinifaceApp::_setOutPath()
{
    static const QSet<QString> META_FORMATS = {"csv","xml","json"};

    _outpath = _getFileInfo( 1);
    _saveMeta = false;

    const QString EXT_PDF = "pdf";
    const QString EXT_3DF = FMM::fileFormats().preferredExt().toLower();
    const QString EXT = _reportId >= 0 ? EXT_PDF : EXT_3DF;
    const bool inFileIs3DF = _inpath.suffix().toLower() == EXT_3DF;

    if ( doMask() && !inFileIs3DF && !doMap())
    {
        std::cerr << "Cannot export masks from non-3DF files, or if not mapping landmarks on the input file!" << std::endl;
        return false;
    }   // end if

    if ( doMask() && _reportId >= 0)
    {
        return false;
    }   // end if

    // If no outpath specified, the output filepath is the same as the input just with the appropriate extension.
    if ( _outpath.filePath().isEmpty())
        _outpath = QFileInfo( _inpath.path() + "/" + _inpath.completeBaseName() + "." + EXT);
    else if ( _outpath.suffix().isEmpty())
    {   // If no suffix, treat the filepath as a directory and ensure it (and all parent directories) exist before setting
        // the output filename to be inside that directory with the basename of the input file and EXT as the extension.
        const QString dirpath = _outpath.filePath();
        if ( QDir().mkpath( dirpath))
            _outpath = QFileInfo( dirpath + "/" + _inpath.completeBaseName() + "." + EXT);
        else
        {
            std::cerr << QString("Cannot create directory '%1' to write to!").arg( dirpath).toStdString() << std::endl;
            return false;
        }   // end else
    }   // end else if
    else
    {
        const QString suffix = _outpath.suffix().toLower();
   
        // Not an empty suffix, so see if it's an allowed format
        if (META_FORMATS.contains(suffix))
        {
            // Metadata output is only allowed if the input file is a 3DF, or if performing a map operation on the input model.
            if ( _inpath.suffix().toLower() == EXT_3DF || doMap())
                _saveMeta = true;
            else
            {
                std::cerr << "Cannot export metadata from a non-3DF file, or if not obtaining it from the input file!" << std::endl;
                return false;
            }   // end else
        }   // end if
        else if ( _reportId >= 0)
        {
            if ( suffix != EXT_PDF)
            {
                std::cerr << "You must specify an output filename with the extension 'pdf'!" << std::endl;
                return false;
            }   // end if
        }   // end else if
        else if ( !FMM::fileFormats().writeInterface( _outpath.absoluteFilePath()))
        {
            std::cerr << QString("File extension '%1' is not supported for writing to!").arg( suffix).toStdString() << std::endl;
            return false;
        }   // end else if

        // Ensure that the directory path exists
        const QString dirpath = _outpath.canonicalPath();
        if ( !QDir().mkpath( dirpath))
        {
            std::cerr << QString("Cannot create directory '%1' to write to!").arg( dirpath).toStdString() << std::endl;
            return false;
        }   // end if
    }   // end else

    if ( !_saveMeta && _reportId < 0 && !FMM::canWrite( _outpath.absoluteFilePath()))
    {
        std::cerr << QString("Unable to write to '%1'!").arg( _outpath.filePath()).toStdString() << std::endl;
        return false;
    }   // end if

    // If the output file will clobber an existing file, warn and fail unless force is set or user confirms
    if ( !forceWrite() && _outpath.exists())
    {
        std::cout << QString("cliniface: overwrite '%1'? ").arg(_outpath.filePath()).toLocal8Bit().toStdString();

        std::string ins;
#ifndef _WIN32
        std::getline( std::cin, ins);
#else
        ins = getWindowsConsoleInput();
#endif
        if ( ins != "y" && ins != "yes")
            return false;
    }   // end if

    return true;
}   // end _setOutPath


QFileInfo ClinifaceApp::_getFileInfo( int idx) const
{
    QFileInfo finfo;
    if ( _parser.positionalArguments().size() > idx)
    {
        QString fpath = _parser.positionalArguments()[idx];
        if ( fpath.at(0) == '~')    // Only needed for Windows
            fpath.replace("~", QDir::homePath());
        finfo = QFileInfo( fpath);
    }   // end if
    return finfo;
}   // end _getFileInfo

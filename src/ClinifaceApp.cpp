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

#include <QTemporaryFile>
#include <QSharedMemory>
#include <QTextStream>
#include <QSet>
#include <QDir>
#include <fstream>

#include <FaceTools/FaceModelCurvature.h>
#include <FaceTools/FaceModelSymmetry.h>

#include <FaceTools/Ethnicities.h>

#include <FaceTools/Action/ActionDetectFace.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/Action/ActionUpdateGrowthData.h>
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
#include <FaceTools/Metric/StatisticsManager.h>

#include <FaceTools/LndMrk/LandmarksManager.h>
#include <FaceTools/Report/ReportManager.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ClinifaceApp.h>
#include <ClinifaceMain.h>
#include <Preferences.h>

using LMAN = FaceTools::Landmark::LandmarksManager;
using RMAN = FaceTools::Report::ReportManager;
using FMM = FaceTools::FileIO::FaceModelManager;
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
    FMM::add( FaceModelAssImpFileHandlerFactory::make("3mf"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("acc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ask"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ase"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("blend"));
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


void initBaseData()
{
    const QDir appDir( QCoreApplication::applicationDirPath());
    FaceTools::Ethnicities::load( ":/data/ETHNICITIES");
    LMAN::load( ":/data/LANDMARKS");
    LMAN::loadImages( appDir.filePath( LANDMARK_IMGS_DIR));
}   // end initBaseData


void initMetricsData()
{
    const QDir appDir( QCoreApplication::applicationDirPath());
    using namespace FaceTools::Metric;
    //qInfo( "Loading metrics...");
    MetricManager::load( appDir.filePath( METRICS_DIR));
    //qInfo( "Loading statistics...");
    StatisticsManager::load( appDir.filePath( STATISTICS_DIR));
    //qInfo( "Loading TEST statistics...");
    //StatisticsManager::load( appDir.filePath( TEST_STATISTICS_DIR));
    //qInfo( "Loading phenotypes...");
    PhenotypeManager::load( appDir.filePath( HPOS_DIR));
    //qInfo( "Loading genetics...");
    GeneManager::load( ":/data/GENES");
    //qInfo( "Loading syndromes...");
    SyndromeManager::load( ":/data/SYNDROMES");
    //qInfo( "Loading reports...");
    RMAN::setLogoPath(":/logos/PDF_LOGO");
    RMAN::setReportHeaderName( APP_NAME);
    RMAN::load( appDir.filePath( REPORTS_DIR));
}   // end initMetricsData


QString removeExamplesLink()
{
    QString examplesName = "examples";
#ifdef _WIN32
    examplesName = "examples.lnk";
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
        FaceTools::FileIO::exportMetaData( fm, true/*export calliper measurement vertices*/, tree);
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
        FaceTools::FileIO::exportMetaData( fm, true/*export calliper measurement vertices*/, tree);
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
        writeOkay = FMM::write( fm, fpath);
    return writeOkay;
}   // end write




void cutOutFace( FM *fm)
{
    std::cout << "Extracting face..." << std::endl;
    r3d::Mesh::Ptr mesh = FaceTools::Action::ActionExtractFace::extract(fm);
    fm->update( mesh, true, true, 1);   // Keep one manifold
}   // end cutOutFace


void mapFace( FM *fm)
{
    std::cout << "Mapping face..." << std::endl;
    const IntSet &lmids = FaceTools::Landmark::LandmarksManager::ids();
    FaceTools::Action::ActionDetectFace::detect( fm, lmids);
}   // end mapFace


void printHeader()
{
    qInfo() << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    qInfo( " %s version %s <%s>", APP_NAME, APP_VERSION_STRING, APP_WEBSITE);
    qInfo( " Copyright %s %s & %s", APP_CR_YEARS, APP_ORGANISATION, APP_AUTHOR_NAME);
    qInfo( " Developed by %s <mailto:%s>", APP_AUTHOR_NAME, APP_CONTACT_EMAIL);
    qInfo() << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    qInfo() << "" << APP_NAME << "is free software: you can redistribute it and/or modify it";
    qInfo() << " it under the terms of the GNU General Public License as published by";
    qInfo() << " the Free Software Foundation, either version 3 of the License, or";
    qInfo() << " (at your option) any later version.";
    qInfo() << "" << APP_NAME << "is distributed in the hope that it will be useful,";
    qInfo() << " but WITHOUT ANY WARRANTY; without even the implied warranty of";
    qInfo() << " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.";
    qInfo() << " See the GNU General Public License for more details.";
    qInfo() << " You should have received a copy of the GNU General Public License";
    qInfo() << " along with this program. If not, see <http://www.gnu.org/licenses/>.";
    qInfo() << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}   // end printHeader


void makeConfigDir()
{
    // The old .cliniface file now needs to be the new .cliniface directory (if it doesn't exist as such already)
    const QFileInfo finfo( QDir::home().filePath( QString(".%1").arg(EXE_NAME)));

    // See if the old preferences file exists - we'll want to copy it over to the new directory.
    QString oldprefs;
    if ( finfo.exists() && !finfo.isDir())
    {
        QTemporaryFile tmp;
        if ( tmp.open())
            oldprefs = tmp.fileName();
    }   // end if

    // Copy $HOME/.cliniface to temporary file
    if ( !oldprefs.isEmpty())
    {
        QFile::copy( finfo.filePath(), oldprefs);
        QFile::remove( finfo.filePath());
    }   // end if

    // Make the directory if it doesn't exist already as well as the user's plugins directory
    if ( !QFile::exists(finfo.filePath()))
    {
        std::cerr << QString("Initialising %1 directory").arg(finfo.filePath()).toStdString() << std::endl;
        QDir::home().mkpath( QString(".%1").arg(EXE_NAME));
        if ( !oldprefs.isEmpty())   // Copy in the old preferences if they exist
            QFile::copy( oldprefs, QDir::home().filePath( QString(".%1/preferences").arg(EXE_NAME)));
        QDir::home().mkpath( QString(".%1/plugins").arg(EXE_NAME)); // Make the user plugins directory
    }   // end if
}   // end makeConfigDir

}   // end namespace



using Cliniface::ClinifaceApp;

ClinifaceApp::ClinifaceApp()
    : _app(nullptr), _fm(nullptr),
      _cutopt( {"c", "cut"}, tr( "Cutout and centre the face (bounds estimated).")),
      _mapopt( {"m", "map"}, tr( "Map and align the face and landmarks (after cut).")),
      _mskopt( {"k", "mask"}, tr( "Export the mask from a 3DF (or a mapped model).")),
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
    _parser.addOption( _forceopt);
}   // end ctor


ClinifaceApp::~ClinifaceApp()
{
    if ( _fm)
    {
        FaceTools::FaceModelCurvature::purge(_fm);
        FaceTools::FaceModelSymmetry::purge(_fm);
        FMM::close(_fm);
        _fm = nullptr;
    }   // end if

    if ( _app)
        delete _app;
}   // end dtor


int ClinifaceApp::start( int argc, char **argv)
{
    _app = new QApplication( argc, argv);
    //_app->setAttribute(Qt::AA_UseHighDpiPixmaps);

    _parser.process(*_app);
    makeConfigDir(); // Make the .cliniface directory in the user's home directory if it doesn't already exist
    if ( !Preferences::init())
    {
        std::cerr << "Unable to initialise preferences!" << std::endl;
        return -1;
    }   // end if

    if ( !_parser.positionalArguments().isEmpty())
        _inpath = QFileInfo( _parser.positionalArguments().first());    // May be empty

    initFileIO();

    if ( !_parser.optionNames().isEmpty() && _inpath.filePath().isEmpty())
    {
        std::cerr << "Command switches are not accepted without also specifying an input file!" << std::endl;
        return -1;
    }   // end if

    int rval = -1;
    if ( _inpath.filePath().isEmpty() || ( (_parser.positionalArguments().size() <= 1) && !doCut() && !doMap() && !doMask()))
    {
#ifdef _WIN32
        std::cout << std::endl;
#endif
        rval = _openGUI();
    }   // end if
    else if ( _setOutPath())
        rval = _runCommandLine();

    return rval;
}   // end start


int ClinifaceApp::_runCommandLine()
{
    initBaseData(); // Landmarks and Ethnicities
    if ( doCut() || doMap() || doMask() || _saveMeta)
        initMetricsData();

    Preferences::apply();

    _fm = FMM::read( _inpath.absoluteFilePath());
    if ( !_fm)
    {
        std::cerr << QString("Unable to open '%1'; %2").arg( _inpath.filePath(), FMM::error()).toStdString() << std::endl;
        return -1;
    }   // end if

    if ( doCut() || doMap())
    {
        FaceTools::FaceModelCurvature::add(_fm);
        //FaceTools::FaceModelSymmetry::add(_fm);
        if ( doCut())
            cutOutFace(_fm);
        if ( doMap())
            mapFace(_fm);
    }   // end if

    if ( _fm->hasLandmarks())
    {
        FaceTools::Action::ActionUpdateMeasurements::updateAllMeasurements(_fm);
        FaceTools::Action::ActionUpdateGrowthData::setStatsToModel(_fm);
    }   // end if

    int rval = 1;
    if ( doMask())
    {
        assert( _fm->hasMask());
        r3d::Mesh::Ptr mask = _fm->mask();
        _fm->setMask(nullptr);
        _fm->update( mask, true, false/*don't settle landmarks*/, 1);
    }   // end if

    if ( !write( _fm, _outpath))
    {
        std::cerr << "Failed to save to " << _outpath.filePath().toLocal8Bit().toStdString() << std::endl;
        rval = -1;
    }   // end if

    return rval;
}   // end _runCommandLine


int ClinifaceApp::_openGUI()
{
    printHeader();
    initBaseData(); // Landmarks and Ethnicities
    initMetricsData();

    int rval = 1;
    ClinifaceMain *mainWin = nullptr;

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
            rval = _app->exec();
            removeExamplesLink();
            std::cerr << "-- Cleaning up --" << std::endl;
            delete mainWin;
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
    rval = _app->exec();
    removeExamplesLink();
    std::cerr << "-- Cleaning up --" << std::endl;
    delete mainWin;

    return rval;
}   // end _openGUI


#ifdef _WIN32
namespace {
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
}   // end namespace
#endif


bool ClinifaceApp::_setOutPath()
{
    static const QSet<QString> META_FORMATS = {"csv","xml","json"};

    if ( _parser.positionalArguments().size() > 1)
        _outpath = QFileInfo( _parser.positionalArguments()[1]);    // May be empty

    _saveMeta = false;
    const QString EXT = FMM::fileFormats().preferredExt().toLower();
    const bool inFileIs3DF = _inpath.suffix().toLower() == "3df";

    if ( doMask() && !inFileIs3DF && !doMap())
    {
        std::cerr << "Cannot export masks from non-3DF files, or if not mapping landmarks on the input file!" << std::endl;
        return false;
    }   // end if

    // If no outpath specified, the output filepath is the same as the input just with the relevant extension.
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
    {   // Not an empty suffix, so see if it's an allowed format
        const QString suffix = _outpath.suffix().toLower();
        if (META_FORMATS.contains(suffix))
        {
            // Metadata output is only allowed if the input file is a 3DF, or if performing a map operation on the input model.
            if ( _inpath.suffix().toLower() == EXT || doMap())
                _saveMeta = true;
            else
            {
                std::cerr << "Cannot export metadata from a non-3DF file, or if not obtaining it from the input file!" << std::endl;
                return false;
            }   // end else
        }   // end if
        else if ( !FMM::fileFormats().writeInterface( _outpath.absoluteFilePath()))
        {
            std::cerr << QString("File extension '%1' is not supported for writing to!").arg( suffix).toStdString() << std::endl;
            return false;
        }   // end else if
    }   // end else

    if ( !_saveMeta && !FMM::canWrite( _outpath.absoluteFilePath()))
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

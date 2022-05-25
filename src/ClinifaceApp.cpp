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

#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include <QTemporaryFile>
#include <QSharedMemory>
#include <QTextStream>
#include <QSet>
#include <fstream>
#include <functional>

#include <FaceTools/FaceModelCurvatureStore.h>
#include <FaceTools/FaceModelSymmetryStore.h>
#include <FaceTools/FaceModelDeltaStore.h>

#include <FaceTools/Ethnicities.h>

#include <FaceTools/Action/ActionDetectFace.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/Action/ActionUpdateMeasurements.h>
#include <FaceTools/Action/ActionCopyAllPaths.h>

#include <FaceTools/Detect/FeaturesDetector.h>

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
#include <UpdatesDialog.h>
#include <ImageBrowser.h>
#include <Preferences.h>

using LMAN = FaceTools::Landmark::LandmarksManager;
using RMAN = FaceTools::Report::ReportManager;
using FMM = FaceTools::FileIO::FaceModelManager;
using MM = FaceTools::Metric::MetricManager;
using FaceTools::FM;

namespace {

QString tr( const QString& m) { return QObject::tr( m.toStdString().c_str());}


bool openFileStream( const QString &fpath, std::ofstream &ofs)
{
    ofs.open( fpath.toStdString());
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


bool closeModel( FM *fm, bool save=false)
{
    FaceTools::FaceModelCurvatureStore::purge(*fm);
    FaceTools::FaceModelSymmetryStore::purge(fm);
    FaceTools::FaceModelDeltaStore::purge(fm);
    bool okay = true;
    QString fpath;
    if (save && !FMM::write( *fm, fpath))
    {
        std::cerr << tr("Failed to save to '%1'!\n").arg(fpath).toStdString();
        okay = false;
    }   // end if
    FMM::close(*fm);
    return okay;
}   // end closeModel


FM *openModel( const QString &fpath)
{
    FM *fm = FMM::read( fpath);
    if ( !fm)
        std::cerr << tr("Unable to open '%1'; %2\n").arg( fpath, FMM::error()).toStdString();
    return fm;
}   // end openModel


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

    // Configure updates and set whether to load examples
    using namespace Cliniface;
    const Options &opts = Preferences::options(); // Read options
    UpdatesDialog::setAutoCheckUpdate( opts.checkUpdate());
    UpdatesDialog::setPatchURL( opts.patchURL());
    ImageBrowser::setParseExamples( opts.parseExamples());
}   // end initFileIO


void initBase()
{
    FaceTools::Ethnicities::load( ":/data/ETHNICITIES");
    LMAN::load( ":/data/LANDMARKS");
    const QDir appDir( QCoreApplication::applicationDirPath());
    LMAN::loadImages( appDir.filePath( LANDMARK_IMGS_DIR));
    QTools::FileIO::APP_IMAGE_TOOL = appDir.filePath( APPIMAGETOOL_NAME);
    QTools::FileIO::UPDATE_TOOL = appDir.filePath( FILE_UPDATE_TOOL_NAME);
}   // end initBase


bool initReports()
{
    FM::LENGTH_UNITS = "mm";
    const Cliniface::Options &opts = Cliniface::Preferences::options();
    const std::string haarModels = opts.haarModels().toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << "[WARN] Cliniface cannot initialise face detector!\n";

    RMAN::init( opts.pdflatex(), opts.idtfConv());
    using FaceTools::Report::Report;
    Report::setInkscape( opts.inkscape());
    Report::setDefaultPageDims( opts.pageDims());
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
        std::cout << std::setw(3) << id << "| " << rname.toStdString() << std::endl;
        id++;
    }   // end for
}   // end listReports


bool isValidInputFile( const QFileInfo &infile)
{
    return infile.exists() && FMM::canRead( infile.absoluteFilePath());
}   // isValidInputFile


// Make shortcut/symlink from examples directory from installation/mount dir to the user's .cliniface
// directory. Do this every time because on Linux, the mount point changes with each execution (since
// Cliniface is run from AppImage).
void makeExamplesLink()
{
    const QString linkTarget = Cliniface::Options::exampleImagesDirRaw();
    QFile::remove( linkTarget);
    QFile::link( QDir( QCoreApplication::applicationDirPath()).filePath( EXAMPLES_DIR), linkTarget);
}   // end makeExamplesLink


bool exportReport( const FM *fm, int repId, const QFileInfo &finfo)
{
    assert( finfo.suffix().toLower() == "pdf");
    const QString repName = RMAN::names().at(repId);
    std::cout << "Generating report '" << repName.toStdString() << "'; please wait..." << std::endl;
    FaceTools::Report::Report::Ptr report = RMAN::report( repName);
    assert( report);
    if ( !FaceTools::U3DCache::refresh( *fm))
    {
        std::cerr << "Unable to cache U3D model!\n";
        return false;
    }   // end if

    if ( !report->setContent())
    {
        std::cerr << "Unable to set report content!\n";
        return false;
    }   // end if

    if ( !report->generate())
    {
        std::cerr << "Unable to generate PDF!\n";
        return false;
    }   // end if

    const QString abspath = finfo.absoluteFilePath();
    const std::string sabspath = abspath.toStdString();
    if (!QFile::copy( report->pdffile(), abspath))
    {
        std::cerr << "Unable to copy generated report to '" << sabspath << "'\n";
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


bool copyMeasuresFromSourceToTarget( const FM *sfm, FM *tfm)
{
    assert( sfm);
    bool copyOkay = false;
    if ( FaceTools::Action::ActionCopyAllPaths::canCopy( sfm, tfm))
        copyOkay = FaceTools::Action::ActionCopyAllPaths::copy( *sfm, *tfm) > 0;
    return copyOkay;
}   // end copyMeasuresFromSourceToTarget


// Batch copy user measurements from sfm to 3DF files in tgtDir
bool copyMeasuresFromSourceToTargetDir( const FM *sfm, const QDir &tgtDir)
{
    assert( sfm);
    const QString EXT_3DF = FMM::fileFormats().preferredExt().toLower();
    QStringList copyOkay, copyFail;
    QFileInfoList tfiles = tgtDir.entryInfoList( {"*." + EXT_3DF}, QDir::Files | QDir::Readable | QDir::Writable);
    tfiles.removeAll( QFileInfo( FMM::filepath( *sfm)));   // Remove possibility of clobbering source!
    std::cout << tr("Found %1 writable %2 files to copy user measurements to...\n").arg(tfiles.size()).arg(EXT_3DF).toStdString();
    for ( const QFileInfo &finfo : tfiles)
    {
        const QString fpath = finfo.filePath();
        FM *tfm = openModel( fpath);
        if ( !tfm)
        {
            std::cout << tr("Unable to open model '%1'!\n").arg(fpath).toStdString();
            copyFail += fpath;
            continue;
        }   // end if

        if ( copyMeasuresFromSourceToTarget( sfm, tfm))
        {
            if ( closeModel(tfm, true))
            {
                std::cout << tr("Copied user measurements to '%1'\n").arg(fpath).toStdString();
                copyOkay += fpath;
            }   // end if
            else
                copyFail += fpath;
        }   // end if
        else
        {
            std::cerr << tr("Failed to copy user measurements to '%1'!\n").arg(fpath).toStdString();
            closeModel(tfm);
            copyFail += fpath;
        }   // end else
    }   // end for

    //std::cout << tr("Copied user measurements to %1 3DF files in %2\n").arg(copyOkay.size()).arg(tgtDir.path()).toStdString();
    return !copyOkay.isEmpty();
}   // end copyMeasuresFromSourceToTargetDir


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
    if ( finfo.exists() && !finfo.isDir())  // If .cliniface exists as a file (old versions), remove it
        QFile::remove( finfo.filePath());
    // Make the directory if it doesn't exist already
    if ( !finfo.exists())
    {
        std::cerr << tr("Initialising directory %1\n").arg(finfo.filePath()).toStdString();
        QDir::home().mkpath( finfo.filePath());
    }   // end if

    // Silently make the user's plugins directory if it doesn't already exist
    const QFileInfo pdir( finfo.filePath() + "/plugins");
    if ( !pdir.exists())
        QDir::home().mkpath( pdir.filePath());
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
    : _app(nullptr), _fm(nullptr), _mainWin(nullptr),
      _copyUserMeasures( {"c", "copy"}, tr( "Copy user measurements from a source 3DF to a target 3DF (or directory of 3DFs).")),
      _cutopt( {"s", "slice"}, tr( "Cut out and retain the centre of the face by estimating facial bounds.")),
      _mapopt( {"m", "map"}, tr( "Map and align the face and landmarks (after cutting out the face centre).")),
      _mskopt( {"k", "mask"}, tr( "Export the coregistered anthropometric mask from a 3DF (or a mapped model).")),
      _repopt( {"r", "report"}, tr( "Generate a report with <id> from a 3DF (or a mapped model) or list available reports if <id> invalid."), tr("id"), "-1"),
      _forceopt( {"f", "force"}, tr( "Force save (overwrite existing file)."))
{
    QStringList hlp;
    hlp << "The output filepath or target directory. By default, a 3DF is saved with"
        << "the same path and basename as the input file."
        << "A file with no extension is treated as a directory. For default output, the file is saved into that directory"
        << "(being created if it doesn't already exist) and the output file is exported in the same format as the input."
        << "Export to a different format by specifying one of the extensions {3df,obj,ply,stl,u3d}."
        << "Note that landmarks and other metadata are only saved in the 3DF format."
        << "\n----------------------------------------------------------\n"
        << "Exporting the anthropometric mask (-k, --mask):\n"
        << "Export the anthropometric mask after mapping (passing -m) or if the input file is a 3DF."
        << "\n----------------------------------------------------------\n"
        << "Exporting metadata:\n"
        << "Export only metadata by specifying an output file with one of the metadata extensions {csv,xml,json}."
        << "Metadata export is allowed only if the input file is a 3DF or if the -m switch was passed."
        << "\n----------------------------------------------------------\n"
        << "Copying user measurements (-c, --copy):\n"
        << "Copy user measurements from a source input 3DF by specifying a target (output) 3DF."
        << "Measurements from the source file are barycentrically mapped to the other 3DF so the files"
        << "must both already be coregistered against the same underlying anthropometric mask."
        << "If the target is a directory, it will be searched for 3DFs and measurements from the"
        << "source 3DF will be copied in batch to the 3DFs within the top level of that directory.";

    _parser.setApplicationDescription( QString("%1 version %2 <%3>\n%4").arg(APP_NAME).arg(APP_VERSION_STRING).arg(APP_WEBSITE).arg(APP_DESCRIPTION));
    _parser.addHelpOption();
    _parser.addVersionOption();
    _parser.addPositionalArgument("input", tr( "Filepath of the 3D image to open."), tr("input"));
    _parser.addPositionalArgument("target", tr( hlp.join(" ")), tr("[target]"));
    _parser.addOption( _copyUserMeasures);
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
        closeModel(_fm, false);
        _fm = nullptr;
    }   // end if

    if ( _app)
        delete _app;
}   // end dtor


int ClinifaceApp::start( int argc, char **argv)
{
    _app = new QApplication( argc, argv);
    QApplication::setStyle( QStyleFactory::create("Fusion"));   // Was before QApplication in 6.0.4

    std::function<void()> cleanUpFn = [this](){ 
        std::cerr << "-- Cleaning up --\n";
        QFile::remove( Options::exampleImagesDir());
        if ( _mainWin)
        {
            delete _mainWin;
            _mainWin = nullptr;
        }   // end if
    };  // end cleanUpFn
    QObject::connect( qApp, &QCoreApplication::aboutToQuit, cleanUpFn);

    _parser.process(*_app);

    makeConfigDir(); // Make the .cliniface directory in the user's home directory if it doesn't already exist
    makeExamplesLink();

    if ( !Preferences::init())  // Only reads in config - does not apply
    {
        std::cerr << "Unable to initialise preferences!\n";
        return -1;
    }   // end if

    makeExeLink();

    _inpath = _getFileInfo( 0); // Get the input file
    if ( !initReports())
    {
        std::cerr << "Unable to initialise reports!\n";
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
        std::cerr << "The command switch(es) passed also require an input file to be specified!\n";
        return -1;
    }   // end if

    // If an input file was given, test that it's an allowed format
    if ( _hasInputFile() && !isValidInputFile( _inpath))
    {
        std::cerr << tr("Unable to read in '%1'!\n").arg( _inpath.filePath()).toStdString();
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
    return !( _inpath.filePath().isEmpty()
              || ((_parser.positionalArguments().size() <= 1)
                  && !doCopyMeasures() && !doCropFace() && !doMap() && !doExportMask() && (_reportId < 0))
              );
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

    _fm = openModel( _inpath.absoluteFilePath());
    if ( !_fm)
        return -1;

    int rval = 1;

    if ( doCopyMeasures())
    {
        if ( !_outpath.suffix().isEmpty())
        {
            FM *tfm = openModel( _outpath.absoluteFilePath());
            if ( !tfm)
                return -1;
            rval = copyMeasuresFromSourceToTarget( _fm, tfm) ? 1 : -1;
            if ( !closeModel(tfm, rval == 1))  // Try to save on success then close
                rval = -1;  // Failed to save
        }   // end if
        else
            rval = copyMeasuresFromSourceToTargetDir( _fm, _tgtDir) ? 1 : -1;
        return rval;
    }   // end if

    if ( doCropFace() || doMap())
    {
        FaceTools::FaceModelCurvatureStore::add(*_fm);
        //FaceTools::FaceModelSymmetryStore::add(_fm);
        if ( doCropFace())
            cutOutFace(_fm);
        if ( doMap() && !mapFace(_fm))
        {
            std::cerr << "Failed to detect and map landmarks!\n";
            return -1;
        }   // end if
    }   // end if

    if ( _fm->hasLandmarks())
    {
        FaceTools::Action::ActionUpdateMeasurements::updateAllMeasurements(_fm);
        FaceTools::Metric::StatsManager::updateStatsForModel(*_fm);
    }   // end if

    if ( _reportId >= 0)
    {
        if ( !exportReport( _fm, _reportId, _outpath))
        {
            std::cerr << tr("Failed to export report to '%1!'\n").arg(_outpath.filePath()).toStdString();
            rval = -1;
        }   // end if
    }   // end if
    else
    {
        if ( doExportMask())
        {
            assert( _fm->hasMask());
            r3d::Mesh::Ptr mask = _fm->mask().deepCopy();
            _fm->setMask(nullptr);
            _fm->update( mask, true, false/*don't settle landmarks*/, 1);
        }   // end if

        if ( !write( _fm, _outpath))
        {
            std::cerr << tr("Failed to save to '%1'!\n").arg(_outpath.filePath()).toStdString();
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

    /*
    // Only allow for single instances of the GUI version to force opening in existing version.
    QSharedMemory singleInstance( QString("%1_%2").arg(APP_NAME).arg(APP_VERSION_STRING));
    if ( singleInstance.attach())   // Attempt to attach to existing instance
    {
        singleInstance.lock();
        _mainWin = (ClinifaceMain*)singleInstance.data();
        singleInstance.unlock();
        assert(_mainWin);
        _mainWin->show();
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
            _mainWin = new ClinifaceMain;
            singleInstance.lock();
            ClinifaceMain *to = (ClinifaceMain*)singleInstance.data();
            memcpy( to, _mainWin, sizeof(ClinifaceMain));
            singleInstance.unlock();

            _mainWin->show();
            if ( !_inpath.filePath().isEmpty() && !_mainWin->loadModel(_inpath.absoluteFilePath()))
                std::cerr << tr("Can't open '%1'; %2\n").arg( _inpath.filePath(), FMM::error()).toStdString();

            QObject::connect( qApp, &QCoreApplication::aboutToQuit, cleanUpFn);
            rval = _app->exec();
        }   // end else
    }   // end else
    singleInstance.detach();
    */

    _mainWin = new ClinifaceMain;
    Preferences::apply();
    _mainWin->show();
    _mainWin->initImageBrowser();
    if ( !_inpath.filePath().isEmpty() && !_mainWin->loadModel(_inpath.absoluteFilePath()))
        std::cerr << tr("Can't open '%1'; %2\n").arg( _inpath.filePath(), FMM::error()).toStdString();
    rval = _app->exec();
    return rval;
}   // end _openGUI



bool ClinifaceApp::_setOutPath()
{
    static const QSet<QString> META_FORMATS = {"csv","xml","json"};

    _saveMeta = false;
    _outpath = _getFileInfo( 1);
    //std::cerr << tr("_outpath = '%1'\n").arg(_outpath.filePath()).toStdString(); // DEBUG

    const QString EXT_PDF = "pdf";
    const QString EXT_3DF = FMM::fileFormats().preferredExt().toLower();
    const QString EXT = _reportId >= 0 ? EXT_PDF : EXT_3DF;
    const bool inFileIs3DF = _inpath.suffix().toLower() == EXT_3DF;
    const bool outFileIs3DF = _outpath.suffix().toLower() == EXT_3DF;

    if ( (doCopyMeasures() || doExportMask()) && !inFileIs3DF && !doMap())
    {
        std::cerr << "Input file must be a 3DF, or pass the -m switch to detect and map the face.\n";
        return false;
    }   // end if

    if ( (doCopyMeasures() || doExportMask()) && _reportId >= 0)
    {
        std::cerr << "Providing a report ID is invalid with these options.\n";
        return false;
    }   // end if

    // If no outpath specified, the output filepath is the same as the input just with the appropriate extension
    // except in the case of user measurement copy which requires explicit naming of a target.
    if ( _outpath.filePath().isEmpty())
    {
        if ( doCopyMeasures())
        {
            std::cerr << "A target 3DF or directory must be given!\n";
            return false;
        }   // end if

        _outpath = QFileInfo( _inpath.path() + "/" + _inpath.completeBaseName() + "." + EXT);
    }   // end if
    else if ( _outpath.suffix().isEmpty())  // If no suffix, treat as a directory
    {
        _tgtDir = QDir( _outpath.filePath());

        // If copying measurements, this directory must already exist since it'll be searched later for 3DFs to copy to.
        if ( doCopyMeasures())
        {
            if (!_tgtDir.exists())
            {
                std::cerr << "Target directory does not exist!\n";
                return false;
            }   // end if
            else if ( !inFileIs3DF)
            {
                std::cerr << "Input file must be a 3DF!\n";
                return false;
            }   // end else if
        }   // end if
        else if ( QDir().mkpath( _tgtDir.path()))
            _outpath = QFileInfo( _tgtDir.path() + "/" + _inpath.completeBaseName() + "." + EXT);
        else
        {
            std::cerr << tr("Cannot create directory '%1' to write to!\n").arg( _tgtDir.path()).toStdString();
            return false;
        }   // end else
    }   // end else if
    else
    {
        const QString suffix = _outpath.suffix().toLower();
   
        // Not an empty suffix, so see if it's an allowed format
        if (META_FORMATS.contains(suffix))
        {
            // Metadata output is only allowed if the input file is a 3DF, or if first mapping the input model.
            if ( inFileIs3DF || doMap())
                _saveMeta = true;
            else
            {
                std::cerr << "Cannot export metadata from a non-3DF file, or if not obtaining it from the input file!\n";
                return false;
            }   // end else
        }   // end if
        else if ( _reportId >= 0)
        {
            if ( suffix != EXT_PDF)
            {
                std::cerr << "You must specify an output filename with the extension 'pdf'!\n";
                return false;
            }   // end if
        }   // end else if
        else if ( doCopyMeasures())
        {
            if ( !outFileIs3DF || !inFileIs3DF)
            {
                std::cerr << "Input and output files must be 3DFs (or the output target must be an extant directory containing 3DFs).\n";
                return false;
            }   // end if
        }   // end else if
        else if ( !FMM::fileFormats().writeInterface( _outpath.absoluteFilePath()))
        {
            std::cerr << tr("File extension '%1' is not supported for writing to!\n").arg( suffix).toStdString();
            return false;
        }   // end else if

        // Ensure that the directory path exists for the output filepath
        const QString dirpath = _outpath.canonicalPath();
        if ( !QDir().mkpath( dirpath))
        {
            std::cerr << tr("Cannot create directory '%1' to write to!\n").arg( dirpath).toStdString();
            return false;
        }   // end if
    }   // end else

    // Test if can do a general export
    if ( !doCopyMeasures() && !_saveMeta && _reportId < 0 && !FMM::canWrite( _outpath.absoluteFilePath()))
    {
        std::cerr << tr("Unable to write to '%1'!\n").arg( _outpath.filePath()).toStdString();
        return false;
    }   // end if

    // If the output file will clobber an existing file, warn and fail unless force is set or user confirms
    if ( !doCopyMeasures() && !forceWrite() && _outpath.exists())
    {
        std::cout << QString("cliniface: overwrite '%1'? ").arg(_outpath.filePath()).toStdString();

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

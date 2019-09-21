/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
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

#include <QApplication>
#include <QStyleFactory>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDir>
#include <QSurfaceFormat>
#include <QFileInfo>
#include <sstream>
#include <iomanip>

#include <FaceTypes.h>
#include <Ethnicities.h>

#include <Action/ActionDetectFace.h>
#include <Action/ActionGetFaceManifold.h>

#include <FileIO/FaceModelManager.h>
#include <FileIO/FaceModelAssImpFileHandlerFactory.h>
#include <FileIO/FaceModelOBJFileHandler.h>
#include <FileIO/FaceModelPLYFileHandler.h>
#include <FileIO/FaceModelU3DFileHandler.h>
#include <FileIO/FaceModelXMLFileHandler.h>

#include <Metric/GeneManager.h>
#include <Metric/PhenotypeManager.h>
#include <Metric/SyndromeManager.h>
#include <Metric/MetricCalculator.h>
#include <Metric/MetricCalculatorManager.h>
#include <Metric/MetricCalculatorTypeRegistry.h>
#include <Metric/CircularityMetricCalculatorType.h>
#include <Metric/CurvatureMetricCalculatorType.h>
#include <Metric/DistanceMetricCalculatorType.h>

#include <LndMrk/LandmarksManager.h>

#include <Report/ReportManager.h>

#include <boost/filesystem.hpp>
#include "ClinifaceMain.h"
#include "Preferences.h"
using FaceTools::FM;


void initFileIO()
{
    using namespace FaceTools::FileIO;
    FMM::add( new FaceModelXMLFileHandler);    // Default (preferred)
    FMM::add( new FaceModelOBJFileHandler);
    FMM::add( new FaceModelPLYFileHandler);
    FMM::add( new FaceModelU3DFileHandler);
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ask"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ase"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("fbx"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("dxf"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3s"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3o"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("acc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifczip"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("raw"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("sib"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("nff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("enff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("xgl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("zgl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ter"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("mot"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lws"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lwo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lxo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("stl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("blend"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ms3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ndo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("off"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("vta"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("smd"));
}   // end initFileIO


void initData()
{
    using namespace FaceTools::Metric;
    MetricCalculatorTypeRegistry::addMCT( new DistanceMetricCalculatorType);
    MetricCalculatorTypeRegistry::addMCT( new CircularityMetricCalculatorType);
    MetricCalculatorTypeRegistry::addMCT( new CurvatureMetricCalculatorType);
    MetricCalculator::CUSTOM_STATS_REF = "Cliniface (2019)";

    qInfo( "Loading ethnicities...");
    FaceTools::Ethnicities::load( ":/data/ETHNICITIES");

    qInfo( "Loading landmarks...");
    FaceTools::Landmark::LandmarksManager::load( ":/data/LANDMARKS");

    qInfo( "Loading metrics...");
    MetricCalculatorManager::load( QDir( QApplication::applicationDirPath()).filePath( METRICS_DIR));

    qInfo( "Loading phenotypes...");
    PhenotypeManager::load( QDir( QApplication::applicationDirPath()).filePath( HPOS_DIR));

    qInfo( "Loading genetics...");
    GeneManager::load( ":/data/GENES");

    qInfo( "Loading syndromes...");
    SyndromeManager::load( ":/data/SYNDROMES");

    qInfo( "Loading reports...");
    using FaceTools::Report::ReportManager;
    ReportManager::setLogoPath(":/logos/PDF_LOGO");
    ReportManager::setReportHeaderName( APP_NAME);
    ReportManager::load( QDir( QApplication::applicationDirPath()).filePath( REPORTS_DIR));
}   // end initData


void printHeader()
{
    qInfo() << "======================================================================";
    qInfo( " %s %s <%s>", APP_NAME, APP_VERSION_STRING, APP_WEBSITE);
    qInfo() << " Copyright 2018/2019" << APP_ORGANISATION;
    qInfo() << " Developed by" << APP_AUTHOR_NAME;
    qInfo() << "----------------------------------------------------------------------";
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
    qInfo() << "======================================================================";
}   // end printHeader


FM* loadModel( const QString& fname)
{
    FM* fm = FaceTools::FileIO::FMM::read( fname.toStdString());
    if ( !fm)
    {
        const QString errstr = FaceTools::FileIO::FMM::error().c_str();
        qWarning() << QString("Unable to open '%1'; %2").arg(fname, errstr);
    }   // end if
    return fm;
}   // end loadModel


bool exportTo3DF( FM *fm, const std::string& exdir)
{
    using namespace boost::filesystem;
    path filepath( FaceTools::FileIO::FMM::filepath(fm)); // Existing filepath of model
    const std::string dsuff = FaceTools::FileIO::FMM::fileFormats().preferredExt().toStdString(); // 3DF
    filepath.replace_extension( dsuff);   // Now with .3df as the extension

    if ( !exdir.empty())
        filepath = path(exdir) / filepath.filename();

    std::string filename = filepath.string();
    return FaceTools::FileIO::FMM::write( fm, &filename);
}   // end exportTo3DF


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


void makeHomeClinifaceDir()
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

    // Make a shortcut/symlink from the examples directory from the installation/mount directory
    // to the user's .cliniface directory. We do this every time because on Linux, the mount point
    // changes with each execution (since it's run from AppImage).
    const QString linkTarget = removeExamplesLink();
    QFile::link( QDir( QApplication::applicationDirPath()).filePath( EXAMPLES_DIR), linkTarget);
}   // end makeHomeClinifaceDir


int main( int argc, char* argv[])
{
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        FILE *stream;
        freopen_s( &stream, "CONOUT$", "w", stdout);
        freopen_s( &stream, "CONOUT$", "w", stderr);
        std::cerr << std::endl;
    }   // end if
#endif

    QSurfaceFormat fmt = QVTKOpenGLWidget::defaultFormat();
    fmt.setSamples(0);  // Needed to allow FXAA to work properly!
    QSurfaceFormat::setDefaultFormat( fmt);
    //QSurfaceFormat::setDefaultFormat( QVTKOpenGLWidget::defaultFormat());
    //vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
#ifdef NDEBUG   // Prevent any VTK warning/error pop-ups for release build
    vtkObject::GlobalWarningDisplayOff();
#endif

    qRegisterMetaType<FaceTools::Action::Event>("Event");

    Q_INIT_RESOURCE(resources);
    //QApplication::setStyle( QStyleFactory::create("Fusion"));

    QApplication app( argc, argv);
    QCoreApplication::setApplicationName( APP_NAME);
    QCoreApplication::setOrganizationName( APP_ORGANISATION);
    QCoreApplication::setApplicationVersion( APP_VERSION_STRING);

    QCommandLineParser parser;
    parser.setApplicationDescription( APP_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("[filename1, filename2, ...]", QObject::tr( "The model file(s) to open separated by spaces."));
    QCommandLineOption detectLandmarksOption( {"l", "landmarks"}, QObject::tr( "Detect landmarks and export."));
    QCommandLineOption removeNonFaceOption( {"m", "manifold"}, QObject::tr( "Remove non-face manifolds and export."));
    QCommandLineOption exportDirOption( {"t", "target-directory"},
            QObject::tr( "The <directory> into which 3DF format files are exported (defaults to input file directory)."),
            QObject::tr( "directory"));
    parser.addOption( detectLandmarksOption);
    parser.addOption( removeNonFaceOption);
    parser.addOption( exportDirOption);

    parser.process(app);

    const bool testDetectLandmarks = parser.isSet( detectLandmarksOption);
    const bool testRemoveNonFace = parser.isSet( removeNonFaceOption);
    const bool testExport = parser.isSet( exportDirOption);
    QString exportDir = parser.value( exportDirOption);
    const QStringList filenames = parser.positionalArguments();

    if ( !exportDir.isEmpty() && !boost::filesystem::is_directory( exportDir.toStdString()))
    {
        exportDir = "";
        qWarning() << "Invalid export directory! Will export to save directory as original files.";
    }   // end if

    const bool doOpenGUI = !testDetectLandmarks && !testRemoveNonFace && !testExport;
    if ( doOpenGUI)
        printHeader();

    // Make the .cliniface directory in the user's home directory if it doesn't already exist
    makeHomeClinifaceDir();

    if ( !Cliniface::Preferences::get())  // Initialises preferences
    {
        std::cerr << "ERROR! Unable to initialise preferences! Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }   // end if

    initFileIO();
    initData();

    int rval = 0;
    if ( !doOpenGUI)
    {
        for ( const QString& fname : filenames)
        {
            FM *fm = loadModel(fname);
            if ( !fm)
                continue;

            // Force (re)detection of landmarks on this face?
            if ( testDetectLandmarks)
            {
                const QString errStr = FaceTools::Action::ActionDetectFace::redetectLandmarks( fm).c_str();
                if ( !errStr.isEmpty())
                    qWarning() << "Unable to detect landmarks:" << errStr;
            }   // end if

            // Remove non-face manifolds?
            if ( testRemoveNonFace)
            {
                // Can only perform if facial landmarks present
                if ( !FaceTools::Action::ActionGetFaceManifold::removeNonFaceManifolds(fm))
                    qWarning() << "Facial landmarks missing; cannot remove non-face manifolds!";
            }   // end if

            exportTo3DF( fm, exportDir.toStdString());

            FaceTools::FileIO::FMM::close(fm);
        }   // end if
    }   // end if
    else
    {
        Cliniface::ClinifaceMain* mainWin = new Cliniface::ClinifaceMain;
        for ( const QString& fname : filenames)
            mainWin->loadModel(fname);

        mainWin->show();
        rval = app.exec();
        std::cerr << "-- Cleaning up --" << std::endl;
        delete mainWin;
    }   // end else

    removeExamplesLink();
    std::cerr << "-- Exiting --" << std::endl;
#if _WIN32
    FreeConsole();
#endif

    return rval;
}   // end main

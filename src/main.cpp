/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDir>
#include <sstream>
#include <iomanip>
#include <FaceTypes.h>
#include <FaceModelManager.h>
#include <FaceModelAssImpFileHandlerFactory.h>
#include <FaceModelU3DFileHandler.h>
#include <FaceModelOBJFileHandler.h>
#include <FaceModelXMLFileHandler.h>

#include <ActionGetComponent.h>
#include <ActionDetectFace.h>

#include <GeneManager.h>
#include <LandmarksManager.h>
#include <PhenotypeManager.h>
#include <ReportManager.h>
#include <SyndromeManager.h>
#include <MetricCalculatorManager.h>
#include <MetricCalculatorTypeRegistry.h>
#include <CircularityMetricCalculatorType.h>
#include <CurvatureMetricCalculatorType.h>
#include <DistanceMetricCalculatorType.h>

#include <boost/filesystem.hpp>
#include "ClinifaceMain.h"
#include "Preferences.h"
using FaceTools::FM;


void initFileIO()
{
    using namespace FaceTools::FileIO;
    FMM::add( new FaceModelXMLFileHandler);    // Default (preferred)
    FMM::add( new FaceModelOBJFileHandler);
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
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ply"));
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

    qInfo( "Loading landmarks...");
    FaceTools::Landmark::LandmarksManager::load( QDir( QApplication::applicationDirPath()).filePath( LANDMARKS_FILE).toStdString());

    qInfo( "Loading metrics...");
    MetricCalculatorManager::load( QDir( QApplication::applicationDirPath()).filePath( METRICS_DIR));

    qInfo( "Loading phenotypes...");
    PhenotypeManager::load( QDir( QApplication::applicationDirPath()).filePath( HPOS_DIR));

    qInfo( "Loading genetics...");
    GeneManager::load( QDir( QApplication::applicationDirPath()).filePath( GENES_FILE));

    qInfo( "Loading syndromes...");
    SyndromeManager::load( QDir( QApplication::applicationDirPath()).filePath( SYNDROMES_FILE));

    qInfo( "Loading reports...");
    using FaceTools::Report::ReportManager;
    ReportManager::setLogoPath(":/logos/PDF_LOGO");
    ReportManager::setReportHeaderName( APP_NAME);
    ReportManager::load( QDir( QApplication::applicationDirPath()).filePath( REPORTS_DIR));
}   // end initData


void loadPreferences()
{
    // Get the location of the preferences file in the user's home directory
    const QString homedir = QStandardPaths::locate( QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory);
    const QString configfile = QDir( homedir).filePath( QString(".%1").arg(EXE_NAME));
    if ( !QFile::exists(configfile))  // If not present, copy in the default
    {
        qInfo() << "Initialising default preferences at" << configfile;
        QFile::copy( QDir( QApplication::applicationDirPath()).filePath( DEFAULT_PREFS), configfile);
    }   // end if
    Cliniface::Preferences::load( configfile);
}   // end loadPreferences


void printHeader()
{
    qInfo() << "======================================================================";
    qInfo( " %s %s <%s>", APP_NAME, APP_VERSION_STRING, APP_WEBSITE);
    qInfo() << " Copyright 2019" << APP_ORGANISATION;
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

    Q_INIT_RESOURCE(resources);
    QApplication::setStyle( QStyleFactory::create("Fusion"));

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
    QCommandLineOption removeNonFaceOption( {"c", "component"}, QObject::tr( "Remove non-face components and export."));
    QCommandLineOption exportDirOption( {"t", "target-directory"},
            QObject::tr( "The <directory> to export files in 3DF format to (defaults to the input file's directory)."),
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

    FaceTools::registerTypes();
    loadPreferences();
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

            // Remove non-face components?
            if ( testRemoveNonFace)
            {
                // Can only perform if facial landmarks present
                if ( !FaceTools::Action::ActionGetComponent::removeNonFaceComponent(fm))
                    qWarning() << "Facial landmarks missing; cannot remove non-face components!";
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

    std::cerr << "-- Exiting --" << std::endl;
#if _WIN32
    FreeConsole();
#endif

    return rval;
}   // end main

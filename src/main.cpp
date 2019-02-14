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
#include <QDebug>
#include <QDir>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <sstream>
#include <iomanip>
#include <FaceTypes.h>
#include <QtDebug>
#include "ClinifaceMain.h"
#include "Preferences.h"


int main( int argc, char* argv[])
{
    qInfo() << "======================================================================";
    qInfo( " %s %s <%s>", APP_NAME, APP_VERSION_STRING, APP_WEBSITE);
    qInfo() << " Copyright 2018" << APP_ORGANISATION;
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

    QSurfaceFormat fmt = QVTKOpenGLWidget::defaultFormat();
    fmt.setSamples(0);  // Needed to allow FXAA to work properly!
    QSurfaceFormat::setDefaultFormat( fmt);
    //QSurfaceFormat::setDefaultFormat( QVTKOpenGLWidget::defaultFormat());
    //vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
    vtkObject::GlobalWarningDisplayOff();   // Prevent VTK warning/error pop-ups

    Q_INIT_RESOURCE(resources);
    QApplication app( argc, argv);
    QApplication::setStyle( QStyleFactory::create("Fusion"));

    QCoreApplication::setApplicationName( APP_NAME);
    QCoreApplication::setOrganizationName( APP_ORGANISATION);
    QCoreApplication::setApplicationVersion( APP_VERSION_STRING);

    QCommandLineParser parser;
    parser.setApplicationDescription( APP_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "The model file(s) to open."));
    parser.process(app);

    FaceTools::registerTypes();
    Cliniface::Preferences::load( QDir( QApplication::applicationDirPath()).filePath( APP_PREFS));

    Cliniface::ClinifaceMain* mainWin = new Cliniface::ClinifaceMain;

    for ( const QString& fname : parser.positionalArguments())
        mainWin->loadModel(fname);

    mainWin->show();
    const int rval = app.exec();

    qInfo( "-- Shutting down --");
    delete mainWin;
    qInfo( "-- Exit OK --");
    return rval;
}   // end main


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
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <sstream>
#include <iomanip>
#include "ClinifaceMain.h"


int main( int argc, char* argv[])
{
    std::cerr << APP_NAME << " " << APP_VERSION_STRING << " <" << APP_WEBSITE << ">" << std::endl
              << "Copyright 2018 " << APP_ORGANISATION << std::endl
              << "Developed by " << APP_AUTHOR_NAME << std::endl
              << "Cliniface is free software: you can redistribute it and/or modify" << std::endl
              << "it under the terms of the GNU General Public License as published by" << std::endl
              << "the Free Software Foundation, either version 3 of the License, or" << std::endl
              << "(at your option) any later version." << std::endl
              << "Cliniface is distributed in the hope that it will be useful," << std::endl
              << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
              << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl
              << "GNU General Public License for more details." << std::endl
              << "You should have received a copy of the GNU General Public License" << std::endl
              << "along with this program. If not, see <http://www.gnu.org/licenses/>." << std::endl << std::endl;

    Q_INIT_RESOURCE(resources);

    //QSurfaceFormat::setDefaultFormat( QVTKOpenGLWidget::defaultFormat()); // Use when switching over from QVTKWidget to QVTKOpenGLWidget
    QApplication app( argc, argv);

    /*
    qDebug() << "Available application styles:";
    const QStringList styles = QStyleFactory::keys();
    for ( QString s : styles)
        qDebug() << s;
    */

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

    vtkObject::GlobalWarningDisplayOff();   // Prevent VTK warning/error pop-ups
    Cliniface::ClinifaceMain* mainWin = new Cliniface::ClinifaceMain;
    for ( const QString& fname : parser.positionalArguments())
        mainWin->loadModel(fname);

    mainWin->show();
    const int rval = app.exec();
    delete mainWin;
    std::cerr << "\n -- Cheerio! --" << std::endl;
    return rval;
}   // end main


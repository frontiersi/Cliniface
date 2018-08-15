/************************************************************************
 * Copyright (C) 2017 Richard Palmer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <sstream>
#include <iomanip>
#include <QThread>
#include "ClinifaceMain.h"


void printFace()
{
   std::cerr << "\
    +--------------------------------------------------------------------------------+\n\
    |                                                    . . .  .   .                |\n\
    |                 . . . ..               .            ........ .    .    .       |\n\
    |                ........                              ...~...........           |\n\
    |           .............                              ......~......... ...      |\n\
    |   .   ........~...~...               ... .        .   .... .....~.... . . .    |\n\
    |     .. .....~.........   .        ...~..... .     .... ..........~..... ..     |\n\
    |    .......~~......... .  ... .. ...~..~~~.... . . .  .. ....... ......... ..   |\n\
    |    ...................          ........~... .          .. ... .. ...... .     |\n\
    | ...................   .          ........... .            .  . . ....... .     |\n\
    |  . ...~........ .. .                .......                         ........   |\n\
    | .......... ..                        . .     .                      ......     |\n\
    |  ............                  . ...  .. . .. .      .   .  .    .   ..... .   |\n\
    |.  ...... .        .      . .   ...~...~.~..~..... .. ..      . .... ......     |\n\
    | . ......     .     . ... ... ......~........... .........      ... ..~~.. .    |\n\
    | . ......  .....     ..~....~~.~~.~~~+~.~~+++~.~~.~~~~~~~     ..... ..~....     |\n\
    |    ............      .~~~~~~+~~~~~++++~+++++~+~~~~~.~.        ..~. .~.. .      |\n\
    | . .  ...~.....~..       ...~~~~~~~+o+++++o++..~....        ....~....~..  .     |\n\
    |    .....~.....~....        ..  ...~~++...~... ...           ....~.~~. ...      |\n\
    |     .. ..~..........         .. .. ....... ...             .......~...         |\n\
    |     . ....~~~~........ .              .   .     .   .     ..~..~.~~...         |\n\
    |        ....~~.~......  ..  .. ......~....~..~.~........ ......~..~.            |\n\
    |         . ..~~......... . ....~.~.~~~~~~~~~~~~.~.....  ......~.~.~.            |\n\
    |          . ..~~~........   ................... ...    ..........~              |\n\
    |           . ...............   ............... .  ............~...              |\n\
    |                .... ......~..... .........~..........~..........               |\n\
    |                ..............~.............................~...                |\n\
    |                .......................~.......................                 |\n\
    |                  .. .......... .....~....~~~~............~...   .              |\n\
    |                    .. ..... .........~~~~~~~~~....~.......     .               |\n\
    |                      . .............~~~~~~~~~~~~.......     ... .              |\n\
    |                 .       ..........~.~~~~~~+~+~~~.......   . . .                |\n\
    |                  . .      ... .....~~~~~~~~~~+~.~~...    .....                 |\n\
    |                 .. .       ......~.~~~~~~~.~~~.~...   ....... .                |\n\
    |                   . ...        ................   . ........ .     .           |\n\
    |                    .....      .       ..  .      .......... .                  |\n\
    +--------------------------------------------------------------------------------+\n";
}   // end printFace


int main( int argc, char* argv[])
{
    Q_INIT_RESOURCE(resources);

    //QSurfaceFormat::setDefaultFormat( QVTKOpenGLWidget::defaultFormat()); // Use when switching over from QVTKWidget to QVTKOpenGLWidget
    QApplication app( argc, argv);
    QCoreApplication::setApplicationName(APP_NAME);

    std::ostringstream adeets;
    adeets << "Copyright 2017,2018 " << APP_AUTHOR_NAME;
    QCoreApplication::setOrganizationName( adeets.str().c_str());

    QCoreApplication::setApplicationVersion( APP_VERSION_STRING);
    std::cerr
    << " --------------------------------------------------------------------------------------------\n"
    << "     " << APP_NAME << " v" << APP_VERSION_STRING << std::endl
    << "     " << adeets.str() << std::endl;
    printFace();
    std::cerr << std::endl;
    std::cerr
    << "  " << APP_NAME << " is free software in accordance with the conditions of the GNU General Public License v3.0.\n"
    << "  This program comes with ABSOLUTELY NO WARRANTY; see https://www.gnu.org/licenses/gpl-3.0.html for details.\n"
    << "  You are welcome to redistribute this program according to the above conditions; download from " << APP_SOURCE << "\n\n"
    << "  " << APP_NAME << " uses the following libraries and programs:\n"
    << "  - FaceTools (https://github.com/richeytastic/FaceTools)\n"
    << "  - rFeatures (https://github.com/richeytastic/rFeatures)\n"
    << "  - rModelIO (https://github.com/richeytastic/rModelIO)\n"
    << "  - libbuild (https://github.com/richeytastic/libbuild)\n"
    << "  - QTools (https://github.com/richeytastic/QTools)\n"
    << "  - rVTK (https://github.com/richeytastic/rVTK)\n"
    << "  - rlib (https://github.com/richeytastic/rlib)\n"
    << "  - Dlib (http://dlib.net)\n"
    << "  - Qt v5.10.1 (https://www.qt.io)\n"
    << "  - VTK v8.1.1 (https://www.vtk.org)\n"
    << "  - OpenCV (https://opencv.org)\n"
    << "  - CGAL (https://www.cgal.org)\n"
    << "  - Boost v1.64 (http://www.boost.org)\n"
    << "  - Eigen3 (http://eigen.tuxfamily.org)\n"
    << "  - LibICP (https://github.com/symao/libicp)\n"
    << "  - QuaZip (https://github.com/stachenov/quazip)\n"
    << "  - MiKTeX Portable (https://miktex.org/portable)\n"
    << "  - IDTF to U3D converter (http://www2.iaas.msu.ru/tmp/u3d)\n"
    << "  - Open Asset Import Library v3.3.1 (http://assimp.sourceforge.net)"
    << std::endl;

    QCommandLineParser parser;
    parser.setApplicationDescription(APP_DESCRIPTION);
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
    std::cerr << "\n -- Goodbye --" << std::endl;
    return rval;
}   // end main


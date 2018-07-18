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

    QApplication app( argc, argv);
    QCoreApplication::setApplicationName(APP_NAME);

    std::ostringstream adeets;
    adeets << "Copyright 2017,2018 " << APP_AUTHOR_NAME;
    QCoreApplication::setOrganizationName( adeets.str().c_str());

    std::ostringstream oss;
    oss << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << "." << APP_BUILD_DTSTAMP;
    std::string vstring = oss.str();

    QCoreApplication::setApplicationVersion( vstring.c_str());
    std::cerr
    << " --------------------------------------------------------------------------------------------\n"
    << "     " << APP_NAME << " build " << vstring << std::endl
    << "     " << adeets.str() << std::endl;
    printFace();
    std::cerr << std::endl;
    std::cerr
    << "  " << APP_NAME << " is open source under the GNU General Public License v3.0 (https://www.gnu.org/licenses/gpl-3.0.html).\n"
    << "  Source code can be downloaded from " << APP_SOURCE << "\n"
    << "  The developer is grateful to the authors of the following works used herein:\n"
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
    std::cerr << "\n -- Toodle-oo! --" << std::endl;
    return rval;
}   // end main


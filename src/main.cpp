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

#include <QVTKOpenGLNativeWidget.h>
#include <QSurfaceFormat>
#include <QStyleFactory>
#include <QApplication>
#include <FaceTools/FaceTypes.h>
#include <ClinifaceApp.h>


int main( int argc, char* argv[])
{
#ifdef _WIN32
    if ( AttachConsole(ATTACH_PARENT_PROCESS))
    {
        FILE *stream;
        freopen_s( &stream, "CONOUT$", "w+", stdout);
        freopen_s( &stream, "CONOUT$", "w+", stderr);
        //std::cout << std::endl;
    }   // end if
#endif

    QApplication::setStyle( QStyleFactory::create("Fusion"));

    QSurfaceFormat fmt = QVTKOpenGLNativeWidget::defaultFormat();
    //fmt.setProfile( QSurfaceFormat::CompatibilityProfile);
    //fmt.setSamples(0);  // Needed to allow FXAA to work properly!
    QSurfaceFormat::setDefaultFormat( fmt);

    //vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
#ifdef NDEBUG   // Prevent any VTK warning/error pop-ups for release build
    vtkObject::GlobalWarningDisplayOff();
#endif

    qRegisterMetaType<FaceTools::Action::Event>("Event");

    Q_INIT_RESOURCE(resources);

    QCoreApplication::setApplicationName( APP_NAME);
    QCoreApplication::setOrganizationName( APP_ORGANISATION);
    QCoreApplication::setApplicationVersion( APP_VERSION_STRING);

    Cliniface::ClinifaceApp cliniface;
    const int rval = cliniface.start( argc, argv);
#ifdef _WIN32
    FreeConsole();
#endif
    return rval;
}   // end main


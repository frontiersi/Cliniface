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
#include <vtkOpenGLRenderWindow.h>
#include <QOpenGLVersionProfile>
#include <QSurfaceFormat>
#include <QStyleFactory>
#include <QApplication>
#include <FaceTools/FaceTypes.h>
#include <ClinifaceApp.h>

namespace {
static const char* OPENGL_MAJOR = "CLINIFACE_OPENGL_MAJOR_VERSION";
static const char* OPENGL_MINOR = "CLINIFACE_OPENGL_MINOR_VERSION";

void setOpenGLVersion( QSurfaceFormat &sfmt)
{
    const QString glMajorVersion = qEnvironmentVariable(OPENGL_MAJOR);
    const QString glMinorVersion = qEnvironmentVariable(OPENGL_MINOR);
    if ( !glMajorVersion.isEmpty() && !glMinorVersion.isEmpty())
    {
        bool okayMajor = false;
        bool okayMinor = false;
        const int majorVersion = glMajorVersion.toInt( &okayMajor);
        const int minorVersion = glMinorVersion.toInt( &okayMinor);
        if ( okayMajor && okayMinor)
        {
            sfmt.setMajorVersion( majorVersion);
            sfmt.setMinorVersion( minorVersion);
        }   // end if
        else
            std::cerr << "Invalid values for environment variables " << OPENGL_MAJOR << " or " << OPENGL_MINOR << std::endl;
    }   // end if
}   // end setOpenGLVersion
}   // end namespace


int main( int argc, char* argv[])
{
#ifdef _WIN32
    if ( AttachConsole(ATTACH_PARENT_PROCESS))
    {
        FILE *stream;
        freopen_s( &stream, "CONOUT$", "w+", stdout);
        freopen_s( &stream, "CONOUT$", "w+", stderr);
    }   // end if

    const bool unsetGlMajor = qEnvironmentVariable(OPENGL_MAJOR).isEmpty();
    if ( unsetGlMajor)
        qputenv(OPENGL_MAJOR, QByteArray::number(2));

    const bool unsetGlMinor = qEnvironmentVariable(OPENGL_MINOR).isEmpty();
    if ( unsetGlMinor)
        qputenv(OPENGL_MINOR, QByteArray::number(0));
#endif

    QApplication::setStyle( QStyleFactory::create("Fusion"));
    QSurfaceFormat sfmt = QVTKOpenGLNativeWidget::defaultFormat();
    setOpenGLVersion( sfmt);
    QSurfaceFormat::setDefaultFormat( sfmt);
    QCoreApplication::setAttribute( Qt::AA_UseDesktopOpenGL);
    QCoreApplication::setAttribute( Qt::AA_CompressHighFrequencyEvents);

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
    if ( unsetGlMajor)
        qputenv(OPENGL_MAJOR, QByteArray());
    if ( unsetGlMinor)
        qputenv(OPENGL_MINOR, QByteArray());
#endif

    return rval;
}   // end main


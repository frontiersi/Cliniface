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

#include <AppUpdater.h>
#include <Cliniface_Config.h>
#include <quazip5/JlCompress.h>
#include <QCoreApplication>
#include <QTemporaryDir>
#include <iostream>
using Cliniface::AppUpdater;


// static definitions
QString AppUpdater::s_appExe;
QFileDevice::Permissions AppUpdater::s_appExePermissions;

// Record the application exe and its permissions as soon as program
// starts in case any other part of the program does anything to the files.
bool AppUpdater::recordAppExe()
{
    QString appExe;
#ifdef _WIN32
    appExe = QCoreApplication::applicationFilePath();
#elif __linux__
    appExe = qEnvironmentVariable("_");
    if ( appExe[0] == ".")
        appExe = qEnvironmentVariable("PWD") + "/" + appExe;
#endif
    s_appExe = QFileInfo(appExe).canonicalFilePath();
    s_appExePermissions = QFile( s_appExe).permissions();
    return isAppImage();
}   // end recordAppExe


bool AppUpdater::isAppImage()
{
#ifdef __linux__
    // We assume this is an AppImage if the applicationFilePath does NOT match s_appExe.
    // This is because s_appExe will be the filepath to the AppImage before it mounts the
    // temporary file system from which this application is actually run.
    return QCoreApplication::applicationFilePath() != s_appExe;
#endif
    return false;
}   // end isAppImage


AppUpdater::AppUpdater( const QString &fname)
    : _updatePath(fname), _oldRoot( QDir::home().filePath( QString(".%1/_previous").arg(EXE_NAME)))
{
}   // end ctor


void AppUpdater::run()
{
    _err = QString();
    // If an old update directory exists, remove it.
    if ( QFileInfo(_oldRoot).exists() && !QDir(_oldRoot).removeRecursively())
        _err = tr("Couldn't delete \"%1\"!").arg(_oldRoot);
    else
    {
#ifdef _WIN32
        _doWindowsUpdate();
#elif __linux__
        _doLinuxUpdate();
#endif
    }   // end else
    emit onFinished( _err);
}   // end run


void AppUpdater::_doLinuxUpdate()
{
    if ( !isAppImage())
    {
        _err = tr("Updates not supported for non-AppImage version!");
        return;
    }   // end if

    // Since the provided file is an AppImage and so is the application, just copy over!
    if ( !QFile::remove( s_appExe))
        _err = tr("Unable to remove \"%1\" to perform update").arg( s_appExe);
    else
    {
        if ( !QFile::copy( _updatePath, s_appExe))
            _err = tr("Failed to copy over update!");
        else
            QFile( s_appExe).setPermissions( s_appExePermissions);  // Reset permissions
    }   // end if
}   // end _doLinuxUpdate


namespace {

QString now2OldThenNew2Now( const QString &oldRoot, const QString &nowRoot, const QString &newRoot)
{
    QString err;
    const QStringList dirEntries = QDir(newRoot).entryList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for ( const QString &name : dirEntries)
    {
        const QString oldpath = oldRoot + "/" + name;
        const QString nowpath = nowRoot + "/" + name;
        const QString newpath = newRoot + "/" + name;  // Exists already

        if ( QFileInfo(newpath).isDir()) // Recurse if directory
            err = now2OldThenNew2Now( oldpath, nowpath, newpath);
        else
        {
            QFile newfile(newpath);
            QFile nowfile(nowpath);

            // If file currently exists, move to within %HOME%/.cliniface/_previous since can't delete
            if ( nowfile.exists())
            {
                const QDir olddir(QFileInfo(oldpath).path());
                if ( !olddir.exists() && !olddir.mkpath(olddir.path()))
                    return QString("Couldn't create \"%1\"").arg(olddir.path());
                if ( !nowfile.rename(oldpath))
                    return QString("Couldn't rename \"%1\" to \"%2\"").arg(nowpath).arg(oldpath);
            }   // end if

            if ( !newfile.rename(nowpath))
                return QString("Couldn't rename \"%1\" to \"%2\"").arg(newpath).arg(nowpath);
        }   // end else
    }   // end for
    return err;
}   // end now2OldThenNew2Now

}   // end namespace


void AppUpdater::_doWindowsUpdate()
{
    std::cerr << "Extracting update to temporary directory..." << std::endl;
    QTemporaryDir newRoot;
    QStringList flst = JlCompress::extractDir( _updatePath, newRoot.path());
    const bool extractedOkay = flst.size() > 0;
    if ( !extractedOkay)
    {
        _err = tr("Failed to extract update!");
        return;
    }   // end if

    std::cerr << "Moving current ==> old and update ==> current..." << std::endl;
    // Target is level above where cliniface.exe sits; "root/some_dirs/Cliniface" by default
    const QString nowRoot = QFileInfo( QCoreApplication::applicationDirPath() + "/..").canonicalFilePath();
    _err = now2OldThenNew2Now( _oldRoot, nowRoot, newRoot.path());
}   // end _doWindowsUpdate

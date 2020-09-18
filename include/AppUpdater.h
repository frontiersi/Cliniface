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

#ifndef Cliniface_AppUpdater_h
#define Cliniface_AppUpdater_h

#include <QThread>
#include <QFileDevice>

namespace Cliniface {

class AppUpdater : public QThread
{ Q_OBJECT
public:
    // Record the path to the application exe
    static bool recordAppExe();

    // Check if this is AppImage format (can only be true for Linux)
    static bool isAppImage();

    explicit AppUpdater( const QString &updateFile);

    const QString &error() const { return _err;}

signals:
    void onFinished( const QString&);

private:
    void run() override;

    void _doWindowsUpdate();
    void _doLinuxUpdate();

    const QString _updatePath;
    const QString _oldRoot;
    QString _err;

    static QString s_appExe;
    static QFileDevice::Permissions s_appExePermissions;
};  // end class

}   // end namespace
     
#endif

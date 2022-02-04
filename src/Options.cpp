/************************************************************************
 * Copyright (C) 2022 SIS Research Ltd & Richard Palmer
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

#include <Options.h>
#include <Cliniface_Config.h>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <iostream>
using Cliniface::Options;


namespace {

QString applicationRelativeFilePath( const QString &relPath)
{
    const QFileInfo finfo( QDir( QApplication::applicationDirPath()).filePath(relPath));
    return finfo.exists() ? finfo.filePath() : "";
}   // end applicationRelativeFilePath


QString absoluteFilePath( const QString &fpath)
{
    const QFileInfo finfo( fpath);
    return finfo.exists() ? finfo.filePath() : "";
}   // end absoluteFilePath


QString findOnPath( const QString &filepath)
{
    // Search the path first
    const QStringList pathEntries = qEnvironmentVariable("PATH").split(QDir::listSeparator());
    for ( const QString &pathEntry : pathEntries)
    {
        const QFileInfo finfo( QDir( pathEntry).filePath( filepath));
        if ( finfo.exists())
            return finfo.filePath();
    }   // end for

    QString fname = "";
#ifdef _WIN32
    fname = absoluteFilePath( qEnvironmentVariable("ProgramFiles") + "/" + filepath);
    if ( fname.isEmpty())
        fname = absoluteFilePath( qEnvironmentVariable("ProgramFiles(x86)") + "/" + filepath);
    if ( fname.isEmpty())
        fname = absoluteFilePath( qEnvironmentVariable("UserProfile") + "/" + filepath);
    if ( fname.isEmpty())
        fname = absoluteFilePath( qEnvironmentVariable("UserProfile") + "/AppData/Local/" + filepath);
    if ( fname.isEmpty())
        fname = absoluteFilePath( qEnvironmentVariable("UserProfile") + "/AppData/Local/Programs/" + filepath);
    if ( fname.isEmpty())
        fname = absoluteFilePath( qEnvironmentVariable("AllUsersProfile") + "/" + filepath);
#endif
    return fname;
}   // end findOnPath

}   // end namespace


Options::Options()
{
    _idtfconv           = applicationRelativeFilePath( IDTFCONV_PATH);
    _fdtool             = applicationRelativeFilePath( FDTOOL_PATH);
    _pdflatex           = applicationRelativeFilePath( PDFLATEX1_PATH);
    _haarmodl           = applicationRelativeFilePath( HAARMODELS_PATH);

    if ( _pdflatex.isEmpty())
        _pdflatex = findOnPath( PDFLATEX2_PATH);
    if ( _pdflatex.isEmpty())
        _pdflatex = findOnPath( PDFLATEX3_PATH);

    if ( _inkscape.isEmpty())
        _inkscape = findOnPath( INKSCAPE_PATH);

    _pageDims           = QSize(210,297);   // A4 portrait by default
    _openPDFOnSave      = true;

    _showBoxes          = true;
    _whiteBG            = true;
    _antiAlias          = true;
    _smoothLighting     = true;
    _interpShading      = true;
    _prlProjMetrics     = true;
    _viewAngle          = 30.0;

    _maxSmth            = 1.0;
    _cropRad            = 100.0;

    _maxman             = 5;
    _chkupdt            = true;
    _purl               = APP_UPDATE_URL;

    _maskPath           = applicationRelativeFilePath(MASK_PATH);
    _userImagesPath     = absoluteFilePath( QDir::home().filePath( QString(".%1/images").arg(EXE_NAME)));
}   // end ctor


bool Options::operator==( const Options &opts) const
{
    return _haarmodl == opts._haarmodl &&
           _idtfconv == opts._idtfconv &&
           _fdtool == opts._fdtool &&
           _pdflatex == opts._pdflatex &&
           _inkscape == opts._inkscape &&
           _pageDims == opts._pageDims &&
           _openPDFOnSave == opts._openPDFOnSave &&
           _showBoxes == opts._showBoxes &&
           _whiteBG == opts._whiteBG &&
           _antiAlias == opts._antiAlias &&
           _smoothLighting == opts._smoothLighting &&
           _interpShading == opts._interpShading &&
           _prlProjMetrics == opts._prlProjMetrics &&
           _viewAngle == opts._viewAngle &&
           _maxSmth == opts._maxSmth &&
           _cropRad == opts._cropRad &&
           _maxman == opts._maxman &&
           _chkupdt == opts._chkupdt &&
           _purl == opts._purl &&
           _maskPath == opts._maskPath &&
           _userImagesPath == opts._userImagesPath;
}   // end operator==


bool Options::operator!=( const Options &opts) const { return !(*this == opts);}

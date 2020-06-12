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


QString findOnPath( const QString &fname)
{
    const QStringList pathEntries = qEnvironmentVariable("PATH").split(QDir::listSeparator());
    for ( const QString &pathEntry : pathEntries)
    {
        const QFileInfo finfo( QDir( pathEntry).filePath( fname));
        if ( finfo.exists())
            return finfo.filePath();
    }   // end for
    return "";
}   // end findOnPath

}   // end namespace


Options::Options()
{
    _haarmodl           = applicationRelativeFilePath( HAARMODELS_PATH);
    _idtfconv           = applicationRelativeFilePath( IDTFCONV_PATH);

#ifdef _WIN32
    _pdflatex           = applicationRelativeFilePath( PDFLATEX_PATH);  // MiKTeX portable is bundled so should be here
    // Look at a bunch of possible locations that Inkscape could be installed
    _inkscape   = absoluteFilePath( qEnvironmentVariable("ProgramFiles") + "/" + INKSCAPE_PATH);
    if ( _inkscape.isEmpty())
        _inkscape = absoluteFilePath( qEnvironmentVariable("UserProfile") + "/" + INKSCAPE_PATH);
    if ( _inkscape.isEmpty())
        _inkscape = absoluteFilePath( qEnvironmentVariable("AllUsersProfile") + "/" + INKSCAPE_PATH);
#endif

    if ( _inkscape.isEmpty())
        _inkscape = findOnPath( INKSCAPE_PATH);
    if ( _pdflatex.isEmpty())
        _pdflatex = findOnPath( PDFLATEX_PATH);

    _openPDFOnSave      = true;

    _units              = "mm";
    _autoFocus          = false;
    _showBoxes          = true;
    _whiteBG            = true;
    _antiAlias          = true;
    _smoothLighting     = true;
    _interpShading      = true;
    _prlProjMetrics     = true;
    _viewAngle          = 30.0;
    _olapOpacityRed     = 0.20;

    _maxSmth            = 0.9;
    _curvDistTool       = false;
    _cropRad            = 100.0;

    _maxman             = 5;
    _maxload            = 20;

    _nrMaskPath         = applicationRelativeFilePath(MASK_PATH);
    _nrTotalIts         = 50;
    _nrRegNbs           = 50;
    _nrKnnReg           = 5;
    _nrFlagThresh       = 0.9;
    _nrEqPushPull       = false;
    _nrSmoothIts        = 50;
    _nrKappa            = 10.0;
    _nrOrient           = true;
    _nrSigma            = 1.6;
    _nrInitVisIts       = 50;
    _nrLastVisIts       = 1;
    _nrInitElsIts       = 50;
    _nrLastElsIts       = 1;
}   // end ctor


bool Options::operator==( const Options &opts) const
{
    return _haarmodl == opts._haarmodl &&
           _idtfconv == opts._idtfconv &&
           _pdflatex == opts._pdflatex &&
           _inkscape == opts._inkscape &&
           _openPDFOnSave == opts._openPDFOnSave &&
           _units == opts._units &&
           _autoFocus == opts._autoFocus &&
           _showBoxes == opts._showBoxes &&
           _whiteBG == opts._whiteBG &&
           _antiAlias == opts._antiAlias &&
           _smoothLighting == opts._smoothLighting &&
           _interpShading == opts._interpShading &&
           _prlProjMetrics == opts._prlProjMetrics &&
           _viewAngle == opts._viewAngle &&
           _olapOpacityRed == opts._olapOpacityRed &&
           _maxSmth == opts._maxSmth &&
           _curvDistTool == opts._curvDistTool &&
           _cropRad == opts._cropRad &&
           _maxman == opts._maxman &&
           _maxload == opts._maxload &&
           _nrMaskPath == opts._nrMaskPath &&
           _nrTotalIts == opts._nrTotalIts &&
           _nrRegNbs == opts._nrRegNbs &&
           _nrKnnReg == opts._nrKnnReg &&
           _nrFlagThresh == opts._nrFlagThresh &&
           _nrEqPushPull == opts._nrEqPushPull &&
           _nrSmoothIts == opts._nrSmoothIts &&
           _nrKappa == opts._nrKappa &&
           _nrOrient == opts._nrOrient &&
           _nrSigma == opts._nrSigma &&
           _nrInitVisIts == opts._nrInitVisIts &&
           _nrLastVisIts == opts._nrLastVisIts &&
           _nrInitElsIts == opts._nrInitElsIts &&
           _nrLastElsIts == opts._nrLastElsIts;
}   // end operator==


bool Options::operator!=( const Options &opts) const { return !(*this == opts);}

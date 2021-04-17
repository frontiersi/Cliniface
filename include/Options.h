/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#ifndef Cliniface_Options_h
#define Cliniface_Options_h

#include <QString>
#include <QSize>

namespace Cliniface {

class Options
{
public:
    Options();  // Creates default options
    Options( const Options&) = default;
    Options& operator=( const Options&) = default;

    bool operator==( const Options&) const;
    bool operator!=( const Options&) const;

    // ********* Report generation ************
    void setInkscape( const QString& s) { _inkscape = s;}
    const QString& inkscape() const { return _inkscape;}

    void setPdfLatex( const QString& s) { _pdflatex = s;}
    const QString& pdflatex() const { return _pdflatex;}

    void setIdtfConv( const QString& s) { _idtfconv = s;}
    const QString& idtfConv() const { return _idtfconv;}

    void setOpenPDFOnSave( bool v) { _openPDFOnSave = v;}
    bool openPDFOnSave() const { return _openPDFOnSave;}

    void setPageDims( const QSize& s) { _pageDims = s;}
    const QSize& pageDims() const { return _pageDims;}
    // ****************************************

    // ********* Memory limits ****************
    void setMaxMan( int v) { _maxman = v;}
    int maxMan() const { return _maxman;}
    // ****************************************
    
    // ********** Updates *********************
    void setCheckUpdate( bool v) { _chkupdt = v;}
    bool checkUpdate() const { return _chkupdt;}

    void setPatchURL( const QString &v) { _purl = v;}
    const QString &patchURL() const { return _purl;}
    // ****************************************

    // *************** Tools ******************
    void setMaxSmoothCurv( double v) { _maxSmth = v;}
    double maxSmoothCurv() const { return _maxSmth;}

    void setCropRadius( double v) { _cropRad = v;}
    double cropRadius() const { return _cropRad;}

    void setHaarModels( const QString& s) { _haarmodl = s;}
    const QString& haarModels() const { return _haarmodl;}
    // ****************************************

    // ******* Non-rigid registration *********
    void setNrMaskPath( const QString& s) { _nrMaskPath = s;}
    const QString& nrMaskPath() const { return _nrMaskPath;}
    // ****************************************

    // ********** Visualisation ***************
    void setShowBoxes( bool v) { _showBoxes = v;}
    bool showBoxes() const { return _showBoxes;}

    void setWhiteBG( bool v) { _whiteBG = v;}
    bool whiteBG() const { return _whiteBG;}

    void setAntiAlias( bool v) { _antiAlias = v;}
    bool antiAlias() const { return _antiAlias;}

    void setSmoothLighting( bool v) { _smoothLighting = v;}
    bool smoothLighting() const { return _smoothLighting;}

    void setInterpolatedShading( bool v) { _interpShading = v;}
    bool interpolatedShading() const { return _interpShading;}

    void setParallelProjectionMetrics( bool v) { _prlProjMetrics = v;}
    bool parallelProjectionMetrics() const { return _prlProjMetrics;}

    void setViewAngle( double v){ _viewAngle = v;}
    double viewAngle() const { return _viewAngle;}
    // ****************************************

private:
    int _maxman;
    bool _chkupdt;
    QString _purl;

    double _maxSmth;
    bool _curvDistTool;
    double _cropRad;

    QSize _pageDims;
    bool _openPDFOnSave;
    bool _showBoxes;
    bool _whiteBG;
    bool _antiAlias;
    bool _smoothLighting;
    bool _interpShading;
    bool _prlProjMetrics;

    double _viewAngle;

    QString _nrMaskPath;
    QString _pdflatex;
    QString _idtfconv;
    QString _haarmodl;
    QString _inkscape;
};  // end class

}   // end namespace

#endif

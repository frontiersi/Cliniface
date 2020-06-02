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

#ifndef Cliniface_Options_h
#define Cliniface_Options_h

#include <QString>

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
    // ****************************************

    // ********* Memory limits ****************
    void setMaxMan( int v) { _maxman = v;}
    int maxMan() const { return _maxman;}

    void setMaxLoad( int v) { _maxload = v;}
    int maxLoad() const { return _maxload;}
    // ****************************************

    // *************** Tools ******************
    void setMaxSmoothCurv( double v) { _maxSmth = v;}
    double maxSmoothCurv() const { return _maxSmth;}

    void setCurvDistTool( bool v) { _curvDistTool = v;}
    bool curvDistTool() const { return _curvDistTool;}

    void setCropRadius( double v) { _cropRad = v;}
    double cropRadius() const { return _cropRad;}

    void setHaarModels( const QString& s) { _haarmodl = s;}
    const QString& haarModels() const { return _haarmodl;}
    // ****************************************

    // ******* Non-rigid registration *********
    void setNrMaskPath( const QString& s) { _nrMaskPath = s;}
    const QString& nrMaskPath() const { return _nrMaskPath;}

    void setNrTotalIts( int v) { _nrTotalIts = v;}
    int nrTotalIts() const { return _nrTotalIts;}

    void setNrKnnReg( int v) { _nrKnnReg = v;}
    int nrKnnReg() const { return _nrKnnReg;}

    void setNrFlagThresh( double v) { _nrFlagThresh = v;}
    double nrFlagThresh() const { return _nrFlagThresh;}

    void setNrEqPushPull( bool v) { _nrEqPushPull = v;}
    bool nrEqPushPull() const { return _nrEqPushPull;}

    void setNrSmoothIts( int v) { _nrSmoothIts = v;}
    int nrSmoothIts() const { return _nrSmoothIts;}

    void setNrKappa( double v) { _nrKappa = v;}
    double nrKappa() const { return _nrKappa;}

    void setNrOrient( bool v) { _nrOrient = v;}
    bool nrOrient() const { return _nrOrient;}

    void setNrSigma( double v) { _nrSigma = v;}
    double nrSigma() const { return _nrSigma;}

    void setNrRegNbs( int v) { _nrRegNbs = v;}
    int nrRegNbs() const { return _nrRegNbs;}

    void setNrInitVisIts( int v) { _nrInitVisIts = v;}
    int nrInitVisIts() const { return _nrInitVisIts;}

    void setNrLastVisIts( int v) { _nrLastVisIts = v;}
    int nrLastVisIts() const { return _nrLastVisIts;}

    void setNrInitElsIts( int v) { _nrInitElsIts = v;}
    int nrInitElsIts() const { return _nrInitElsIts;}

    void setNrLastElsIts( int v) { _nrLastElsIts = v;}
    int nrLastElsIts() const { return _nrLastElsIts;}
    // ****************************************

    // ********** Visualisation ***************
    void setUnits( const QString& s) { _units = s;}
    const QString& units() const { return _units;}

    void setAutoFocus( bool v) { _autoFocus = v;}
    bool autoFocus() const { return _autoFocus;}

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

    void setOverlapOpacityReduction( double v){ _olapOpacityRed = v;}
    double overlapOpacityReduction() const { return _olapOpacityRed;}
    // ****************************************

private:
    int _maxload;
    int _maxman;

    double _maxSmth;
    bool _curvDistTool;
    double _cropRad;

    int _nrTotalIts;
    int _nrRegNbs;
    int _nrKnnReg;
    double _nrFlagThresh;
    bool _nrEqPushPull;
    int _nrSmoothIts;
    double _nrKappa;
    bool _nrOrient;
    double _nrSigma;
    int _nrInitVisIts;
    int _nrLastVisIts;
    int _nrInitElsIts;
    int _nrLastElsIts;

    bool _openPDFOnSave;
    bool _autoFocus;
    bool _showBoxes;
    bool _whiteBG;
    bool _antiAlias;
    bool _smoothLighting;
    bool _interpShading;
    bool _prlProjMetrics;

    double _viewAngle;
    double _olapOpacityRed;

    QString _units;
    QString _nrMaskPath;
    QString _pdflatex;
    QString _idtfconv;
    QString _haarmodl;
    QString _inkscape;
};  // end class

}   // end namespace

#endif

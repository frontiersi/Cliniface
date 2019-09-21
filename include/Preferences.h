/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
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

#ifndef Cliniface_Preferences_h
#define Cliniface_Preferences_h

#include <QString>
#include <memory>
#include <sol.hpp>

namespace Cliniface {

class Preferences
{
public:
    using Ptr = std::shared_ptr<Preferences>;

    // Return (creating if necessary) the preferences instance.
    // Returns null if the preferences file cannot be accessed.
    static Ptr get();

    static void updateApplication();

    // Overwrite data in the persistent preferences file
    // returning false iff cannot access file for writing.
    // Also updates any application wide settings.
    bool write();

    // Returns full path to the persistent configuration file being used.
    const QString& configPath() const { return _configfile;}

    const QString& maskPath() const { return _maskpath;}
    const QString& pdfReader() const { return _pdfreader;}
    const QString& inkScape() const { return _inkscape;}
    const QString& pdflatex() const { return _pdflatex;}
    const QString& units() const { return _units;}
    int maxLoad() const { return _maxload;}
    int maxMan() const { return _maxman;}
    int knnReg() const { return _knnreg;}
    bool openPDFOnSave() const { return _openPDFOnSave;}
    bool autoFocus() const { return _autoFocus;}
    bool whiteBG() const { return _whiteBG;}
    bool antiAlias() const { return _antiAlias;}
    double overlapOpacity() const { return _olapOpacity;}
    double metricOpacity() const { return _metricOpacity;}

    void setMaskPath( const QString& s) { _maskpath = s;}
    void setPdfReader( const QString& s) { _pdfreader = s;}
    void setInkscape( const QString& s) { _inkscape = s;}
    void setPdfLatex( const QString& s) { _pdflatex = s;}
    void setUnits( const QString& s) { _units = s;}
    void setMaxLoad( int v) { _maxload = v;}
    void setMaxMan( int v) { _maxman = v;}
    void setKnnReg( int v) { _knnreg = v;}
    void setOpenPDFOnSave( bool v) { _openPDFOnSave = v;}
    void setAutoFocus( bool v) { _autoFocus = v;}
    void setWhiteBG( bool v) { _whiteBG = v;}
    void setAntiAlias( bool v) { _antiAlias = v;}
    void setOverlapOpacity( double v){ _olapOpacity = v;}
    void setMetricOpacity( double v){ _metricOpacity = v;}

private:
    static Ptr _singleton;
    bool _allspec;
    int _maxload, _maxman, _knnreg;
    bool _openPDFOnSave;
    bool _autoFocus;
    bool _whiteBG;
    bool _antiAlias;
    double _olapOpacity;
    double _metricOpacity;
    QString _units;
    QString _configfile;
    QString _maskpath;
    QString _pdflatex, _idtfconv;
    QString _haarmodl, _faceshap;
    QString _pdfreader, _inkscape;
    sol::state _lua;

    // Returns true iff the last read in of parameters specified all of
    // them without any default being set. If this function returns false,
    // it is necessary to update the configuration file with new parameters.
    static bool _allSpecified();

    // Load preferences returning true on success.
    static bool _read( const QString& luascript);

    bool _read();
    bool _write( const QString&);
    QString _readFilePath( const char*, const char*);
    QString _readDirPath( const char*, const char*);
    QString _readString( const char*, const char*);
    bool _readBool( const char*, bool);
    int _readInt( const char*, int);
    double _readDouble( const char*, double);

    Preferences();
    Preferences( const Preferences&) = delete;
    void operator=( const Preferences&) = delete;
};  // end class

}   // end namespace

#endif

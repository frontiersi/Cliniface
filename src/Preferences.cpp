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

#include <Cliniface_Config.h>
#include <Preferences.h>
#include <FeaturesDetector.h>
#include <FaceShapeLandmarks2DDetector.h>
#include <ActionNonRigidRegistration.h>
#include <ActionShowMetrics.h>
#include <ActionSetOpacity.h>
#include <ActionExportPDF.h>
#include <FaceModelManager.h>
#include <ReportManager.h>
#include <ModelSelector.h>
#include <QApplication>
#include <QStandardPaths>
#include <QTextStream>
#include <QFile>
#include <QDir>
using Cliniface::Preferences;
using FaceTools::FMV;
using MS = FaceTools::Action::ModelSelector;


Preferences::Ptr Preferences::_singleton;


Preferences::Ptr Preferences::get()
{
    if ( !_singleton)
    {
        // Get the location of the preferences file in the user's home directory
        const QString homedir = QStandardPaths::locate( QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory);
        const QString configfile = QDir( homedir).filePath( QString(".%1").arg(EXE_NAME));

        _singleton = Ptr( new Preferences, []( Preferences* x){ delete x;});
        _singleton->_configfile = configfile;

        if ( !QFile::exists(configfile))  // If not present, write out an empty file.
        {
            std::cerr << "Initialising default preferences at " << configfile.toStdString() << std::endl;
            _singleton->write( configfile);
        }   // end if

        if ( read( configfile))
        {
            if ( !allSpecified())
            {
                std::cerr << "Updating preferences at " << configfile.toStdString() << std::endl;
                _singleton->write();
            }   // end if
        }   // end if
        else
            _singleton = nullptr;
    }   // end if

    return _singleton;
}   // end get


// static
bool Preferences::read( const QString& fpath)
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::read: ";
    bool loadedOk = false;

    try
    {
        _singleton->_lua.script_file( fpath.toStdString());
        loadedOk = true;
    }   // end try
    catch ( const sol::error& e)
    {
        std::cerr << werr << "Unable to read '" << fpath.toStdString() << "'!" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
    }   // end catch

    if (loadedOk && !_singleton->read())
    {
        std::cerr << werr << "Unable to read values from '" << fpath.toStdString() << "'!" << std::endl;
        loadedOk = false;
    }   // end if

    return loadedOk;
}   // end read


bool Preferences::write( const QString& fpath)
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::write: ";
    QFile file( fpath);
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cerr << werr << "Unable to open file for writing!" << std::endl;
        return false;
    }   // end if

    QTextStream os(&file);
    os << "prefs = {" << endl
     << "\tmaskpath = \""    << get()->_maskpath << "\"," << endl
     << "\tinkscape = \""    << get()->_inkscape << "\"," << endl
     << "\tpdflatex = \""    << get()->_pdflatex << "\"," << endl
     << "\tpdfreader = \""   << get()->_pdfreader << "\"," << endl
     << "\tidtfconv = \""    << get()->_idtfconv << "\"," << endl
     << "\thaarmodels = \""  << get()->_haarmodl << "\"," << endl
     << "\tfaceshape = \""   << get()->_faceshap << "\"," << endl
     << "\tunits = \""       << get()->_units << "\"," << endl
     << "\topenpdfonsave = " << (get()->_openPDFOnSave ? "true" : "false") << "," << endl
     << "\tautofocus = "     << (get()->_autoFocus ? "true" : "false") << "," << endl
     << "\twhitebg = "       << (get()->_whiteBG ? "true" : "false") << "," << endl
     << "\tantialias = "     << (get()->_antiAlias ? "true" : "false") << "," << endl
     << "\tmaxmanifolds = "  << get()->_maxman << "," << endl
     << "\tknnreg = "        << get()->_knnreg << "," << endl
     << "\tmaxload = "       << get()->_maxload << "," << endl
     << "\toopacity = "      << get()->_olapOpacity << "," << endl
     << "\tmopacity = "      << get()->_metricOpacity << "," << endl
     << "}" << endl;

    return true;
}   // end write


bool Preferences::write()
{
    bool wok = false;
    if ( write(_configfile))
    {
        updateApplication();
        wok = true;
    }   // end if
    return wok;
}   // end write


bool Preferences::allSpecified() { return get()->_allspec;}


Preferences::Preferences()
    : _allspec(false), _maxload(20), _maxman(5), _knnreg(7),
      _openPDFOnSave(true), _autoFocus(true), _whiteBG(true), _antiAlias(true),
      _olapOpacity(0.80), _metricOpacity(0.99), _units("mm")
{
    _lua.open_libraries( sol::lib::base);
}   // end ctor



QString Preferences::readFilePath( const char* cstr, const char* d)
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::readFilePath: ";
    QString val;
    if ( sol::optional<std::string> v = _lua["prefs"][cstr])
        val = v.value().c_str();

    if ( val.isEmpty() || !QFile::exists(val))
    {
        _allspec = false;
        val = QDir( QApplication::applicationDirPath()).filePath(d);
        std::cerr << werr << "Empty or invalid '" << cstr << "' entry so trying " << val.toStdString() << std::endl;
    }   // end if

    if ( val.isEmpty() || !QFile::exists(val))
        val = "";

    return val;
}   // end readFilePath


QString Preferences::readString( const char* c, const char* d)
{
    QString vstr = d;
    sol::optional<std::string> v = _lua["prefs"][c];
    if ( v)
        vstr = v.value().c_str();
    if ( vstr.isEmpty())
        _allspec = false;
    return vstr;
}   // end readString


int Preferences::readInt( const char* c, int d)
{
    int v = d;
    sol::optional<int> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end readInt


double Preferences::readDouble( const char* c, double d)
{
    double v = d;
    sol::optional<double> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end readDouble


bool Preferences::readBool( const char* c, bool d)
{
    bool v = d;
    sol::optional<bool> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end readBool


bool Preferences::read()
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::read: ";
    static const std::string eerr = "[ERROR] Cliniface::Preferences::read: ";

    const sol::table table = _lua["prefs"];
    if ( !table.valid())
    {
        std::cerr << eerr << "No 'prefs' table!" << std::endl;
        return false;
    }   // end if

    _allspec       = true;
    _units         = readString( "units", "mm");
    _maskpath      = readFilePath( "maskpath", MASK_PATH);
    _pdflatex      = readFilePath( "pdflatex", PDFLATEX_PATH);
    _idtfconv      = readFilePath( "idtfconv", IDTFCONV_PATH);
    _faceshap      = readFilePath( "faceshape", FACESHAPE_PATH);
    _haarmodl      = readFilePath( "haarmodels", HAARMODELS_PATH);
    _inkscape      = readFilePath( "inkscape", INKSCAPE_PATH);
    _pdfreader     = readFilePath( "pdfreader", PDFREADER_PATH);
    _openPDFOnSave = readBool( "openpdfonsave", !_pdfreader.isEmpty()) && !_pdfreader.isEmpty();
    _autoFocus     = readBool( "autofocus", true);
    _whiteBG       = readBool( "whitebg", true);
    _antiAlias     = readBool( "antialias", true);
    _maxman        = std::max( 1, readInt( "maxmanifolds", 5));
    _maxload       = std::max( 1, readInt( "maxload", 20));
    _knnreg        = std::max( 1, readInt( "knnreg", 7));
    _olapOpacity   = std::max( 0.1, std::min( readDouble( "oopacity", 0.80), 1.0));
    _metricOpacity = std::max( 0.1, std::min( readDouble( "mopacity", 0.99), 1.0));

    // Try to initialise the face detection module
    const std::string haarModels = _haarmodl.toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << werr << "Unable to initialise face detector (" << haarModels << ")" << std::endl;
    const std::string faceShapeLandmarks = _faceshap.toStdString();
    if ( !FaceTools::Detect::FaceShapeLandmarks2DDetector::initialise( faceShapeLandmarks))
        std::cerr << werr << "Unable to initialise landmarks detector (" << faceShapeLandmarks << ")" << std::endl;

    updateApplication();
    FaceTools::FaceModel::LENGTH_UNITS = _units;
    FaceTools::Report::ReportManager::init( _pdflatex, _idtfconv, _inkscape);

    return true;
}   // end read


void Preferences::updateApplication()
{
    FaceTools::FileIO::FMM::setLoadLimit( size_t( get()->maxLoad()));
    FaceTools::FaceModel::MAX_MANIFOLDS = get()->maxMan();
    MS::setAutoFocusOnSelectEnabled( get()->autoFocus());

    FaceTools::Action::ActionNonRigidRegistration::setMaskPath( get()->maskPath());
    FaceTools::Action::ActionNonRigidRegistration::setKNN( get()->knnReg());
    FaceTools::Action::ActionSetOpacity::setOpacityOnOverlap( get()->overlapOpacity());
    FaceTools::Action::ActionShowMetrics::setOpacityOnShow( get()->metricOpacity());

    FaceTools::Action::ActionExportPDF::setOpenOnSave( get()->openPDFOnSave());

    QColor bg = get()->whiteBG() ? Qt::white : Qt::black;
    for ( FMV* fmv : MS::viewers())
    {
        fmv->setBackgroundColour( bg);
        fmv->getRenderer()->SetUseFXAA( get()->antiAlias());
    }   // end for
}   // end updateApplication

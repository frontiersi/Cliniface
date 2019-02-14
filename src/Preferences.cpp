/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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

#include <Preferences.h>
#include <FeaturesDetector.h>
#include <FaceShapeLandmarks2DDetector.h>
#include <FaceModelManager.h>
#include <ReportManager.h>
#include <QApplication>
#include <QFile>
#include <QDir>
using Cliniface::Preferences;


Preferences::Ptr Preferences::_singleton;


bool Preferences::load( const QString& fpath)
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::load: ";
    bool loadedOk = false;
    _singleton = Ptr( new Preferences, []( Preferences* x){ delete x;});

    try
    {
        _singleton->_lua.script_file( fpath.toStdString());
        loadedOk = true;
    }   // end try
    catch ( const sol::error& e)
    {
        std::cerr << werr << "Unable to load '" << fpath.toStdString() << "'!" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
    }   // end catch

    if ( !loadedOk)
    {
        _singleton = nullptr;
        return false;
    }   // end if

    if ( !_singleton->read())
    {
        std::cerr << werr << "Unable to read values from '" << fpath.toStdString() << "'!" << std::endl;
        _singleton = nullptr;
        return false;
    }   // end if

    return true;
}   // end load


Preferences::Preferences() : _maxload(1)
{
    _lua.open_libraries( sol::lib::base);
}   // end ctor


namespace {

QString readFilePath( const sol::table& table, const char* cstr)
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::readFilePath: ";
    QString val;
    if ( sol::optional<std::string> v = table[cstr])
        val = QDir( QApplication::applicationDirPath()).filePath(v.value().c_str());
    else
        std::cerr << werr << "No '" << cstr << "' entry!" << std::endl;

    if (!QFile::exists(val))
    {
        std::cerr << werr << "'" << val.toStdString() << "' does not exist!" << std::endl;
        val = "";
    }   // end if

    return val;
}   // end readFilePath
}   // end namespace



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

    _pdflatex = readFilePath( table, "pdflatex");
    _idtfconv = readFilePath( table, "idtfconv");
    _faceshap = readFilePath( table, "faceshape");
    _haarmodl = readFilePath( table, "haarmodels");
    _inkscape = readFilePath( table, "inkscape");

    _pdfreader = readFilePath( table, "pdfreader");
    _openPDFOnSave = table["openpdfonsave"].get_or(false) && !_pdfreader.isEmpty();

    _maxload = table["maxload"].get_or(1);

    // Try to initialise the face detection module
    const std::string haarModels = _haarmodl.toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << werr << "Unable to initialise face detector (" << haarModels << ")" << std::endl;
    const std::string faceShapeLandmarks = _faceshap.toStdString();
    if ( !FaceTools::Detect::FaceShapeLandmarks2DDetector::initialise( faceShapeLandmarks))
        std::cerr << werr << "Unable to initialise landmarks detector (" << faceShapeLandmarks << ")" << std::endl;

    FaceTools::FileIO::FMM::setLoadLimit( _maxload);
    FaceTools::Report::ReportManager::init( _pdflatex, _idtfconv, _inkscape);

    return true;
}   // end read


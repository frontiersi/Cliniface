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

#include <Cliniface_Config.h>
#include <Preferences.h>
#include <UpdatesDialog.h>

#include <FaceTools/Detect/FeaturesDetector.h>

#include <FaceTools/MaskRegistration.h>
#include <FaceTools/FileIO/FaceModelDatabase.h>
#include <FaceTools/Action/ActionSelect.h>
#include <FaceTools/Action/ActionExportPDF.h>
#include <FaceTools/Action/ActionShowMetrics.h>
#include <FaceTools/Action/ActionSmooth.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/ModelSelect.h>
#include <FaceTools/Action/FaceActionManager.h>

#include <FaceTools/Vis/FaceView.h>
#include <FaceTools/FaceModel.h>
#include <FaceTools/Path.h>

#include <FaceTools/Report/ReportManager.h>

#include <QTextStream>
#include <QFile>
#include <QDir>
using Cliniface::Preferences;
using Cliniface::Options;
using FaceTools::FMV;
using FaceTools::Vis::FV;
using FaceTools::FVS;
using FaceTools::FM;
using MS = FaceTools::ModelSelect;


Preferences::Ptr Preferences::_singleton;


bool Preferences::init() { return _get() != nullptr;}


Preferences::Ptr Preferences::_get()
{
    if ( !_singleton)
    {
        // Get the location of the preferences file in the user's home directory
        const QString configfile = QDir::home().filePath( QString(".%1/preferences").arg(EXE_NAME));
        _singleton = Ptr( new Preferences, []( Preferences* x){ delete x;});
        _singleton->_configfile = configfile;

        if ( !QFile::exists(configfile))  // If not present, write out an empty file.
        {
            std::cout << "Initialising preferences at " << configfile.toStdString() << std::endl;
            writeConfig();
        }   // end if

        if ( _read( configfile))
        {
            if ( !_allSpecified())
            {
                //std::cout << "Updating preferences due to missing/invalid entries" << std::endl;
                writeConfig();
            }   // end if
        }   // end if
        else
            _singleton = nullptr;
    }   // end if

    return _singleton;
}   // end _get


// static
bool Preferences::_read( const QString& fpath)
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

    if (loadedOk && !_singleton->_read())
    {
        std::cerr << werr << "Unable to read values from '" << fpath.toStdString() << "'!" << std::endl;
        loadedOk = false;
    }   // end if

    return loadedOk;
}   // end _read


namespace {
QString printBool( bool v) { return v ? "true" : "false";}
QString printSize( const QSize &s) { return QString("{ width = %1, height = %2}").arg(s.width()).arg(s.height());}
}   // end namespace


bool Preferences::writeConfig()
{
    QString fpath = _get()->_configfile;
    static const std::string werr = "[WARNING] Cliniface::Preferences::writeConfig: ";
    QFile file( fpath);
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cerr << werr << "Unable to open file for writing!" << std::endl;
        return false;
    }   // end if

    const Options &opts = _get()->appliedOptions();

    QTextStream os(&file);
    os << "prefs = {" << Qt::endl

     << "\thaarModels = \""    << opts.haarModels() << "\"," << Qt::endl
     << "\tidtfConv = \""      << opts.idtfConv() << "\"," << Qt::endl
     << "\tfdtool = \""        << opts.fdTool() << "\"," << Qt::endl
     << "\tpdfLaTeX = \""      << opts.pdflatex() << "\"," << Qt::endl
     << "\tinkscape = \""      << opts.inkscape() << "\"," << Qt::endl
     << "\tpageDims = "        << printSize( opts.pageDims()) << "," << Qt::endl
     << "\topenPdfOnSave = "   << printBool( opts.openPDFOnSave()) << "," << Qt::endl

     << "\tshowBoxes = "       << printBool( opts.showBoxes()) << "," << Qt::endl
     << "\twhiteBG = "         << printBool( opts.whiteBG()) << "," << Qt::endl
     << "\tantiAlias = "       << printBool( opts.antiAlias()) << "," << Qt::endl
     << "\tsmoothLighting = "  << printBool( opts.smoothLighting()) << "," << Qt::endl
     << "\tinterpShading =  "  << printBool( opts.interpolatedShading()) << "," << Qt::endl
     << "\tprlProjMetrics = "  << printBool( opts.parallelProjectionMetrics()) << "," << Qt::endl
     << "\tviewAngle = "       << opts.viewAngle() << "," << Qt::endl

     << "\tmaxManifolds = "    << opts.maxMan() << "," << Qt::endl
     << "\tcheckUpdate = "     << printBool( opts.checkUpdate()) << "," << Qt::endl
     << "\tpatchURL601 = \""   << opts.patchURL() << "\"," << Qt::endl

     << "\tmaxCurv = "         << opts.maxSmoothCurv() << "," << Qt::endl
     << "\tnCropRad = "        << opts.cropRadius() << "," << Qt::endl
     << "\tuserImages = \""    << opts.userImagesPath() << "\"," << Qt::endl
     << "\tmask0 = \""         << opts.maskPath() << "\"," << Qt::endl
     << "}" << Qt::endl;

    return true;
}   // end write


bool Preferences::_allSpecified() { return _get()->_allspec;}


Preferences::Preferences() : _allspec(false)
{
    _lua.open_libraries( sol::lib::base);
}   // end ctor


QString Preferences::_readFilePath( const char* cstr)
{
    QString val;
    if ( sol::optional<std::string> v = _lua["prefs"][cstr])
        val = v.value().c_str();

    QFileInfo finfo(val);
    if ( !finfo.exists() || !finfo.isFile())
    {
        _allspec = false;
        val = "";
    }   // end if

    return val;
}   // end _readFilePath


QString Preferences::_readDirPath( const char* cstr)
{
    QString val;
    if ( sol::optional<std::string> v = _lua["prefs"][cstr])
        val = v.value().c_str();

    QFileInfo finfo(val);
    if ( !finfo.exists() || !finfo.isDir())
    {
        _allspec = false;
        val = "";
    }   // end if

    return val;
}   // end _readDirPath


QString Preferences::_readString( const char* c, const QString &d)
{
    QString vstr = d;
    const sol::optional<std::string> v = _lua["prefs"][c];
    if ( v)
        vstr = v.value().c_str();
    else
        _allspec = false;
    return vstr;
}   // end _readString


int Preferences::_readInt( const char* c, int d)
{
    int v = d;
    const sol::optional<int> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end _readInt


double Preferences::_readDouble( const char* c, double d)
{
    double v = d;
    const sol::optional<double> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end _readDouble


bool Preferences::_readBool( const char* c, bool d)
{
    bool v = d;
    sol::optional<bool> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end _readBool


QSize Preferences::_readSize( const char* c, const QSize &d)
{
    QSize v = d;
    const sol::table s = _lua["prefs"][c];
    if ( s.valid())
    {
        v.setWidth( s["width"].get_or(d.width()));
        v.setHeight( s["height"].get_or(d.height()));
    }   // end if
    else
        _allspec = false;
    return v;
}   // end _readSize


void Preferences::reset() { _get()->_opts = Options();}
bool Preferences::isApplied() { return _get()->_opts == _get()->_aopts;}

const QString& Preferences::configPath() { return _get()->_configfile;}


bool Preferences::_read()
{
    static const std::string werr = "[WARNING] Cliniface::Preferences::read: ";
    static const std::string eerr = "[ERROR] Cliniface::Preferences::read: ";

    const sol::table table = _lua["prefs"];
    if ( !table.valid())
    {
        std::cerr << eerr << "No 'prefs' table!" << std::endl;
        return false;
    }   // end if

    _allspec            = true;

    Options opts;   // Default set on construction

    const QString haarModelsPath = _readDirPath( "haarModels");
    if ( !haarModelsPath.isEmpty())
        opts.setHaarModels( haarModelsPath);

    const QString idtfConvPath = _readFilePath( "idtfConv");
    if ( !idtfConvPath.isEmpty())
        opts.setIdtfConv( idtfConvPath);

    const QString fdToolPath = _readFilePath( "fdtool");
    if ( !fdToolPath.isEmpty())
        opts.setFDTool( fdToolPath);

    const QString pdflatexPath = _readFilePath( "pdfLaTeX");
    if ( !pdflatexPath.isEmpty())
        opts.setPdfLatex( pdflatexPath);

    const QString inkscapePath = _readFilePath( "inkscape");
    if ( !inkscapePath.isEmpty())
        opts.setInkscape( inkscapePath);

    opts.setOpenPDFOnSave( _readBool( "openPdfOnSave", opts.openPDFOnSave()));

    opts.setPageDims( _readSize( "pageDims", opts.pageDims()));

    opts.setShowBoxes(  _readBool( "showBoxes", opts.showBoxes()));
    opts.setWhiteBG(    _readBool( "whiteBG", opts.whiteBG()));
    opts.setAntiAlias(  _readBool( "antiAlias", opts.antiAlias()));
    opts.setSmoothLighting( _readBool( "smoothLighting", opts.smoothLighting()));
    opts.setInterpolatedShading( _readBool( "interpShading", opts.interpolatedShading()));
    opts.setParallelProjectionMetrics( _readBool( "prlProjMetrics", opts.parallelProjectionMetrics()));
    opts.setViewAngle( std::max( 1.0, std::min( _readDouble( "viewAngle", opts.viewAngle()), 89.9)));

    opts.setMaxSmoothCurv( std::max( 0.0, std::min( _readDouble( "maxCurv", opts.maxSmoothCurv()), 1.0)));
    opts.setCropRadius( std::max( 1.0, std::min( _readDouble( "nCropRad", opts.cropRadius()), 1000.0)));

    opts.setMaxMan( std::max( 1, _readInt( "maxManifolds", opts.maxMan())));
    opts.setCheckUpdate( _readBool( "checkUpdate", opts.checkUpdate()));
    opts.setPatchURL( _readString( "patchURL601", opts.patchURL()));

    const QString userImagesPath = _readFilePath( "userImages");
    if ( !userImagesPath.isEmpty())
        opts.setUserImagesPath( userImagesPath);

    const QString maskPath = _readFilePath( "mask0");
    if ( !maskPath.isEmpty())
        opts.setMaskPath( maskPath);

    // Try to initialise the face detection module
    const std::string haarModels = opts.haarModels().toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << werr << "Unable to initialise face detector (" << haarModels << ")" << std::endl;

    _get()->_opts = opts;
    apply();

    FaceTools::FaceModel::LENGTH_UNITS = "mm";
    FaceTools::Report::ReportManager::init( opts.pdflatex(), opts.idtfConv());
    FaceTools::Report::Report::setInkscape( opts.inkscape());
    FaceTools::Report::Report::setDefaultPageDims( opts.pageDims());

    return true;
}   // end _read


void Preferences::setOptions( const Options &opts) { _get()->_opts = opts;}
const Options& Preferences::options() { return _get()->_opts;}
const Options& Preferences::appliedOptions() { return _get()->_aopts;}


void Preferences::apply()
{
    using namespace FaceTools;
    const Options &opts = _get()->_opts;

    FaceModel::MAX_MANIFOLDS = opts.maxMan();
    UpdatesDialog::setAutoCheckUpdate( opts.checkUpdate());
    UpdatesDialog::setPatchURL( opts.patchURL());
    MS::setViewAngle( opts.viewAngle());
    Action::ActionSelect::setShowBoundingBoxes( opts.showBoxes());

    Vis::FaceView::setSmoothLighting( opts.smoothLighting());
    Vis::FaceView::setInterpolatedShading( opts.interpolatedShading());
    Action::ActionShowMetrics::setParallelProjectionOnShow( opts.parallelProjectionMetrics());

    Action::ActionSmooth::setMaxCurvature( opts.maxSmoothCurv());
    Action::ActionExtractFace::setCropRadius( opts.cropRadius());

    MaskRegistration::setMask( opts.maskPath());
    FileIO::FaceModelDatabase::reset();
    FileIO::FaceModelDatabase::refresh( opts.userImagesPath());
    Action::ActionExportPDF::setOpenOnSave( opts.openPDFOnSave());

    QColor bg = opts.whiteBG() ? Qt::white : Qt::black;
    for ( FMV* fmv : MS::viewers())
    {
        fmv->setBackgroundColour( bg);
        fmv->getRenderer()->SetUseFXAA( opts.antiAlias());
    }   // end for

    _get()->_aopts = opts;  // Make current options the applied options

    MS::resetAllViews();
}   // end apply 

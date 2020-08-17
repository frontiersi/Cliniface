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

#include <Cliniface_Config.h>
#include <Preferences.h>

#include <FaceTools/Detect/FeaturesDetector.h>

#include <FaceTools/FileIO/FaceModelManager.h>

#include <FaceTools/MaskRegistration.h>
#include <FaceTools/Action/ActionSetOpacity.h>
#include <FaceTools/Action/ActionExportPDF.h>
#include <FaceTools/Action/ActionShowMetrics.h>
#include <FaceTools/Action/ActionSmooth.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/Action/ModelSelector.h>
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
using MS = FaceTools::Action::ModelSelector;


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

     << "\thaarModels = \""   << opts.haarModels() << "\"," << Qt::endl
     << "\tidtfConv = \""     << opts.idtfConv() << "\"," << Qt::endl
     << "\tpdfLaTeX = \""     << opts.pdflatex() << "\"," << Qt::endl
     << "\tinkscape = \""     << opts.inkscape() << "\"," << Qt::endl
     << "\topenPdfOnSave = "  << printBool( opts.openPDFOnSave()) << "," << Qt::endl

     << "\tunits = \""        << opts.units() << "\"," << Qt::endl
     << "\tautoFocus = "      << printBool( opts.autoFocus()) << "," << Qt::endl
     << "\tshowBoxes = "      << printBool( opts.showBoxes()) << "," << Qt::endl
     << "\twhiteBG = "        << printBool( opts.whiteBG()) << "," << Qt::endl
     << "\tantiAlias = "      << printBool( opts.antiAlias()) << "," << Qt::endl
     << "\tsmoothLighting = " << printBool( opts.smoothLighting()) << "," << Qt::endl
     << "\tinterpShading =  " << printBool( opts.interpolatedShading()) << "," << Qt::endl
     << "\tprlProjMetrics = " << printBool( opts.parallelProjectionMetrics()) << "," << Qt::endl
     << "\tviewAngle = "      << opts.viewAngle() << "," << Qt::endl
     << "\tolapOpacityRed = " << opts.overlapOpacityReduction() << "," << Qt::endl

     << "\tmaxManifolds = "   << opts.maxMan() << "," << Qt::endl
     << "\tmaxLoad = "        << opts.maxLoad() << "," << Qt::endl

     << "\tmaxSmth = "        << opts.maxSmoothCurv() << "," << Qt::endl
     << "\tcurvDistTool = "   << printBool( opts.curvDistTool()) << "," << Qt::endl
     << "\tnCropRad = "       << opts.cropRadius() << "," << Qt::endl

     << "\tnrMaskPath = \""     << opts.nrMaskPath() << "\"," << Qt::endl
     << "\tnr502TotalIts = "    << opts.nrTotalIts() << "," << Qt::endl
     << "\tnr502RegNbs = "      << opts.nrRegNbs() << "," << Qt::endl
     << "\tnr502KnnReg = "      << opts.nrKnnReg() << "," << Qt::endl
     << "\tnr502FlagThresh = "  << opts.nrFlagThresh() << "," << Qt::endl
     << "\tnr502EqPushPull = "  << printBool( opts.nrEqPushPull()) << "," << Qt::endl
     << "\tnr502SmoothIts = "   << opts.nrSmoothIts() << "," << Qt::endl
     << "\tnr502Kappa = "       << opts.nrKappa() << "," << Qt::endl
     << "\tnr502Orient = "      << printBool( opts.nrOrient()) << "," << Qt::endl
     << "\tnr502Sigma = "       << opts.nrSigma() << "," << Qt::endl
     << "\tnr502InitVisIts = "  << opts.nrInitVisIts() << "," << Qt::endl
     << "\tnr502LastVisIts = "  << opts.nrLastVisIts() << "," << Qt::endl
     << "\tnr502InitElsIts = "  << opts.nrInitElsIts() << "," << Qt::endl
     << "\tnr502LastElsIts = "  << opts.nrLastElsIts() << "," << Qt::endl
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
    sol::optional<std::string> v = _lua["prefs"][c];
    if ( v)
        vstr = v.value().c_str();
    if ( vstr.isEmpty())
        _allspec = false;
    return vstr;
}   // end _readString


int Preferences::_readInt( const char* c, int d)
{
    int v = d;
    sol::optional<int> tv = _lua["prefs"][c];
    if ( tv)
        v = tv.value();
    else
        _allspec = false;
    return v;
}   // end _readInt


double Preferences::_readDouble( const char* c, double d)
{
    double v = d;
    sol::optional<double> tv = _lua["prefs"][c];
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

    const QString pdflatexPath = _readFilePath( "pdfLaTeX");
    if ( !pdflatexPath.isEmpty())
        opts.setPdfLatex( pdflatexPath);

    const QString inkscapePath = _readFilePath( "inkscape");
    if ( !inkscapePath.isEmpty())
        opts.setInkscape( inkscapePath);

    const QString maskPath = _readFilePath( "nrMaskPath");
    if ( !maskPath.isEmpty())
        opts.setNrMaskPath( maskPath);

    opts.setOpenPDFOnSave( _readBool( "openPdfOnSave", opts.openPDFOnSave()));

    opts.setUnits(      _readString( "units", opts.units()));
    opts.setAutoFocus(  _readBool( "autoFocus", opts.autoFocus()));
    opts.setShowBoxes(  _readBool( "showBoxes", opts.showBoxes()));
    opts.setWhiteBG(    _readBool( "whiteBG", opts.whiteBG()));
    opts.setAntiAlias(  _readBool( "antiAlias", opts.antiAlias()));
    opts.setSmoothLighting( _readBool( "smoothLighting", opts.smoothLighting()));
    opts.setInterpolatedShading( _readBool( "interpShading", opts.interpolatedShading()));
    opts.setParallelProjectionMetrics( _readBool( "prlProjMetrics", opts.parallelProjectionMetrics()));
    opts.setViewAngle( std::max( 1.0, std::min( _readDouble( "viewAngle", opts.viewAngle()), 89.9)));
    opts.setOverlapOpacityReduction( std::max( 0.0, std::min( _readDouble( "olapOpacityRed", opts.overlapOpacityReduction()), 1.0)));

    opts.setMaxSmoothCurv( std::max( 0.0, std::min( _readDouble( "maxSmth", opts.maxSmoothCurv()), 1.0)));
    opts.setCurvDistTool( _readBool( "curvDistTool", opts.curvDistTool()));
    opts.setCropRadius( std::max( 1.0, std::min( _readDouble( "nCropRad", opts.cropRadius()), 1000.0)));

    opts.setMaxMan( std::max( 1, _readInt( "maxManifolds", opts.maxMan())));
    opts.setMaxLoad( std::max( 1, _readInt( "maxLoad", opts.maxLoad())));

    opts.setNrTotalIts( std::max( 1, _readInt( "nr502TotalIts", opts.nrTotalIts())));
    opts.setNrRegNbs( std::max( 1, _readInt( "nr502RegNbs", opts.nrRegNbs())));
    opts.setNrKnnReg( std::max( 1, _readInt( "nr502KnnReg", opts.nrKnnReg())));
    opts.setNrFlagThresh( std::max( 0.0, std::min( _readDouble( "nr502FlagThresh", opts.nrFlagThresh()), 1.0)));
    opts.setNrEqPushPull( _readBool( "nr502EqPushPull", opts.nrEqPushPull()));
    opts.setNrSmoothIts( std::max( 1, _readInt( "nr502SmoothIts", opts.nrSmoothIts())));
    opts.setNrKappa( std::max( 0.0, std::min( _readDouble( "nr502Kappa", opts.nrKappa()), 10.0)));
    opts.setNrOrient( _readBool( "nr502Orient", opts.nrOrient()));
    opts.setNrSigma( std::max( 0.0, std::min( _readDouble( "nr502Sigma", opts.nrSigma()), 10.0)));
    opts.setNrInitVisIts( std::max( 1, _readInt( "nr502InitVisIts", opts.nrInitVisIts())));
    opts.setNrLastVisIts( std::max( 1, _readInt( "nr502LastVisIts", opts.nrLastVisIts())));
    opts.setNrInitElsIts( std::max( 1, _readInt( "nr502InitElsIts", opts.nrInitElsIts())));
    opts.setNrLastElsIts( std::max( 1, _readInt( "nr502LastElsIts", opts.nrLastElsIts())));

    // Try to initialise the face detection module
    const std::string haarModels = opts.haarModels().toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << werr << "Unable to initialise face detector (" << haarModels << ")" << std::endl;

    _get()->_opts = opts;
    apply();

    FaceTools::FaceModel::LENGTH_UNITS = opts.units();
    FaceTools::Report::ReportManager::init( opts.pdflatex(), opts.idtfConv(), opts.inkscape());

    return true;
}   // end _read


void Preferences::setOptions( const Options &opts) { _get()->_opts = opts;}
const Options& Preferences::options() { return _get()->_opts;}
const Options& Preferences::appliedOptions() { return _get()->_aopts;}


void Preferences::apply()
{
    using namespace FaceTools;
    const Options &opts = _get()->_opts;

    FileIO::FaceModelManager::setLoadLimit( size_t( opts.maxLoad()));
    FaceModel::MAX_MANIFOLDS = opts.maxMan();
    MS::setViewAngle( opts.viewAngle());
    MS::setAutoFocusOnSelectEnabled( opts.autoFocus());
    MS::setShowBoundingBoxesOnSelected( opts.showBoxes());

    Vis::FaceView::setSmoothLighting( opts.smoothLighting());
    Vis::FaceView::setInterpolatedShading( opts.interpolatedShading());
    Action::ActionShowMetrics::setParallelProjectionOnShow( opts.parallelProjectionMetrics());

    Path::setPathType( opts.curvDistTool() ? Path::CURVE_FOLLOWING_1 : Path::ORIENTED_CURVE);
    Action::ActionSmooth::setMaxCurvature( opts.maxSmoothCurv());
    Action::ActionExtractFace::setCropRadius( opts.cropRadius());

    // Set Mask Registration parameters
    MaskRegistration::setMask( opts.nrMaskPath());
    MaskRegistration::Params prms;
    prms.k = size_t(opts.nrKnnReg());
    prms.flagThresh = float(opts.nrFlagThresh());
    prms.eqPushPull = opts.nrEqPushPull();
    prms.kappa = float(opts.nrKappa());
    prms.useOrient = opts.nrOrient();
    prms.numInlierIts = size_t(opts.nrSmoothIts());
    prms.smoothK = size_t(opts.nrRegNbs());
    prms.sigmaSmooth = float(opts.nrSigma());
    prms.numViscousStart = size_t(opts.nrInitVisIts());
    prms.numViscousEnd = size_t(opts.nrLastVisIts());
    prms.numElasticStart = size_t(opts.nrInitElsIts());
    prms.numElasticEnd = size_t(opts.nrLastElsIts());
    prms.numUpdateIts = size_t(opts.nrTotalIts());
    MaskRegistration::setParams( prms);

    Action::ActionSetOpacity::setOverlapOpacityReduction( float(opts.overlapOpacityReduction()));
    Action::ActionExportPDF::setOpenOnSave( opts.openPDFOnSave());

    QColor bg = opts.whiteBG() ? Qt::white : Qt::black;
    for ( FMV* fmv : MS::viewers())
    {
        fmv->setBackgroundColour( bg);
        fmv->getRenderer()->SetUseFXAA( opts.antiAlias());
    }   // end for

    _get()->_aopts = opts;  // Make current options the applied options
}   // end apply 

/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <CurvatureVis.h>
#include <Action/ActionVisualise.h>     // FaceTools
#include <Vis/SurfaceVisualisation.h>   // FaceTools
#include <FaceModelCurvature.h>         // FaceTools
#include <FaceModel.h>                  // FaceTools
using FM = FaceTools::FM;
using SV = FaceTools::Vis::SurfaceVisualisation;
using OMCM = RFeatures::ObjModelCurvatureMetrics;
using SMM = FaceTools::Vis::SurfaceMetricsMapper;
using FMC = FaceTools::FaceModelCurvature;

namespace {

using CurvMappingFn = std::function<float(const OMCM*,int)>;

class CurvMapper : public SMM
{
public:
    static SMM::Ptr create( const std::string& label, const CurvMappingFn& fn, float minv, float maxv)
    {
        return SMM::Ptr( new CurvMapper( label, fn, minv, maxv));
    }   // end create

protected:
    // Returns true if the data are available for the given FaceModel. In this case,
    // the static surface data object calculated within FaceTools is checked.
    bool isAvailable( const FM *fm) const override
    {
        // Note that there's no need to set when to purge since FaceTools::ActionMapCurvature
        // calls FaceModelCurvature::purge whenever the model geometry is changed.
        FMC::RPtr fmc = FMC::rmetrics(fm);
        return SMM::isAvailable(fm) && fmc != nullptr;
    }   // end isAvailable

    float metric( int fid, size_t) override { return _fn( _cmetrics, fid);}

    // Called when needing to do mapping by FaceTools::Vis::SurfaceVisualisation.
    // Used to setup data needing to be accessed in order to perform the per polygon metric() calls.
    bool init( const FM *fm) override
    {
        fm->lockForRead();
        _cmap = FMC::rmetrics(fm);
        _cmetrics = new OMCM( fm->model(), fm->manifolds(), *_cmap);
        return true;
    }   // end init

    void done( const FM* fm) override
    {
        _cmap = nullptr;
        fm->unlock();
        delete _cmetrics;
    }   // end done

private:
    CurvMapper( const std::string& label, const CurvMappingFn& fn, float minv, float maxv)
        : SMM( label, true/*polygon data*/, 1/*dimensionality: scalar data*/, minv, maxv), _fn(fn), _cmetrics(nullptr) {}

    CurvMappingFn _fn;
    const OMCM *_cmetrics;
    FMC::RPtr _cmap;
};  // end class

}   // end namespace


using Cliniface::CurvatureVis;

class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( const QString& s, const QIcon& i, SV* sv) : ActionVisualise( s, i, sv) {}
    QString attachToMenu() override { return "Scalar Mapping";}
    QString attachToToolBar() override { return "Scalar Mapping";}
};  // end VisAction


CurvatureVis::CurvatureVis()
{
    /*
    SV* sv0 = new SV( CurvMapper::create( "P1 Curvature",
                      []( const OMCM *m, int f){ return m->faceKP1FirstOrder(f);}, -1, 1),
                      QIcon(":/icons/KP1"));

    SV* sv1 = new SV( CurvMapper::create( "P2 Curvature",
                      []( const OMCM *m, int f){ return m->faceKP2FirstOrder(f);}, -1, 1),
                      QIcon(":/icons/KP2"));
    */

    SV* sv2 = new SV( CurvMapper::create( "Mean\nCurvature",
                      []( const OMCM *m, int f){ return (m->faceKP1FirstOrder(f) + m->faceKP2FirstOrder(f))/2;}, -1, 1));

    SV* sv3 = new SV( CurvMapper::create( "Greatest\nCurvature",
                      //[]( const OMCM *m, int f){ return std::max(fabs(m->faceKP1FirstOrder(f)), fabs(m->faceKP2FirstOrder(f)));}, 0, 1),
                      []( const OMCM *m, int f){ return 0.5 * (fabs(m->faceKP1FirstOrder(f)) + fabs(m->faceKP2FirstOrder(f)));}, 0, 1));

    /*
    SV* sv4 = new SV( CurvMapper::create( "Inflection",
                      []( const OMCM *m, int f){ return (m->faceKP1SecondOrder(f) + m->faceKP2SecondOrder(f))/2;}, -1, 1),
                      //[]( const OMCM *m, int f){ return sqrt( pow(m->faceKP1SecondOrder(f),2) + pow(m->faceKP2SecondOrder(f),2));}, 0, 1.0f),
                      QIcon(":/icons/INFLECTION"));

    SV* sv5 = new SV( CurvMapper::create( "Gaussian\nCurvature",
                      []( const OMCM *m, int f){ return m->faceKP1FirstOrder(f) * m->faceKP2FirstOrder(f);}, -0.5f, 0.5f),
                      QIcon(":/icons/GAUSSIAN"));

    SV* sv6 = new SV( CurvMapper::create( "Determinant",
                      []( const OMCM *m, int f){ return m->faceDeterminant(f);}, -0.1f, 0.1f),
                      QIcon(":/icons/DETERMINANT"));
    */

    using FaceTools::Action::ActionVisualise;

    VisAction* av0 = new VisAction( "Mean\nCurvature", QIcon(":/icons/MEAN"), sv2);
    VisAction* av1 = new VisAction( "Greatest\nCurvature", QIcon(":/icons/ABS"), sv3);
    appendPlugin(av0);
    appendPlugin(av1);
}   // end ctor

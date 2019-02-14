/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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
#include <ActionVisualise.h>        // FaceTools
#include <SurfaceVisualisation.h>   // FaceTools
#include <FaceModelSurfaceData.h>   // FaceTools
using FM = FaceTools::FM;
using SV = FaceTools::Vis::SurfaceVisualisation;
using FMSD = FaceTools::FaceModelSurfaceData;
using OMCM = RFeatures::ObjModelCurvatureMetrics;
using CurvMappingFn = std::function<float(const OMCM*,int)>;
using SDM = FaceTools::Vis::SurfaceDataMapper;

// Qt requires this to be outside of any namespace
void initResource() { Q_INIT_RESOURCE( cvis_icons);}


namespace {

class CurvMapper : public SDM
{
public:
    static SDM::Ptr create( const std::string& label, const CurvMappingFn& fn, float minv, float maxv)
    {
        return SDM::Ptr( new CurvMapper( label, fn, minv, maxv));
    }   // end create

    // Returns true if the data are available for the given FaceModel. In this case,
    // the static surface data object calculated within FaceTools is checked.
    bool isAvailable( const FM *fm) const override { return FMSD::get()->isAvailable(fm);}

protected:
    float metric( int fid, size_t) override { return _fn( _cmetrics, fid);}

    // Called when needing to do mapping by FaceTools::Vis::SurfaceVisualisation.
    // Used to setup data needing to be accessed in order to perform the per polygon metric() calls.
    bool init( const FM *fm)
    {
        FaceTools::SurfaceData::RPtr msd = FMSD::rdata(fm);    // Scoped read lock
        _cmetrics = msd->metrics;   // Set the curvature metrics parameter for the delegate mapping fn.
        return true;
    }   // end init

    CurvMappingFn _fn;
    const OMCM *_cmetrics;

private:
    CurvMapper( const std::string& label, const CurvMappingFn& fn, float minv, float maxv)
        : SDM( label, true/*polygon data*/, 1/*dimensionality: scalar data*/, minv, maxv), _fn(fn), _cmetrics(nullptr) {}
};  // end class

}   // end namespace


using Cliniface::CurvatureVis;

CurvatureVis::CurvatureVis()
{
    initResource();

    SV* sv0 = new SV( CurvMapper::create( "P1 Curvature",
                      []( const OMCM *m, int f){ return m->faceKP1FirstOrder(f);}, -1, 1),
                      QIcon(":/icons/KP1"));

    SV* sv1 = new SV( CurvMapper::create( "P2 Curvature",
                      []( const OMCM *m, int f){ return m->faceKP2FirstOrder(f);}, -1, 1),
                      QIcon(":/icons/KP2"));

    SV* sv2 = new SV( CurvMapper::create( "Mean\nCurvature",
                      []( const OMCM *m, int f){ return (m->faceKP1FirstOrder(f) + m->faceKP2FirstOrder(f))/2;}, -1, 1),
                      QIcon(":/icons/MEAN"));

    SV* sv3 = new SV( CurvMapper::create( "Greatest\nCurvature",
                      //[]( const OMCM *m, int f){ return std::max(fabs(m->faceKP1FirstOrder(f)), fabs(m->faceKP2FirstOrder(f)));}, 0, 1),
                      []( const OMCM *m, int f){ return 0.5 * (fabs(m->faceKP1FirstOrder(f)) + fabs(m->faceKP2FirstOrder(f)));}, 0, 1),
                      QIcon(":/icons/ABS"));

    /*
    SV* sv4 = new SV( CurvMapper::create( "Inflection",
                      []( const OMCM *m, int f){ return (m->faceKP1SecondOrder(f) + m->faceKP2SecondOrder(f))/2;}, -1, 1),
                      //[]( const OMCM *m, int f){ return sqrt( pow(m->faceKP1SecondOrder(f),2) + pow(m->faceKP2SecondOrder(f),2));}, 0, 1.0f),
                      QIcon(":/icons/INFLECTION"));
                      */

    SV* sv5 = new SV( CurvMapper::create( "Gaussian\nCurvature",
                      []( const OMCM *m, int f){ return m->faceKP1FirstOrder(f) * m->faceKP2FirstOrder(f);}, -0.5f, 0.5f),
                      QIcon(":/icons/GAUSSIAN"));

    SV* sv6 = new SV( CurvMapper::create( "Determinant",
                      []( const OMCM *m, int f){ return m->faceDeterminant(f);}, -0.1f, 0.1f),
                      QIcon(":/icons/DETERMINANT"));

    using FaceTools::Action::ActionVisualise;

    //ActionVisualise* av0 = new ActionVisualise(sv0);
    //ActionVisualise* av1 = new ActionVisualise(sv1);
    ActionVisualise* av2 = new ActionVisualise(sv2);
    ActionVisualise* av3 = new ActionVisualise(sv3);
    //ActionVisualise* av4 = new ActionVisualise(sv4);
    //ActionVisualise* av5 = new ActionVisualise(sv5);
    //ActionVisualise* av6 = new ActionVisualise(sv6);

    // 4 and 5 less useful so don't place on toolbar
    //av4->setAllowOnToolbar(false);
    //av5->setAllowOnToolbar(false);

    //addAction( av0);
    //addAction( av1);
    addAction( av2);
    addAction( av3);
    //addAction( av4);
    //addAction( av5);
    //addAction( av6);
}   // end ctor

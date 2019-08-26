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

#include <SymmetryVis.h>
#include <ActionVisualise.h>        // FaceTools
#include <SurfaceVisualisation.h>   // FaceTools
#include <FaceModel.h>              // FaceTools
#include <sstream>
using FM = FaceTools::FM;
using SV = FaceTools::Vis::SurfaceVisualisation;
using SMM = FaceTools::Vis::SurfaceMetricsMapper;
using EG = FaceTools::Action::EventGroup;
using FaceTools::Action::Event;

namespace {

class ScalarSymmetryMapper : public SMM
{
public:
    static SMM::Ptr create( const std::string& label, float minv, float maxv)
    {
        SMM::Ptr smm( new ScalarSymmetryMapper( label, minv, maxv));
        smm->setVisibleRange( minv/2, maxv/2);
        return smm;
    }   // end create

protected:
    bool purge( const FM *fm, Event e) override
    {
        // If the triggering event is AFFINE_CHANGE but the model has landmarks, then don't purge.
        // This is because the asymmetry calculation is provided by the model's orientation (given its landmarks).
        // Without landmarks, asymmetry is simply calculated according to the difference through the YZ plane.
        if ( EG(e).is(Event::AFFINE_CHANGE) && fm->hasLandmarks())
            return false;
        _vdiffs.erase(fm);
        return true;
    }   // end purge

    float metric( int fid, size_t) override
    {
        assert( _vdiffs.count(_curfm) > 0);
        // Metric is the average of the vertex differences for the current model.
        const int* vidxs = _curfm->model().fvidxs(fid);
        const std::unordered_map<int,double>& vds = _vdiffs.at(_curfm);
        double val = (vds.at(vidxs[0]) + vds.at(vidxs[1]) + vds.at(vidxs[2])) * 1.0/3;
        assert( !isnan(val));
        return float(val);
    }   // end metric

    bool init( const FM *fm) override
    {
        fm->lockForRead();
        _curfm = fm;
        // Obtain the data to do symmetry mapping to fm if it doesn't already exist.
        if ( _vdiffs.count(fm) == 0)
        {
            using namespace RFeatures;
            const ObjModel& model = fm->model();
            const ObjModelKDTree& kdt = fm->kdtree();
            const ObjModelSurfacePointFinder spfinder( model);

            cv::Vec3f rpt(0,0,0);
            cv::Vec3f rvec(1,0,0);
            if ( fm->hasLandmarks())
            {
                FaceTools::Landmark::LandmarkSet::CPtr lmks = fm->makeMeanLandmarksSet();
                const Orientation& on = lmks->orientation();
                rpt = lmks->fullMean();
                rvec = on.uvec().cross(on.nvec());
            }   // end if

            const int tv = -1;
            const int mv = -1;
            cv::Vec3f g, q, p;
            int notused;
            std::unordered_map<int,double>& vds = _vdiffs[fm];
            const IntSet& vidxs = model.vtxIds();
            for ( int vidx : vidxs)
            {
                p = model.vtx(vidx);   // Original vertex
                q = p;                 // Copy out for mirroring

                if ( vidx == tv)
                {
                    std::cerr << "Test vertex " << tv << " at   " << p << std::endl;
                    std::cerr << "Mirror vertex " << mv << " at " << model.vtx(mv) << std::endl;
                }   // end if

                ObjModelReflector::reflectPoint( q, rpt, rvec);  // Mirror the vertex through the median plane

                if ( vidx == tv)
                    std::cerr << "Reflected to " << q << " (should be close to mirror vertex given slight rotation)" << std::endl;

                int svidx = kdt.find(q);              // Closest vertex on surface to q

                if ( vidx == tv)
                    std::cerr << "Closest vertex to reflected vertex is " << svidx << std::endl;

                assert( svidx >= 0);
                spfinder.find( q, svidx, notused, g); // Find g as the closest point on surface to q 
                if ( vidx == tv)
                    std::cerr << "Closest point on surface to reflected vertex is " << g << std::endl;

                // Get the sign of the difference as indicating if the mirrored point on the
                // surface is further out (positive) or closer in (negative).
                const double sgn = cv::norm( p - g) > cv::norm( p - q) ? -1 : 1;
                const double diff = cv::norm( q - g) / 2; // The difference is halved because this is a comparison with the pseudo "mean" face.
                assert( !isnan(diff));
                vds[vidx] = sgn * diff;    // Disparity of surface to reflected point
            }   // end for
        }   // end if
        return true;
    }   // end init

    void done( const FM* fm) override
    {
        fm->unlock();
    }   // end done

private:
    ScalarSymmetryMapper( const std::string& label, float minv, float maxv)
        : SMM( label, true/*polygon data*/, 1/*dimensionality: scalar data*/, minv, maxv) {}

    std::unordered_map<const FM*, std::unordered_map<int,double> > _vdiffs;  // Calculated vertex differences
    const FM *_curfm;
};  // end class

}   // end namespace


using Cliniface::SymmetryVis;


class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( const QString& s, const QIcon& i, SV* sv) : ActionVisualise( s, i, sv) {}
    QString attachToMenu() override { return "Scalar Mapping";}
    QString attachToToolBar() override { return "Scalar Mapping";}
};  // end VisAction


SymmetryVis::SymmetryVis()
{
    using FaceTools::Action::ActionVisualise;

    const QString nm = "Asymmetry (" + FM::LENGTH_UNITS + ")";
    SV* sv = new SV( ScalarSymmetryMapper::create( nm.toStdString(), -10, 10));
    VisAction* act = new VisAction( nm, QIcon(":/icons/SYMMETRY"), sv);
    act->addPurgeEvent( {Event::LANDMARKS_CHANGE, Event::GEOMETRY_CHANGE, Event::AFFINE_CHANGE});
    appendPlugin(act);
}   // end ctor

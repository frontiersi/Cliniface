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

#include <SymmetryVis.h>
#include <ActionVisualise.h>        // FaceTools
#include <SurfaceVisualisation.h>   // FaceTools
#include <FaceModel.h>              // FaceTools
#include <sstream>
using FM = FaceTools::FM;
using SV = FaceTools::Vis::SurfaceVisualisation;
using SDM = FaceTools::Vis::SurfaceDataMapper;

void initResource() { Q_INIT_RESOURCE( sym_icons);}

namespace {

class ScalarSymmetryMapper : public SDM
{
public:
    static SDM::Ptr create( const std::string& label, float minv, float maxv)
    { return SDM::Ptr( new ScalarSymmetryMapper( label, minv, maxv));}

    bool isAvailable( const FM *fm) const override { return fm->centreSet();}

    void purge( const FM *fm) { _vdiffs.erase(fm);}

protected:
    float metric( int fid, size_t) override
    {
        // Metric is the average of the vertex differences for the current model.
        const int* vidxs = _curfm->info()->cmodel()->getFaceVertices(fid);
        const std::unordered_map<int,float>& vds = _vdiffs.at(_curfm);
        return (vds.at(vidxs[0]) + vds.at(vidxs[1]) + vds.at(vidxs[2])) * 1.0f/3;
    }   // end metric

    bool init( const FM *fm)
    {
        _curfm = fm;
        // Obtain the data to do symmetry mapping to fm if it doesn't already exist.
        if ( _vdiffs.count(fm) == 0)
        {
            using namespace RFeatures;
            const ObjModel* model = fm->info()->cmodel();
            const ObjModelKDTree* kdt = fm->kdtree();
            const ObjModelSurfacePointFinder spfinder( model);

            int notused, cvidx;
            cv::Vec3f sv, mv, ov;
            const Orientation& on = fm->orientation();
            cv::Vec3f ppt = fm->centre();
            cv::Vec3f pvec = on.uvec().cross(on.nvec());
            std::unordered_map<int,float>& vds = _vdiffs[fm];
            for ( int vidx : model->getVertexIds())
            {
                ov = model->vtx(vidx);    // Original vertex
                mv = ov;                  // Copy out for mirroring
                ObjModelReflector::reflectPoint( mv, ppt, pvec);  // Mirror the vertex
                cvidx = kdt->find(mv); // Find vertex closest to the mirrored point
                spfinder.find( mv, cvidx, notused, sv); // Find closest point on surface to mv
                const float symMag = cv::norm( ov - mv);
                const float disMag = cv::norm( mv - sv);  // Magnitude of disparity of surface to reflected point
                // Get the sign of the difference as indicating if the mirrored point on the
                // surface is further out (positive) or closer in (negative).
                const float sgn = cv::norm( ov - sv) > symMag ? -1 : 1;
                vds[vidx] = sgn * disMag;
            }   // end for
        }   // end if
        return true;
    }   // end init

private:
    ScalarSymmetryMapper( const std::string& label, float minv, float maxv)
        : SDM( label, true/*polygon data*/, 1/*dimensionality: scalar data*/, minv, maxv) {}

    std::unordered_map<const FM*, std::unordered_map<int,float> > _vdiffs;  // Calculated vertex differences
    const FM *_curfm;
};  // end class

}   // end namespace


using Cliniface::SymmetryVis;

SymmetryVis::SymmetryVis()
{
    initResource();

    using FaceTools::Action::ActionVisualise;

    std::ostringstream oss;
    oss << "Asymmetry (" << FM::LENGTH_UNITS.toStdString() << ")";
    SV* sv0 = new SV( ScalarSymmetryMapper::create( oss.str(), -10, 10), QIcon(":/icons/SYMMETRY"));
    ActionVisualise* act = new ActionVisualise(sv0);
    act->setPurgeOnEvent( FaceTools::Action::ORIENTATION_CHANGE);
    addAction( act);
}   // end ctor

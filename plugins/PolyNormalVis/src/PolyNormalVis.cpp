/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#include <PolyNormalVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ScalarVisualisation.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/SurfaceMapper.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FaceTools::Vis::ScalarVisualisation;
using FaceTools::Action::Event;
using r3d::Vec3f;
using r3d::Mat3f;


namespace {

using MappingFn = std::function<float( const Vec3f &fnorm, const Mat3f &T)>;

class PolyNormalVisualisation : public ScalarVisualisation
{
public:
    PolyNormalVisualisation( const std::string& label, float minv, float maxv, const MappingFn &fn)
        : ScalarVisualisation( label, true/*poly data*/, minv, maxv, 0.05f), _fn(fn)
    {
        setNumColours(20);
        rebuild();
    }   // end ctor

    const char *name() const override { return "PolyNormalVisualisation";}

protected:
    vtkSmartPointer<vtkFloatArray> mapMetrics( const FV *fv) override
    {
        const FM *fm = fv->data();
        fm->lockForRead();
        const r3d::Mesh &mesh = fm->mesh();
        Mat3f R = fm->transformMatrix().block<3,3>(0,0);
        const r3dvis::SurfaceMapper smapper(
                [&]( int fid, size_t)
                {
                    const Vec3f fvec = mesh.calcFaceNorm( fid);
                    return _fn( fvec, R);
                },
                true, 1);
        vtkSmartPointer<vtkFloatArray> arr = smapper.makeArray( mesh, mesh.hasMaterials());
        fm->unlock();
        return arr;
    }   // end mapMetrics

private:
    const MappingFn _fn;
};  // end class

}   // end namespace


class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( PolyNormalVisualisation *sv, const QIcon &i) : ActionVisualise( sv->label(), i, sv)
    {
        addPurgeEvent( Event::MESH_CHANGE);
        //addRefreshEvent( Event::SURFACE_DATA_CHANGE);
        addRefreshEvent( Event::VIEW_CHANGE);
    }   // end ctor

    QString attachToMenu() override { return "Surface Mapping";}
    //QString attachToToolBar() override { return "Surface Mapping";}
};  // end VisAction


Cliniface::PolyNormalVis::PolyNormalVis()
{
    PolyNormalVisualisation *svx = new PolyNormalVisualisation( "Right (+X)\nOrientation", -1, 1,
                      []( const Vec3f &f, const Mat3f &R) { return std::max(-1.0f, std::min( f.dot(R.col(0)), 1.0f));});
    PolyNormalVisualisation *svy = new PolyNormalVisualisation( "Up (+Y)\nOrientation", -1, 1,
                      []( const Vec3f &f, const Mat3f &R) { return std::max(-1.0f, std::min( f.dot(R.col(1)), 1.0f));});
    PolyNormalVisualisation *svz = new PolyNormalVisualisation( "Forward (+Z)\nOrientation", -1, 1,
                      []( const Vec3f &f, const Mat3f &R) { return std::max(-1.0f, std::min( f.dot(R.col(2)), 1.0f));});

    appendPlugin( new VisAction( svx, QIcon(":/icons/X")));
    appendPlugin( new VisAction( svy, QIcon(":/icons/Y")));
    appendPlugin( new VisAction( svz, QIcon(":/icons/Z")));
}   // end ctor

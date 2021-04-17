/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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
#include <FaceTools/Vis/ColourVisualisation.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/SurfaceMapper.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FaceTools::Vis::ColourVisualisation;
using FaceTools::Action::Event;
using r3d::Vec3f;
using r3d::Mat3f;


namespace {

using MappingFn = std::function<float( const Vec3f &fnorm, const Mat3f &T)>;

class NormalVis : public ColourVisualisation
{
public:
    NormalVis( const QString& label, const MappingFn &fn)
        : ColourVisualisation( label, -1, 1, 0.05f), _fn(fn)
    {
        setNumColours(20);
        rebuildColourMapping();
    }   // end ctor

    float minAllowedOpacity() const override { return 0.1f;}

    void refresh( FV *fv) override
    {
        const r3d::Mesh &mesh = fv->data()->mesh();
        const Mat3f R = fv->data()->transformMatrix().block<3,3>(0,0);
        auto arr = r3dvis::FaceSurfaceMapper( [&]( int i, size_t) { return _fn( mesh.calcFaceNorm(i), R);}, 1).makeArray( mesh, label().toStdString().c_str());
        fv->addCellsArray( arr);
    }   // end refresh

    void show( FV *fv) override
    {
        fv->setTextured(false);
        fv->setActiveCellScalars( label().toStdString().c_str());
    }   // end show

    void hide( FV *fv) override
    {
        fv->setActiveCellScalars("");
        fv->setTextured(true);
    }   // end hide

private:
    const MappingFn _fn;
};  // end class

}   // end namespace


Cliniface::PolyNormalVis::PolyNormalVis()
{
    const auto bound = []( float v){ return std::max( -1.0f, std::min( v, 1.0f));};
    NormalVis *svx = new NormalVis( "Orientation\nSagittal",
                [&]( const Vec3f &f, const Mat3f &R) { return bound( f.dot(R.col(0)));});
    NormalVis *svy = new NormalVis( "Orientation\nTransverse",
                [&]( const Vec3f &f, const Mat3f &R) { return bound( f.dot(R.col(1)));});
    NormalVis *svz = new NormalVis( "Orientation\nCoronal",
                [&]( const Vec3f &f, const Mat3f &R) { return bound( f.dot(R.col(2)));});

    using FaceTools::Action::ActionColourVis;
    const QString MENU_NAME = "Surface Mapping";
    appendPlugin( new ActionColourVis( svx, QIcon(":/icons/X"), MENU_NAME));
    appendPlugin( new ActionColourVis( svy, QIcon(":/icons/Y"), MENU_NAME));
    appendPlugin( new ActionColourVis( svz, QIcon(":/icons/Z"), MENU_NAME));
}   // end ctor

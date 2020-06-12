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

#include <AsymmetryVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ScalarVisualisation.h>
#include <FaceTools/FaceModelSymmetry.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/SurfaceMapper.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FaceTools::Vis::ScalarVisualisation;
using FaceTools::Action::Event;

namespace {

using MappingFn = std::function<float( const r3d::Vec4f&)>;

class AsymmetryVisualisation : public ScalarVisualisation
{
public:
    AsymmetryVisualisation( const std::string& label, float minv, float maxv, const MappingFn &fn)
        : ScalarVisualisation( label, false/*vertex data*/, minv, maxv, 0.5f, 2), _fn(fn)
    {
        setVisibleRange( -5.0, 5.0);
        setMinColour( QColor( 0, 0, 160));
        setMaxColour( QColor( 160, 0, 0));
        setNumColours( 5);  // 3 mm bands
        rebuild();
    }   // end ctor

    const char *name() const override { return "AsymmetryVisualisation";}

    bool isAvailable( const FV *fv) const override
    {
        return FaceTools::FaceModelSymmetry::vals(fv->data()) != nullptr;
    }   // end isAvailable

protected:
    vtkSmartPointer<vtkFloatArray> mapMetrics( const FV *fv) override
    {
        const FM *fm = fv->data();
        fm->lockForRead();
        FaceTools::FaceModelSymmetry::RPtr smap = FaceTools::FaceModelSymmetry::vals(fm);
        const r3dvis::SurfaceMapper smapper( [&]( int vidx, size_t){ return _fn( smap->at(vidx));}, false, 1);
        vtkSmartPointer<vtkFloatArray> arr = smapper.makeArray( fm->mesh(), fm->mesh().hasMaterials());
        fm->unlock();
        return arr;
    }   // end mapMetrics

private:
    const MappingFn _fn;
};  // end class

}   // end namespace


using Cliniface::AsymmetryVis;


class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( AsymmetryVisualisation *sv, const QIcon& i) : ActionVisualise( sv->label(), i, sv)
    {
        addPurgeEvent( Event::MESH_CHANGE);
    }   // end ctor

    QString attachToMenu() override { return "Surface Mapping";}
    QString attachToToolBar() override { return "Surface Mapping";}
};  // end VisAction


AsymmetryVis::AsymmetryVis()
{
    const QString nm = "Asymmetry [%1] (" + FM::LENGTH_UNITS + ")";

    AsymmetryVisualisation *vT = new AsymmetryVisualisation( nm.arg("All").toStdString(), -21, 21, []( const r3d::Vec4f& v){ return v[3];});

    AsymmetryVisualisation *vX = new AsymmetryVisualisation( nm.arg("X").toStdString(), -21, 21, []( const r3d::Vec4f& v){ return v[0];});
    AsymmetryVisualisation *vY = new AsymmetryVisualisation( nm.arg("Y").toStdString(), -21, 21, []( const r3d::Vec4f& v){ return v[1];});
    AsymmetryVisualisation *vZ = new AsymmetryVisualisation( nm.arg("Z").toStdString(), -21, 21, []( const r3d::Vec4f& v){ return v[2];});

    appendPlugin( new VisAction( vT, QIcon(":/icons/SYMMETRY")));
    appendPlugin( new VisAction( vX, QIcon(":/icons/SYMMETRY_X")));
    appendPlugin( new VisAction( vY, QIcon(":/icons/SYMMETRY_Y")));
    appendPlugin( new VisAction( vZ, QIcon(":/icons/SYMMETRY_Z")));
}   // end ctor

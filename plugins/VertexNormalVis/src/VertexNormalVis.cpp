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

#include <VertexNormalVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/VectorVisualisation.h>
#include <FaceTools/FaceModelCurvature.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/SurfaceMapper.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FMC = FaceTools::FaceModelCurvature;
using FaceTools::Vis::VectorVisualisation;
using FaceTools::Action::Event;

namespace {

class VertexNormalVisualisation : public VectorVisualisation
{
public:
    VertexNormalVisualisation( const std::string& label) : VectorVisualisation( label, false) {}

    const char *name() const override { return "VertexNormalVisualisation";}

protected:
    bool isAvailable( const FV *fv) const override { return FMC::rmetrics( fv->data()) != nullptr;}

    vtkSmartPointer<vtkFloatArray> mapMetrics( const FV *fv) override
    {
        const FM *fm = fv->data();
        fm->lockForRead();
        FMC::RPtr cmap = FMC::rmetrics( fm);
        const r3dvis::SurfaceMapper smapper( [&]( int id, size_t k){ return cmap->vertexNormal( id)[k];}, false, 3);
        vtkSmartPointer<vtkFloatArray> arr = smapper.makeArray( fm->mesh(), fm->mesh().hasMaterials());
        fm->unlock();
        return arr;
    }   // end mapMetrics
};  // end class

}   // end namespace


class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( VertexNormalVisualisation *sv, const QIcon &i) : ActionVisualise( sv->label(), i, sv)
    {
        addPurgeEvent( Event::MESH_CHANGE);
        addRefreshEvent( Event::SURFACE_DATA_CHANGE);
    }   // end ctor

    QString attachToMenu() override { return "Surface Mapping";}    
    QString attachToToolBar() override { return "Surface Mapping";}
};  // end VisAction


Cliniface::VertexNormalVis::VertexNormalVis()
{
    VertexNormalVisualisation *vis = new VertexNormalVisualisation( "Vector mapping");
    appendPlugin( new VisAction( vis, QIcon(":/icons/VECTOR")));
}   // end ctor

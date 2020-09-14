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

#include <CurvatureVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ScalarVisualisation.h>
#include <FaceTools/FaceModelCurvature.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/SurfaceMapper.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FMC = FaceTools::FaceModelCurvature;
using FaceTools::Vis::ScalarVisualisation;
using FaceTools::Action::Event;

namespace {

using MappingFn = std::function<float( const r3d::CurvatureMetrics&, int)>;

class CurvatureVisualisation : public ScalarVisualisation
{
public:
    CurvatureVisualisation( const std::string& label, float minv, float maxv, const MappingFn& fn)
        : ScalarVisualisation( label, false/*vertex data*/, minv, maxv, 5.0f), _fn(fn) {}

    const char *name() const override { return "CurvatureVisualisation";}

    bool isAvailable( const FV *fv) const override { return FMC::rmetrics( fv->data()) != nullptr;}

protected:
    vtkSmartPointer<vtkFloatArray> mapMetrics( const FV *fv) override
    {
        const FM *fm = fv->data();
        fm->lockForRead();
        const r3d::Mesh &mesh = fm->mesh();
        FMC::RPtr cmap = FMC::rmetrics( fm);
        r3d::CurvatureMetrics cmetrics( *cmap);
        const r3dvis::SurfaceMapper smapper( [&]( int id, size_t){ return _fn( cmetrics, id);}, false, 1);
        vtkSmartPointer<vtkFloatArray> arr = smapper.makeArray( mesh, mesh.hasMaterials());
        fm->unlock();
        return arr;
    }   // end mapMetrics

private:
    const MappingFn _fn;
};  // end class

}   // end namespace


using Cliniface::CurvatureVis;

class VisAction : public FaceTools::Action::ActionVisualise
{
public:
    VisAction( CurvatureVisualisation *cv, const QIcon &i) : ActionVisualise( cv->label(), i, cv)
    {
        addPurgeEvent( Event::MESH_CHANGE);
        addRefreshEvent( Event::SURFACE_DATA_CHANGE | Event::VIEW_CHANGE);
    }   // end ctor

    QString attachToMenu() override { return "Surface Mapping";}
    QString attachToToolBar() override { return "Surface Mapping";}
};  // end VisAction


CurvatureVis::CurvatureVis()
{
    CurvatureVisualisation *cv0 = new CurvatureVisualisation( "Mean\nCurvature (degs)", -90, 90,
                      []( const r3d::CurvatureMetrics& m, int i)
                      { return 90.0f * (m.vertexKP1FirstOrder(i) + m.vertexKP2FirstOrder(i))/2;});
    cv0->setVisibleRange( -45, 45);
    cv0->setMinColour( QColor( 0, 0, 160));
    cv0->setMaxColour( QColor( 160, 0, 0));
    cv0->setNumColours( 15);    // 6 degrees per band
    cv0->setNumStepSize( 2);
    cv0->rebuild();

    CurvatureVisualisation *cv1 = new CurvatureVisualisation( "Absolute\nCurvature (degs)", 0, 90,
                      []( const r3d::CurvatureMetrics& m, int i)
                      { return 45.0f * (fabsf(m.vertexKP1FirstOrder(i)) + fabsf(m.vertexKP2FirstOrder(i)));});
    cv1->setVisibleRange( 0, 45);
    cv1->setMinColour( QColor( 255, 255, 255));
    cv1->setMaxColour( QColor( 160, 0, 160));
    cv1->setNumColours( 9);    // 5 degrees per band
    cv1->rebuild();

    appendPlugin( new VisAction( cv0, QIcon(":/icons/MEAN")));
    appendPlugin( new VisAction( cv1, QIcon(":/icons/ABS")));
}   // end ctor

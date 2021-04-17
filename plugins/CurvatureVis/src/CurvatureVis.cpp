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

#include <CurvatureVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ColourVisualisation.h>
#include <FaceTools/FaceModelCurvatureStore.h>
#include <FaceTools/FaceModel.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FMCS = FaceTools::FaceModelCurvatureStore;
using FaceTools::Vis::ColourVisualisation;
using FaceTools::Action::Event;

namespace {

class CurvatureVisualisation : public ColourVisualisation
{
public:
    CurvatureVisualisation( const QString& label, float minv, float maxv)
        : ColourVisualisation( label, minv, maxv, 5.0f) {}

    float minAllowedOpacity() const override { return 0.1f;}

    void refresh( FV *fv) override { fv->addPointsArray( arrfn(fv->data()));}

    void show( FV *fv) override
    {
        fv->setTextured(false);
        fv->setActivePointScalars( arrfn(fv->data())->GetName());
    }   // end show

    void hide( FV *fv) override
    {
        fv->setActivePointScalars("");
        fv->setTextured(true);
    }   // end hide

protected:
    bool isAvailable( const FV *fv) const override { return FMCS::rvals( *fv->data()) != nullptr;}
    virtual vtkSmartPointer<vtkFloatArray> arrfn( const FM*) = 0;
};  // end class


class MeanCurvatureVisualisation : public CurvatureVisualisation
{
public:
    MeanCurvatureVisualisation() : CurvatureVisualisation( "Curvature\nMean (degrees)", -90, 90)
    {
        setVisibleRange( -45, 45);
        setMinColour( Qt::blue);
        setMaxColour( Qt::red);
        setNumColours( 15);    // 6 degrees per band
        setNumStepSize( 1);
        rebuildColourMapping();
    }   // end ctor

protected:
    vtkSmartPointer<vtkFloatArray> arrfn( const FM *fm) override { return FMCS::rvals( *fm)->meanArray();}
};  // end class


class AbsCurvatureVisualisation : public CurvatureVisualisation
{
public:
    AbsCurvatureVisualisation() : CurvatureVisualisation( "Curvature\nAbsolute (degrees)", 0, 90)
    {
        setVisibleRange( 0, 45);
        setMinColour( Qt::white); // White
        setMaxColour( Qt::darkMagenta);
        setNumColours( 9);    // 5 degrees per band
        rebuildColourMapping();
    }   // end ctor

protected:
    vtkSmartPointer<vtkFloatArray> arrfn( const FM *fm) override { return FMCS::rvals( *fm)->absArray();}
};  // end class

}   // end namespace


using Cliniface::CurvatureVis;


CurvatureVis::CurvatureVis()
{
    using FaceTools::Action::ActionColourVis;
    const QString MENU_NAME = "Colour Mapping";
    const QString TOOLBAR_NAME = "Colour Mapping";
    appendPlugin( new ActionColourVis(
          new MeanCurvatureVisualisation, QIcon(":/icons/MEAN"), MENU_NAME, TOOLBAR_NAME));
    appendPlugin( new ActionColourVis(
          new AbsCurvatureVisualisation, QIcon(":/icons/ABS"), MENU_NAME));
}   // end ctor

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

#include <AsymmetryVis.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ColourVisualisation.h>
#include <FaceTools/FaceModelSymmetryStore.h>
#include <FaceTools/FaceModel.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FMSS = FaceTools::FaceModelSymmetryStore;
using FaceTools::Vis::ColourVisualisation;
using FaceTools::Action::Event;

namespace {

using Fn = std::function<vtkSmartPointer<vtkFloatArray>( const FM*)>;

class AsymVis : public ColourVisualisation
{
public:
    AsymVis( const QString& label, Fn fn) : ColourVisualisation( label, -21, 21, 0.5f, 1), _arrfn(fn)
    {
        setVisibleRange( -5.0, 5.0);
        setMinColour( Qt::blue);
        setMaxColour( Qt::red);
        setNumColours( 5);  // 2 mm bands
        rebuildColourMapping();
    }   // end ctor

    float minAllowedOpacity() const override { return 0.1f;}

    void refresh( FV *fv) override { fv->addPointsArray( _arrfn(fv->data()));}

    void show( FV *fv) override
    {
        fv->setTextured(false);
        fv->setActivePointScalars( _arrfn(fv->data())->GetName());
    }   // end show

    void hide( FV *fv) override
    {
        fv->setActivePointScalars("");
        fv->setTextured(true);
    }   // end hide

protected:
    bool isAvailable( const FV *fv) const override { return FMSS::isMapped( fv->data());}

private:
    const Fn _arrfn;
};  // end class

}   // end namespace


using Cliniface::AsymmetryVis;


AsymmetryVis::AsymmetryVis()
{
    const QString nm = "Asymmetry\n%1 (mm)";
    AsymVis *vT = new AsymVis( nm.arg("All Dimensions"), []( const FM *fm){ return FMSS::vals(fm)->allArray();});
    AsymVis *vX = new AsymVis( nm.arg("Horizontal"), []( const FM *fm){ return FMSS::vals(fm)->xArray();});
    AsymVis *vY = new AsymVis( nm.arg("Vertical"), []( const FM *fm){ return FMSS::vals(fm)->yArray();});
    AsymVis *vZ = new AsymVis( nm.arg("Depth"), []( const FM *fm){ return FMSS::vals(fm)->zArray();});

    using FaceTools::Action::ActionColourVis;
    const QString MENU_NAME = "Colour Mapping";
    const QString TOOLBAR_NAME = "Colour Mapping";
    appendPlugin( new ActionColourVis( vT, QIcon(":/icons/SYMMETRY"), MENU_NAME, TOOLBAR_NAME));
    appendPlugin( new ActionColourVis( vX, QIcon(":/icons/SYMMETRY_X"), MENU_NAME));
    appendPlugin( new ActionColourVis( vY, QIcon(":/icons/SYMMETRY_Y"), MENU_NAME));
    appendPlugin( new ActionColourVis( vZ, QIcon(":/icons/SYMMETRY_Z"), MENU_NAME));
}   // end ctor

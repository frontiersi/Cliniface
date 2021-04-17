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

#include <DeltaVis.h>
#include <DeltaPerpView.h>
#include <FaceTools/ModelSelect.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Vis/ColourVisualisation.h>
#include <FaceTools/FaceModel.h>
#include <r3dvis/VtkTools.h>
using FM = FaceTools::FM;
using FV = FaceTools::Vis::FaceView;
using FaceTools::Vis::ColourVisualisation;
using FaceTools::Action::Event;
using FMDS = FaceTools::FaceModelDeltaStore;
using FMD = FaceTools::FaceModelDelta;
using MS = FaceTools::ModelSelect;
using Cliniface::DeltaPerpView;

namespace {

std::shared_ptr<const FMD> fmd( const FV *fv)
{
    assert(fv);
    FM::RPtr tgt = fv->rdata();
    FM::RPtr src = MS::otherModelScopedRead( tgt.get());
    return FMDS::vals( tgt.get(), src.get());
}   // end fmd


class DeltaColourVis : public ColourVisualisation
{
public:
    DeltaColourVis( const QString& label, float minv, float maxv)
        : ColourVisualisation( label, minv, maxv, 0.5f, 1)
    {
        setNumColours( 5);  // 2 mm bands
    }   // end ctor

    float minAllowedOpacity() const override { return 0.1f;}

    bool isAvailable( const FV *fv) const override
    {
        FM::RPtr tgt = fv->rdata();
        FM::RPtr src = MS::otherModelScopedRead( tgt.get());
        return src && FMDS::has( tgt.get(), src.get());
    }   // end isAvailable
};  // end class


using Fn = std::function<vtkFloatArray*( const FMD*)>;

class DeltaScalarVis : public DeltaColourVis
{
public:
    DeltaScalarVis( const QString& label, float minv, float maxv, const Fn &fn)
        : DeltaColourVis( label, minv, maxv), _arrfn(fn)
    {
        setMinColour( Qt::white);
        setMaxColour( Qt::darkMagenta);
        setVisibleRange( 0, 5);
        rebuildColourMapping();
    }   // end ctor

    void refresh( FV *fv) override { fv->addPointsArray( _arrfn(fmd(fv).get()));}

    void show( FV *fv) override
    {
        fv->setTextured(false);
        fv->setActivePointScalars( _arrfn(fmd(fv).get())->GetName());
    }   // end show

    void hide( FV *fv) override
    {
        fv->setActivePointScalars("");
        fv->setTextured(true);
    }   // end hide

private:
    const Fn _arrfn;
};  // end class


class DeltaVectorVis : public DeltaColourVis
{
public:
    DeltaVectorVis( const QString &lb) : DeltaColourVis( lb, -21, 21)
    {
        setMinColour( Qt::blue);
        setMaxColour( Qt::red);
        setVisibleRange( -5.0f, 5.0f);
        rebuildColourMapping();
    }   // end ctor

    void show( FV *fv) override
    {
        fv->setTextured(true);
        if ( _views.count(fv) > 0)
            _views.at(fv)->setVisible(true);
    }   // end show

    void hide( FV *fv) override
    {
        if ( _views.count(fv) > 0)
            _views.at(fv)->setVisible(false);
    }   // end hide

    // So the arrows can show through immediately
    float maxAllowedOpacity() const override { return 0.99f;}

    void refresh( FV *fv) override
    {
        if ( _views.count(fv) == 0)
            _views[fv] = DeltaPerpView::create(fv);
    }   // end refresh

    void syncTransform( const FV *fv) override
    {
        assert( _views.count(fv) > 0);
        _views.at(fv)->syncTransform();
    }   // end syncTransform

    void purge( const FV *fv) override
    {
        if ( _views.count(fv) > 0)
        {
            _views.erase(fv);
            FM::RPtr src = MS::otherModelScopedRead( fv->data());
            if ( src)
            {
                for ( FV *f : src->fvs())
                {
                    hide(f);
                    _views.erase(f);
                }   // end for
            }   // end if
        }   // end if
    }   // end purge

    void refreshColourMap( const FV *fv) override
    {
        assert( _views.count(fv) > 0);
        _views.at(fv)->refreshColourMap( lookupTable(), minVisible(), maxVisible());
    }   // end refreshColourMap

private:
    std::unordered_map<const FV*, DeltaPerpView::Ptr> _views;
};  // end class

}   // end namespace


Cliniface::DeltaVis::DeltaVis()
{
    ColourVisualisation *dsva = new DeltaVectorVis( "Shape Difference (mm)");

    ColourVisualisation *dsvp = new DeltaScalarVis( "Shape Difference\nPerpendicular (mm)", -21, 21,
                                    []( const FMD *fmd){ return fmd->perpArray();});
    dsvp->setMinColour( Qt::blue);
    dsvp->setMaxColour( Qt::red);
    dsvp->setVisibleRange( -5.0f, 5.0f);
    dsvp->rebuildColourMapping();

    ColourVisualisation *dsvd = new DeltaScalarVis( "Shape Difference\nMagnitude (mm)", -21, 21,
                                    []( const FMD *fmd){ return fmd->smagArray();});
    dsvd->setMinColour( Qt::blue);
    dsvd->setMaxColour( Qt::red);
    dsvd->setVisibleRange( -5.0f, 5.0f);
    dsvd->rebuildColourMapping();

    ColourVisualisation *dsvs = new DeltaScalarVis( "Shape Difference\nTangential (mm)", 0, 40,
                                    []( const FMD *fmd){ return fmd->angdArray();});

    using FaceTools::Action::ActionColourVis;
    const QString MENU_NAME = "Colour Mapping";
    const QString TOOLBAR_NAME = "Colour Mapping";

    auto av0 = new ActionColourVis( dsva, QIcon(":/icons/DELTA_VECTOR"), MENU_NAME, TOOLBAR_NAME);
    auto av1 = new ActionColourVis( dsvd, QIcon(":/icons/DELTA_ABS"), MENU_NAME, TOOLBAR_NAME);
    auto av2 = new ActionColourVis( dsvp, QIcon(":/icons/DELTA_PERPM"), MENU_NAME, TOOLBAR_NAME);
    auto av3 = new ActionColourVis( dsvs, QIcon(":/icons/DELTA_ANGLE"), MENU_NAME, TOOLBAR_NAME);

    static const QString ttip = QObject::tr( "Difference in face shape (not scale) with the non-selected model\n");
    av0->setToolTip( ttip + QObject::tr("as vectors with colours showing degree of perpendicular\nprotrusion (red) or retrusion (blue)."));
    av1->setToolTip( ttip + QObject::tr("with colour showing signed magnitude of difference."));
    av2->setToolTip( ttip + QObject::tr("with colour showing perpendicular protrusion (red) or retrusion (blue)."));
    av3->setToolTip( ttip + QObject::tr("with colour showing the degree of transverse difference\n(i.e., difference tangential to the surface)."));
    appendPlugin( av0);
    appendPlugin( av1);
    appendPlugin( av2);
    appendPlugin( av3);
}   // end ctor

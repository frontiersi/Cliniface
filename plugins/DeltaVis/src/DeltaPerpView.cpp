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

#include <DeltaPerpView.h>
#include <r3dvis/VtkActorCreator.h>
#include <r3dvis/VtkTools.h>
#include <FaceTools/Vis/ColourVisualisation.h>
#include <FaceTools/FaceModelViewer.h>
#include <FaceTools/ModelSelect.h>
#include <FaceTools/FaceModel.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
using Cliniface::DeltaPerpView;
using CV = FaceTools::Vis::ColourVisualisation;
using FaceTools::Vis::FV;
using FM = FaceTools::FaceModel;
using MS = FaceTools::ModelSelect;
using FMD = FaceTools::FaceModelDelta;
using FMDS = FaceTools::FaceModelDeltaStore;


DeltaPerpView::Ptr DeltaPerpView::create( FV *fv)
{
    return Ptr( new DeltaPerpView( fv), []( const DeltaPerpView *d){ delete d;});
}   // end create


DeltaPerpView::DeltaPerpView( FV *fv) : _fv(fv), _isvis(false)
{
    FM::RPtr fmb = MS::otherModelScopedRead( fv->data());
    assert( fmb);
    std::shared_ptr<const FMD> fmd = FMDS::vals( fv->data(), fmb.get());
    _sactor = r3dvis::VtkActorCreator::generateSurfaceActor( fmd->alignedSourceMask());
    //_sactor->SetPickable(false);

    // Set the vectors array for the source actor mask 
    CV::addPointsArray( _sactor, fmd->vecsArray());
    CV::setActivePointVectors( _sactor, fmd->vecsArray()->GetName());
    // And how the vectors should be scaled...
    CV::addPointsArray( _sactor, fmd->sclsArray());
    CV::setActivePointScalars( _sactor, fmd->sclsArray()->GetName());
 
    _vfield = r3dvis::VtkVectorField::create( r3dvis::getPolyData(_sactor));
    _vfield->setScaleFactor(1.0);
    _vfield->setOpacity( 0.99);
    _vfield->setPickable(false);
}   // end ctor


DeltaPerpView::~DeltaPerpView() {}


void DeltaPerpView::setVisible( bool v)
{
    vtkProp *prop = _vfield->prop();
    _isvis = v;
    if ( v)
        _fv->viewer()->add(prop);
    else
        _fv->viewer()->remove(prop);
}   // end setVisible


void DeltaPerpView::syncTransform()
{
    _vfield->pokeTransform( _fv->transformMatrix());
    //_sactor->PokeMatrix( _fv->transformMatrix());
}   // end syncTransform


void DeltaPerpView::refreshColourMap( const vtkLookupTable *lt, float minVis, float maxVis)
{
    //_sactor->GetMapper()->SetLookupTable( const_cast<vtkLookupTable*>(lt));
    //_sactor->GetMapper()->SetScalarRange( minVis, maxVis);
    _vfield->setColourMap( lt, minVis, maxVis);
}   // end refreshColourMap

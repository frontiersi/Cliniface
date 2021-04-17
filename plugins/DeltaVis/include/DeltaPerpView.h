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

#ifndef Cliniface_Plugin_DeltaPerpView_h
#define Cliniface_Plugin_DeltaPerpView_h

#include "Cliniface_Plugin_DeltaVis_Export.h"
#include <r3dvis/VtkVectorField.h>
#include <FaceTools/FaceModelDeltaStore.h>

namespace Cliniface {

class Cliniface_Plugin_DeltaVis_EXPORT DeltaPerpView
{
public:
    using Ptr = std::shared_ptr<DeltaPerpView>;
    static Ptr create( FaceTools::Vis::FV*);

    inline bool isVisible() const { return _isvis;}
    void setVisible( bool);

    void syncTransform();
    void refreshColourMap( const vtkLookupTable*, float, float);

private:
    FaceTools::Vis::FV *_fv;
    bool _isvis;
    r3dvis::VtkVectorField::Ptr _vfield;
    vtkSmartPointer<vtkActor> _sactor;   // Source surface for the vector field

    explicit DeltaPerpView( FaceTools::Vis::FV*);
    ~DeltaPerpView();
    DeltaPerpView( const DeltaPerpView&) = delete;
    void operator=( const DeltaPerpView&) = delete;
};  // end class

}   // end namespace

#endif


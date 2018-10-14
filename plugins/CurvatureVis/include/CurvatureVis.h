/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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

#ifndef Cliniface_Plugin_CurvatureVis_h
#define Cliniface_Plugin_CurvatureVis_h

#include "Cliniface_Plugin_CurvatureVis_Export.h"
#include <FaceActionGroup.h>   // FaceTools

namespace Cliniface {

class Cliniface_Plugin_CurvatureVis_EXPORT CurvatureVis : public FaceTools::Action::FaceActionGroup
{ Q_OBJECT
Q_PLUGIN_METADATA( IID FaceToolsPluginFaceActionInterface_iid)
Q_INTERFACES( FaceTools::Action::FaceActionInterface)
public:
    CurvatureVis();
};  // end class

}   // end namespace

#endif

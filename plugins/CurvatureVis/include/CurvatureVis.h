/************************************************************************
 * Copyright (C) 2019 SIS Research Ltd & Richard Palmer
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
#include <Cliniface_Config.h>   // Cliniface configuration header
#include <QTools/PluginInterface.h>

namespace Cliniface {

class Cliniface_Plugin_CurvatureVis_EXPORT CurvatureVis : public QTools::PluginInterface
{ Q_OBJECT
Q_PLUGIN_METADATA( IID QToolsPluginInterface_iid)
Q_INTERFACES( QTools::PluginInterface)
public:
    CurvatureVis();
    std::string applicationCode() const { return APP_PLUGIN_TOKEN;}
};  // end class

}   // end namespace

#endif

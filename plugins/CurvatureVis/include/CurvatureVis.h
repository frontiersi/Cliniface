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

#ifndef FACE_APP_SHARED_PLUGIN_FIRST_ORDER_MIN_CURVATURE_VIS_H
#define FACE_APP_SHARED_PLUGIN_FIRST_ORDER_MIN_CURVATURE_VIS_H

#include <VisualisationAction.h>   // FaceTools
#include "plugins_3dfast_curvature_Export.h"

namespace FaceApp {
namespace Visualisation {

class plugins_3dfast_curvature_EXPORT FirstOrderMinCurvature : public FaceTools::VisualisationAction
{ Q_OBJECT
public:
    FirstOrderMinCurvature();

    virtual const QIcon* getIcon() const { return &_icon;}
    virtual QString getDisplayName() const { return "kp2' Curvature";}

    virtual bool isAvailable( const FaceTools::FaceModel*) const;
    virtual vtkSmartPointer<vtkActor> makeActor( const FaceTools::FaceModel*);
    virtual bool allowScalarVisualisation( float& minv, float& maxv) const;

private:
    const QIcon _icon;
    float _minv, _maxv;
};  // end class

}   // end namespace
}   // end namespace

#endif

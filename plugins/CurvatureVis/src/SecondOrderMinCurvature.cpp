#include <SecondOrderMinCurvature.h>
#include <PolySurfaceCurvScalarMapper.h>
#include <FaceView.h>
using FaceApp::Visualisation::SecondOrderMinCurvature;
using RFeatures::ObjModelCurvatureMetrics;
using RFeatures::ObjModel;
using FaceTools::FaceView;
typedef boost::unordered_map<int,int> IntIntMap;

SecondOrderMinCurvature::SecondOrderMinCurvature() : _icon(":GRADIENTS_ICON"), _minv(0), _maxv(0) { init();}

namespace {
class SecondOrderMinCurvMapper : public RVTK::PolySurfaceCurvScalarMapper
{
public:
    SecondOrderMinCurvMapper( const ObjModelCurvatureMetrics::Ptr cm, vtkActor* actor, const IntIntMap* lookup, const std::string& metricName)
        : RVTK::PolySurfaceCurvScalarMapper(cm, actor, lookup, metricName)
    {}  // end ctor

protected:
    virtual float getCurvMetric( int faceIdx) const
    {
        return (float)_cmetrics->getFaceKP2ThirdOrder(faceIdx);
    }   // end getCurvMetric
};  // end class
}   // end namespace


// public
bool SecondOrderMinCurvature::isAvailable( const FaceView* fview) const
{
    return fview->getModel()->getCurvatureMetrics() != NULL;
}   // end isAvailable


// public
void SecondOrderMinCurvature::mapActor( FaceView* fview, vtkActor* actor, const IntIntMap* lookup)
{
    const std::string vname = getDisplayName().toStdString();
    mapper( fview->getModel()->getCurvatureMetrics(), actor, lookup, vname);
    mapper.mapActor();
    mapper.getMappedRange( &_minv, &_maxv);

    vtkDataSetAttributes* da = RVTK::getPolyData( actor)->GetCellData();
    da->SetActiveScalars( vname.c_str());
    actor->GetMapper()->SetScalarVisibility(true);
}   // end mapActor


// public
bool SecondOrderMinCurvature::allowScalarVisualisation( float& minv, float& maxv) const
{
    minv = _minv;
    maxv = _maxv;
    return true;
}   // end allowScalarVisualisation

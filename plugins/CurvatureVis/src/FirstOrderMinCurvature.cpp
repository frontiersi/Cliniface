#include <FirstOrderMinCurvature.h>
#include <PolySurfaceCurvScalarMapper.h>
#include <FaceView.h>
using FaceApp::Visualisation::FirstOrderMinCurvature;
using RFeatures::ObjModelCurvatureMetrics;
using RFeatures::ObjModel;
using FaceTools::FaceView;
typedef boost::unordered_map<int,int> IntIntMap;

FirstOrderMinCurvature::FirstOrderMinCurvature() : _icon(":GRADIENTS_ICON"), _minv(0), _maxv(0) { init();}

namespace {
class FirstOrderMinCurvMapper : public RVTK::PolySurfaceCurvScalarMapper
{
public:
    FirstOrderMinCurvMapper( const ObjModelCurvatureMetrics::Ptr cm, vtkActor* actor, const IntIntMap* lookup, const std::string& metricName)
        : RVTK::PolySurfaceCurvScalarMapper(cm, actor, lookup, metricName)
    {}  // end ctor

protected:
    virtual float getCurvMetric( int faceIdx) const
    {
        return (float)_cmetrics->getFaceKP2SecondOrder(faceIdx);
    }   // end getCurvMetric
};  // end class
}   // end namespace


// public
bool FirstOrderMinCurvature::isAvailable( const FaceModel* fmodel) const
{
    return fmodel->getCurvatureMetrics() != NULL;
}   // end isAvailable


// public
void FirstOrderMinCurvature::mapActor( FaceView* fview, vtkActor* actor, const IntIntMap* lookup)
{
    const std::string vname = getDisplayName().toStdString();
    FirstOrderMinCurvMapper mapper( fview->getModel()->getCurvatureMetrics(), actor, lookup, vname);
    mapper.mapActor();
    mapper.getMappedRange( &_minv, &_maxv);

    vtkDataSetAttributes* da = RVTK::getPolyData( actor)->GetCellData();
    da->SetActiveScalars( vname.c_str());
    actor->GetMapper()->SetScalarVisibility(true);
}   // end mapActor


// public
bool FirstOrderMinCurvature::allowScalarVisualisation( float& minv, float& maxv) const
{
    minv = _minv;
    maxv = _maxv;
    return true;
}   // end allowScalarVisualisation


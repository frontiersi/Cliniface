#include <FirstOrderMaxCurvature.h>
#include <PolySurfaceCurvScalarMapper.h>
#include <FaceView.h>
using FaceApp::Visualisation::FirstOrderMaxCurvature;
using RFeatures::ObjModelCurvatureMetrics;
using RFeatures::ObjModel;
using FaceTools::FaceView;
typedef boost::unordered_map<int,int> IntIntMap;

FirstOrderMaxCurvature::FirstOrderMaxCurvature() : _icon(":GRADIENTS_ICON"), _minv(0), _maxv(0) { init();}

namespace {
class FirstOrderMaxCurvMapper : public RVTK::PolySurfaceCurvScalarMapper
{
public:
    FirstOrderMaxCurvMapper( const ObjModelCurvatureMetrics::Ptr cm, vtkActor* actor, const IntIntMap* lookup, const std::string& metricName)
        : RVTK::PolySurfaceCurvScalarMapper(cm, actor, lookup, metricName)
    {}  // end ctor

protected:
    virtual float getCurvMetric( int faceIdx) const
    {
        return (float)_cmetrics->getFaceKP1SecondOrder(faceIdx);
    }   // end getCurvMetric
};  // end class
}   // end namespace


// public
bool FirstOrderMaxCurvature::isAvailable( const FaceView* fview) const
{
    return fview->getModel(fmodel->getCurvatureMetrics() != NULL;
}   // end isAvailable


// public
void FirstOrderMaxCurvature::mapActor( FaceView* fview, vtkActor* actor, const IntIntMap* lookup)
{
    const std::string vname = getDisplayName().toStdString();
    FirstOrderMaxCurvMapper( fview->getModel()->getCurvatureMetrics(), actor, lookup, vname);
    mapper.mapActor();
    mapper.getMappedRange( &_minv, &_maxv);

    vtkDataSetAttributes* da = RVTK::getPolyData( actor)->GetCellData();
    da->SetActiveScalars( vname.c_str());
    actor->GetMapper()->SetScalarVisibility(true);
}   // end mapActor


// public
bool FirstOrderMaxCurvature::allowScalarVisualisation( float& minv, float& maxv) const
{
    minv = _minv;
    maxv = _maxv;
    return true;
}   // end allowScalarVisualisation

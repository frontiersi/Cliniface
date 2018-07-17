#include <CurvatureVis.h>
#include <ActionVisualise.h>        // FaceTools
#include <ScalarVisualisation.h>    // FaceTools


void initResources()
{
    Q_INIT_RESOURCE(cvis_icons);
}   // end initResources


Cliniface::CurvatureVis::CurvatureVis()
{
    typedef FaceTools::Vis::ScalarVisualisation SV;
    typedef RFeatures::ObjModelCurvatureMetrics OMCM;

    SV* sv0 = new SV( "P1 Curvature", QIcon(":/icons/grad_kp1.png"));
    sv0->setMappingFunction( []( const OMCM* m, int f){ return m->faceKP1FirstOrder(f);});

    SV* sv1 = new SV( "P2 Curvature", QIcon(":/icons/grad_kp2.png"));
    sv1->setMappingFunction( []( const OMCM* m, int f){ return m->faceKP2FirstOrder(f);});

    SV* sv2 = new SV( "Mean Curvature", QIcon(":/icons/grad_mean.png"));
    sv2->setMappingFunction( []( const OMCM* m, int f){ return (m->faceKP1FirstOrder(f) + m->faceKP2FirstOrder(f))/2;});

    SV* sv3 = new SV( "Gaussian Curvature", QIcon(":/icons/grad_gaussian.png"));
    sv3->setMappingFunction( []( const OMCM* m, int f){ return m->faceKP1FirstOrder(f) * m->faceKP2FirstOrder(f);});

    SV* sv4 = new SV( "Abs. Curvature", QIcon(":/icons/grad_abs.png"));
    sv4->setMappingFunction( []( const OMCM* m, int f){ return sqrt( (pow(m->faceKP1FirstOrder(f),2) + pow(m->faceKP2FirstOrder(f),2))/2);});

    SV* sv5 = new SV( "Determinant", QIcon(":/icons/grad_determinant.png"));
    sv5->setMappingFunction( []( const OMCM* m, int f){ return m->faceDeterminant(f);});

    using FaceTools::Action::ActionVisualise;
    addAction( new ActionVisualise( sv0));
    addAction( new ActionVisualise( sv1));
    addAction( new ActionVisualise( sv2));
    addAction( new ActionVisualise( sv3));
    addAction( new ActionVisualise( sv4));
    addAction( new ActionVisualise( sv5));
}   // end ctor

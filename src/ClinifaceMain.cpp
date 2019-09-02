/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <ClinifaceMain.h>
#include <ui_ClinifaceMain.h>

#include <AboutDialog.h>

#include <ActionRedo.h>
#include <ActionUndo.h>
#include <ActionSmooth.h>
#include <ActionReflect.h>
#include <ActionCopyView.h>
#include <ActionMoveView.h>
#include <ActionSetFocus.h>
#include <ActionVisualise.h>
#include <ActionFillHoles.h>
#include <ActionUpdateU3D.h>
#include <ActionDetectFace.h>
#include <ActionScaleModel.h>
#include <ActionRotateModel.h>
#include <ActionMapCurvature.h>
#include <ActionInvertNormals.h>
#include <ActionUpdateMeasurements.h>
#include <ActionAlignVertices.h>
#include <ActionAlignICP.h>
#include <ActionAlignLandmarks.h>
#include <ActionCloseFaceModel.h>
#include <ActionExportMetaData.h>
#include <ActionImportMetaData.h>
#include <ActionResetDetection.h>
#include <ActionSaveFaceModel.h>
#include <ActionBackfaceCulling.h>
#include <ActionUpdateThumbnail.h>
#include <ActionSaveAsFaceModel.h>
#include <ActionLoadDirFaceModels.h>
#include <ActionTransformToCentre.h>
#include <ActionCloseAllFaceModels.h>
#include <ActionTransformToStandardPosition.h>

#include <ActionSetSurfaceColour.h>
#include <ActionSetMinScalarColour.h>
#include <ActionSetMaxScalarColour.h>
#include <ActionSetNumScalarColours.h>
#include <ActionChangeSurfaceMappingRange.h>

#include <ReportManager.h>
#include <ModelSelector.h>
#include <FaceActionManager.h>

#include <OutlinesVisualisation.h>
#include <PolyLabelsVisualisation.h>
#include <VertexLabelsVisualisation.h>

#include <QLabel>
#include <QMimeData>
#include <QApplication>
#include <QDesktopWidget>
#include <QWhatsThis>
#include <QWidgetAction>
#include <QVBoxLayout>
using Cliniface::ClinifaceMain;


namespace {
QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, [d](){ d->show(); d->raise(); d->activateWindow();});
    return a;
}   // end connectDialog


QString appName()
{
#ifdef NDEBUG
    return APP_NAME;
#else
    return QString( "%1 (DEBUG)").arg(APP_NAME);
#endif
}   // end appName


// Plugin help documentation is embedded within a single HTML page with header and footer defined by Cliniface.
QString producePluginHelpContent( FaceAction* act)
{
    const QString helpFile = act->helpFile("");
    if ( helpFile.isEmpty())    // No help file defined
        return "";

    const QString dname = act->displayName();

    QString content;
    QTextStream os(&content);

    // Write out the documentation header
    os << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">" << endl
       << "<html>" << endl
       << "<head>" << endl
       << "<title>" << dname << "</title>" << endl
       << "<link rel=\"stylesheet\" type=\"text/css\" href=\"styles/main.css\">" << endl
       << "</head>" << endl
       << "<body id=\"_top\">" << endl
       << "<center><a href=\"https://cliniface.org\"><img src=\"images/cface.svg\" alt=\"Cliniface Logo\" style=\"width:27%;\"/></a></center>" << endl
       << "<center><h1>" << dname << "</h1></center>" << endl
       << "<center><a href=\"index.html\">Back to contents</a></center>" << endl
       << "<hr>" << endl
       << FaceTools::loadTextFromFile(helpFile) << endl // Write out the help file contents
       // Write out the footer
       << "<br>" << endl
       << "<hr>" << endl
       << "<center>" << endl
       << "<a href=\"#_top\">Back to top</a><br>" << endl
       << "<a href=\"index.html\">Back to contents</a>" << endl
       << "<strong><p>&copy; 2018/2019 Spatial Information Systems Research Limited &amp; Richard Palmer</p><strong>" << endl
       << "</center>" << endl
       << "</body>" << endl
       << "</html>" << endl;

    return content;
}   // end producePluginHelpContent

}   // end namespace


void ClinifaceMain::registerActions()
{
    FAM::registerAction( _actionLoadFaceModels);
    FAM::registerAction( _actionLoadDirFaceModels);
    FAM::registerAction( _actionSaveFaceModel);
    FAM::registerAction( _actionSaveAsFaceModel);
    FAM::registerAction( _actionExportPDF);
    FAM::registerAction( _actionCloseFaceModel);
    FAM::registerAction( _actionCloseAllFaceModels);
    FAM::registerAction( _actionExportMetaData);
    FAM::registerAction( _actionImportMetaData);

    FAM::registerAction( _actionRedo);
    FAM::registerAction( _actionUndo);
    FAM::registerAction( _actionEditLandmarks);
    FAM::registerAction( _actionEditPaths);
    FAM::registerAction( _actionVisOutlines);
    FAM::registerAction( _actionVisWireframe);
    FAM::registerAction( _actionVisTexture);
    FAM::registerAction( _actionVisPolyLabels);
    FAM::registerAction( _actionVisVertexLabels);

    FAM::registerAction( new ActionMapCurvature);
    FAM::registerAction( new ActionUpdateU3D);
    FAM::registerAction( _actionOrientCameraToFrontFace);
    FAM::registerAction( _actionOrientCameraToLeftFace);
    FAM::registerAction( _actionOrientCameraToRightFace);
    FAM::registerAction( _actionTransformToStandardPosition);
    FAM::registerAction( _actionTransformToCentre);
    FAM::registerAction( _actionDetectFace);
    FAM::registerAction( _actionResetDetection);
    FAM::registerAction( _actionResetCamera);
    FAM::registerAction( _actionSetParallelProjection);
    FAM::registerAction( _actionSaveScreenshot);
    FAM::registerAction( _actionSmooth);
    FAM::registerAction( _actionReflect);
    FAM::registerAction( _actionNonRigidRegistration);
    FAM::registerAction( _actionToggleAxes);
    FAM::registerAction( _actionToggleCameraActorInteraction);
    FAM::registerAction( _actionSynchroniseCameraMovement);
    FAM::registerAction( _actionMarquee);
    FAM::registerAction( _actionSetFocus);
    FAM::registerAction( _actionRadialSelect);
    FAM::registerAction( _actionDiscardManifold);
    FAM::registerAction( _actionRemoveManifolds);
    FAM::registerAction( _actionCrop);
    FAM::registerAction( _actionMakeLeftFace);
    FAM::registerAction( _actionMakeRightFace);
    FAM::registerAction( _actionAlignLandmarks);
    FAM::registerAction( _actionAlignVertices);
    FAM::registerAction( _actionAlignICP);
    FAM::registerAction( _actionFillHoles);
    FAM::registerAction( _actionSetOpacity);
    FAM::registerAction( _actionBackfaceCulling);

    FAM::registerAction( _actionRenamePath);
    FAM::registerAction( _actionAddPath);
    FAM::registerAction( _actionDeletePath);

    FAM::registerAction( _actionSetMinScalarColour);
    FAM::registerAction( _actionSetMaxScalarColour);
    FAM::registerAction( _actionSetNumScalarColours);
    FAM::registerAction( _actionChangeSurfaceMappingRange);
    FAM::registerAction( _actionToggleScalarLegend);
    FAM::registerAction( _actionRotateX90);
    FAM::registerAction( _actionRotateY90);
    FAM::registerAction( _actionRotateZ90);
    FAM::registerAction( _actionScaleModel);

    FAM::registerAction( _actionInvertNormals);
    FAM::registerAction( _actionToggleStereoRendering);

    connect( _ui->action_VisualisationsToolbar, &QAction::triggered, [=](bool v){ _ui->visToolBar->setVisible(v); _ui->scmapToolBar->setVisible(v);});
    connect( _ui->action_Exit, &QAction::triggered, this, &ClinifaceMain::close);
}   // end registerActions


void ClinifaceMain::createFileMenu()
{
    _ui->menu_File->addAction( _actionLoadFaceModels->qaction());
    _ui->menu_File->addAction( _actionLoadDirFaceModels->qaction());
    _ui->menu_File->addAction( _actionSaveFaceModel->qaction());
    _ui->menu_File->addAction( _actionSaveAsFaceModel->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actionImportMetaData->qaction());
    _ui->menu_File->addAction( _actionExportMetaData->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actionUndo->qaction());
    _ui->menu_File->addAction( _actionRedo->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actionShowScanInfo->qaction());
    _ui->menu_File->addAction( _actionShowModelProperties->qaction());

    if ( FaceTools::Report::ReportManager::isAvailable())
        _ui->menu_File->addAction( _actionExportPDF->qaction());

    _ppoints.set("File", _ui->menu_File, _ui->menu_File->addSeparator());

    _ui->menu_File->addAction( _actionCloseFaceModel->qaction());
    _ui->menu_File->addAction( _actionCloseAllFaceModels->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _ui->action_Exit);
}   // end createFileMenu


// private
void ClinifaceMain::createViewMenu()
{
    _ui->menu_View->addAction( _actionVisTexture->qaction());
    _ui->menu_View->addAction( _actionVisWireframe->qaction());
    _ui->menu_View->addAction( _actionVisOutlines->qaction());
    _ui->menu_View->addSeparator();
    _ui->menu_View->addAction( _actionEditLandmarks->qaction());
    _ui->menu_View->addAction( _actionEditPaths->qaction());
    _ui->menu_View->addAction( _actionRadialSelect->qaction());
    _ui->menu_View->addAction( _actionVisPolyLabels->qaction());
    _ui->menu_View->addAction( _actionVisVertexLabels->qaction());

    QMenu *surfaceMenu = _ui->menu_View->addMenu( "&Scalar Mapping...");
    _ppoints.set("Scalar Mapping", surfaceMenu);
    surfaceMenu->setIcon( QIcon(":/icons/COLOURS"));
    surfaceMenu->addAction( _actionToggleScalarLegend->qaction());
    surfaceMenu->addAction( _actionSetMinScalarColour->qaction());
    surfaceMenu->addAction( _actionSetMaxScalarColour->qaction());
    surfaceMenu->addSeparator();

    _ui->menu_View->addSeparator();

    _ui->menu_View->addAction( _actionBackfaceCulling->qaction());
    _ui->menu_View->addAction( _actionToggleAxes->qaction());
    _ui->menu_View->addAction( _actionSaveScreenshot->qaction());
    _ui->menu_View->addAction( _actionToggleStereoRendering->qaction());
    _ui->menu_View->addAction( _ui->action_VisualisationsToolbar);

    /****** _ui->visToolBar ******/
    _ui->visToolBar->addAction( _actionVisTexture->qaction());
    _ui->visToolBar->addAction( _actionVisWireframe->qaction());
    _ui->visToolBar->addAction( _actionVisOutlines->qaction());
    _ui->visToolBar->addSeparator();
    _ui->visToolBar->addAction( _actionEditLandmarks->qaction());
    _ui->visToolBar->addAction( _actionShowMetrics->qaction());
    _ui->visToolBar->addAction( _actionEditPaths->qaction());
    _ui->visToolBar->addAction( _actionRadialSelect->qaction());
    _ui->visToolBar->addSeparator();
    _ui->visToolBar->addAction( _actionBackfaceCulling->qaction());
    _ui->visToolBar->addWidget( _actionSetOpacity->widget());
    _ui->visToolBar->addAction( _actionToggleAxes->qaction());

    /****** _ui->scmapToolBar ******/
    _ui->scmapToolBar->addWidget( _actionChangeSurfaceMappingRange->widget());
    _ui->scmapToolBar->addWidget( _actionSetNumScalarColours->widget());
    _ui->scmapToolBar->addSeparator();

    /*
    QWidget* empty = new QWidget;
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ui->scmapToolBar->addWidget( empty);
    */
}   // end createViewMenu


// private
void ClinifaceMain::createCameraMenu()
{
    _ui->menu_Camera->addAction( _actionResetCamera->qaction());
    _ui->menu_Camera->addAction( _actionOrientCameraToLeftFace->qaction());
    _ui->menu_Camera->addAction( _actionOrientCameraToFrontFace->qaction());
    _ui->menu_Camera->addAction( _actionOrientCameraToRightFace->qaction());
    _ui->menu_Camera->addSeparator();
    _ui->menu_Camera->addAction( _actionSetParallelProjection->qaction());
    _ui->menu_Camera->addAction( _actionSynchroniseCameraMovement->qaction());
    _ui->menu_Camera->addAction( _actionMarquee->qaction());
}   // end createCameraMenu


// private
void ClinifaceMain::createTransformMenu()
{
    _ui->menu_Transform->addAction( _actionAlignLandmarks->qaction());
    _ui->menu_Transform->addAction( _actionAlignVertices->qaction());
    _ui->menu_Transform->addAction( _actionAlignICP->qaction());
    _ui->menu_Transform->addSeparator();
    _ui->menu_Transform->addAction( _actionTransformToStandardPosition->qaction());
    _ui->menu_Transform->addAction( _actionTransformToCentre->qaction());
    _ui->menu_Transform->addAction( _actionRotateX90->qaction());
    _ui->menu_Transform->addAction( _actionRotateY90->qaction());
    _ui->menu_Transform->addAction( _actionRotateZ90->qaction());
    _ui->menu_Transform->addAction( _actionReflect->qaction());
    _ui->menu_Transform->addAction( _actionScaleModel->qaction());
    _ui->menu_Transform->addSeparator();
    _ui->menu_Transform->addAction( _actionToggleCameraActorInteraction->qaction());
}   // end createTransformMenu


// private
void ClinifaceMain::createGeometryMenu()
{
    _ui->menu_Geometry->addAction( _actionInvertNormals->qaction());
    _ui->menu_Geometry->addAction( _actionFillHoles->qaction());
    _ui->menu_Geometry->addAction( _actionSmooth->qaction());
    _ui->menu_Geometry->addAction( _actionCrop->qaction());
    _ui->menu_Geometry->addAction( _actionMakeLeftFace->qaction());
    _ui->menu_Geometry->addAction( _actionMakeRightFace->qaction());
    _actionNonRigidRegistration->setLocked(true);
    _ui->menu_Geometry->addAction( _actionNonRigidRegistration->qaction());
}   // end createGeometryMenu


// private
void ClinifaceMain::createMetricsMenu()
{
    _ui->menu_Metrics->addAction( _actionDetectFace->qaction());
    _ui->menu_Metrics->addAction( _actionResetDetection->qaction());
    _ui->menu_Metrics->addAction( _actionShowMetrics->qaction());
}   // end createMetricsMenu


// private
void ClinifaceMain::createHelpMenu()
{
    _ui->action_Help->setShortcut(QKeySequence::HelpContents);
    connect( _ui->action_Help, &QAction::triggered, [this](){ _helpAss->show();});
    _ui->menu_Help->addAction( _ui->action_Help);
    _ui->menu_Help->addAction( QWhatsThis::createAction());
    _ui->menu_Help->addSeparator();
    _ui->menu_Help->addAction( connectDialog( _ui->action_Preferences, _prefsDialog));
    _ui->menu_Help->addAction( connectDialog( _ui->action_Plugins, _ploader->dialog()));
    _ui->menu_Help->addAction( connectDialog( _ui->action_About, new AboutDialog(this)));
}   // end createHelpMenu


void ClinifaceMain::createToolBar()
{
    _ui->mainToolBar->addAction( _actionLoadFaceModels->qaction());
    _ui->mainToolBar->addAction( _actionSaveFaceModel->qaction());
    _ui->mainToolBar->addAction( _actionShowScanInfo->qaction());
    if ( FaceTools::Report::ReportManager::isAvailable())
        _ui->mainToolBar->addAction( _actionExportPDF->qaction());
    _ui->mainToolBar->addSeparator();
    _ui->mainToolBar->addAction( _actionUndo->qaction());
    _ui->mainToolBar->addAction( _actionRedo->qaction());
    _ui->mainToolBar->addSeparator();
    _ui->mainToolBar->addAction( _actionDetectFace->qaction());
    _ui->mainToolBar->addAction( _actionFillHoles->qaction());
    _ui->mainToolBar->addAction( _actionSmooth->qaction());
    _ui->mainToolBar->addAction( _actionCrop->qaction());

    _ui->mainToolBar->addSeparator();

    _ui->mainToolBar->addAction( _actionAlignLandmarks->qaction());
    _ui->mainToolBar->addAction( _actionTransformToStandardPosition->qaction());
    _ui->mainToolBar->addAction( _actionTransformToCentre->qaction());
    _ui->mainToolBar->addAction( _actionRotateX90->qaction());
    _ui->mainToolBar->addAction( _actionRotateY90->qaction());
    _ui->mainToolBar->addAction( _actionRotateZ90->qaction());
    _ui->mainToolBar->addAction( _actionReflect->qaction());
    _ui->mainToolBar->addAction( _actionScaleModel->qaction());
    _ui->mainToolBar->addAction( _actionToggleCameraActorInteraction->qaction());

    _ui->mainToolBar->addSeparator();

    _ui->mainToolBar->addAction( _actionResetCamera->qaction());
    _ui->mainToolBar->addAction( _actionOrientCameraToLeftFace->qaction());
    _ui->mainToolBar->addAction( _actionOrientCameraToFrontFace->qaction());
    _ui->mainToolBar->addAction( _actionOrientCameraToRightFace->qaction());
    _ui->mainToolBar->addAction( _actionSetParallelProjection->qaction());
    _ui->mainToolBar->addAction( _actionSynchroniseCameraMovement->qaction());

    // Append a space and then the logo to the toolbar.
    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ui->mainToolBar->addWidget(emptySpacer);
    QLabel* label = new QLabel;
    label->setText( QString("<a href=\"") + APP_WEBSITE + QString("\"><img src=\":/logos/TOOLBAR_LOGO\"/></a>"));
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);
    _ui->mainToolBar->addWidget(label);
}   // end createToolBar


void ClinifaceMain::createContextMenu()
{
    _cmenu = new FaceTools::Interactor::ContextMenu;
    _cmenu->addAction( _actionSetFocus);
    _cmenu->addSeparator();
    _cmenu->addAction( _actionAddPath);
    _cmenu->addAction( _actionRenamePath);
    _cmenu->addAction( _actionDeletePath);
    _cmenu->addSeparator();
    _cmenu->addAction( _actionRadialSelect);
    _cmenu->addAction( _actionDiscardManifold);
    _cmenu->addAction( _actionRemoveManifolds);
}   // end createContextMenu


void ClinifaceMain::createActions()
{
    using namespace FaceTools::Report;
    using namespace FaceTools::Vis;

    _actionLoadFaceModels = new ActionLoadFaceModels( "Open", QIcon(":/icons/LOAD"));
    _actionLoadDirFaceModels = new ActionLoadDirFaceModels( "Open Dir", QIcon(":/icons/LOAD_DIR"));
    _actionSaveFaceModel = new ActionSaveFaceModel( "Save", QIcon(":/icons/SAVE"));
    _actionSaveAsFaceModel = new ActionSaveAsFaceModel( "Save As", QIcon(":/icons/SAVE_AS"));
    _actionExportPDF = new ActionExportPDF( "Create Template Report", QIcon(":/icons/PDF"), Qt::CTRL + Qt::Key_R);
    _actionCloseFaceModel = new ActionCloseFaceModel( "Close", QIcon(":/icons/CLOSE"));
    _actionCloseAllFaceModels = new ActionCloseAllFaceModels( "Close All", QIcon(":/icons/CLOSE_ALL"));
    _actionExportMetaData = new ActionExportMetaData( "Export Metadata", QIcon(":/icons/CODE_FILE"));
    _actionImportMetaData = new ActionImportMetaData( "Import Metadata", QIcon(":/icons/CODE_FILE"));

    _actionRedo = new ActionRedo( "Redo", QIcon(":/icons/REDO"));
    _actionUndo = new ActionUndo( "Undo", QIcon(":/icons/UNDO"));

    _actionVisTexture = new ActionVisualise( "Texture", QIcon(":/icons/TEXTURE"), new TextureVisualisation, Qt::Key_T);
    _actionVisTexture->addTriggerEvent( Event::LOADED_MODEL);
    _actionVisWireframe = new ActionVisualise( "Wireframe", QIcon(":/icons/WIREFRAME"), new WireframeVisualisation, Qt::Key_W);
    _actionVisOutlines = new ActionVisualise( "Outlines", QIcon(":/icons/OUTLINES"), new OutlinesVisualisation, Qt::Key_O);
    _actionVisPolyLabels = new ActionVisualise( "Polygon Labels", QIcon(":/icons/NUMBERS"), new PolyLabelsVisualisation, Qt::SHIFT + Qt::Key_V);
    _actionVisVertexLabels = new ActionVisualise( "Vertex Labels", QIcon(":/icons/NUMBERS"), new VertexLabelsVisualisation, Qt::Key_V);
    _actionEditLandmarks = new ActionEditLandmarks( "Show Landmarks", QIcon(":/icons/MARKER"), Qt::Key_L);

    _actionOrientCameraToFrontFace = new ActionOrientCameraToFace("Orient Camera to Front Face", QIcon(":/icons/ORIENT_CAMERA"), 470., 0.0, Qt::Key_Up);
    _actionOrientCameraToLeftFace = new ActionOrientCameraToFace("Orient Camera to Left Face", QIcon(":/icons/LOOK_RIGHT"), 470., -CV_PI/2, Qt::Key_Left);
    _actionOrientCameraToRightFace = new ActionOrientCameraToFace("Orient Camera to Right Face", QIcon(":/icons/LOOK_LEFT"), 470., CV_PI/2, Qt::Key_Right);
    _actionTransformToCentre = new ActionTransformToCentre("Recentre Model", QIcon(":/icons/CENTRE"), Qt::Key_C);
    _actionTransformToStandardPosition = new ActionTransformToStandardPosition("Transform to Standard Position", QIcon(":/icons/TRANSFORM"), Qt::SHIFT + Qt::Key_C);
    //_actionOrientCameraToFrontFace->addTriggerEvent( Event::VIEWER_CHANGE);
    _actionOrientCameraToFrontFace->addTriggerEvent( Event::FACE_DETECTED);
    _actionOrientCameraToFrontFace->addTriggerEvent( Event::LOADED_MODEL);

    _actionDiscardManifold = new ActionDiscardManifold( "Discard Manifold Under Cursor", QIcon(":/icons/FACE"));
    _actionRemoveManifolds = new ActionRemoveManifolds( "Discard Manifolds NOT Under Cursor", QIcon(":/icons/FACE"));
    _actionDetectFace = new ActionDetectFace( "Detect Facial Landmarks", QIcon(":/icons/DETECT_FACE"));
    _actionResetDetection = new ActionResetDetection( "Remove Facial Landmarks", QIcon(":/icons/RESET_DETECT_FACE"));

    _actionResetCamera = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), Qt::Key_Space);
    _actionResetCamera->addTriggerEvent( Event::VIEWER_CHANGE);
    _actionResetCamera->addTriggerEvent( Event::LOADED_MODEL);
    _actionResetCamera->addTriggerEvent( Event::CLOSED_MODEL);

    _actionSetParallelProjection = new ActionSetParallelProjection( "Show Orthographic Projection", QIcon(":/icons/ORTHOGRAPHIC"), Qt::Key_E);

    _actionSaveScreenshot = new ActionSaveScreenshot( "Take Screenshot (All Viewers)", QIcon(":/icons/SCREENSHOT"));
    _actionSmooth = new ActionSmooth( "Smooth Sharp Edges/Corners", QIcon(":/icons/SHAVE1"));
    _actionReflect = new ActionReflect( "Reflect", QIcon(":/icons/REFLECT"));
    _actionNonRigidRegistration = new ActionNonRigidRegistration( "Non-Rigid Surface Registration", QIcon(":/icons/REMESH"));

    _actionToggleAxes = new ActionToggleAxes( "Show World Axes", QIcon(":/icons/AXES"), Qt::Key_A);

    _actionToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Move Model", QIcon(":/icons/MOVE_MODEL"), Qt::Key_Z);
    _actionSynchroniseCameraMovement = new ActionSynchroniseCameraMovement( "Synchronise Cameras", QIcon(":/icons/SYNCH_CAMERAS"));
    _actionMarquee = new ActionMarquee("Auto-Rotate Views", QIcon(":/icons/MOVIE"));

    _actionSetFocus = new ActionSetFocus( "Set Camera Focus", QIcon(":/icons/FOCUS"), Qt::Key_F);
    this->addAction(_actionSetFocus->qaction());
    _actionRadialSelect = new ActionRadialSelect( "Select Radial Area", QIcon(":/icons/LASSO"));
    _actionCrop = new ActionCrop( "Crop Region", QIcon(":/icons/SCALPEL"), _actionRadialSelect);

    _actionMakeLeftFace = new ActionMakeHalfFace( "Create Left Symmetric Face", QIcon(":/icons/LEFT_FACE"));
    _actionMakeLeftFace->setPlane( cv::Vec3f(-1,0,0));

    _actionMakeRightFace = new ActionMakeHalfFace( "Create Right Symmetric Face", QIcon(":/icons/RIGHT_FACE"));
    _actionMakeRightFace->setPlane( cv::Vec3f(1,0,0));

    _actionAlignLandmarks = new ActionAlignLandmarks( "Rigid Landmark Alignment", QIcon(":/icons/ALIGN"));
    _actionAlignVertices = new ActionAlignVertices( "Rigid Vertex Correspondence", QIcon(":/icons/ALIGN"));
    _actionAlignICP = new ActionAlignICP( "Rigid Vertex ICP Alignment", QIcon(":/icons/ALIGN"));
    _actionFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"));
    _actionSetOpacity = new ActionSetOpacity( "Model Opacity");
    _actionBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"), Qt::Key_B);

    _actionEditPaths = new ActionEditPaths( "Show Custom Distances", QIcon(":/icons/CALIPERS"), &_pathsVis, _pathsInteractor, Qt::Key_P);
    _actionAddPath = new ActionAddPath( "Add Distance Measure", QIcon(":/icons/CALIPERS"), &_pathsVis, _pathsInteractor);
    _actionDeletePath = new ActionDeletePath( "Delete Distance Measure", QIcon(":/icons/ERASER"), &_pathsVis, _pathsInteractor);
    _actionRenamePath = new ActionRenamePath( "Rename Distance Measure", QIcon(":/icons/EDIT"), _pathsInteractor);

    //_actionSetSurfaceColour = new ActionSetSurfaceColour( "Set Base Surface Colour", this);
    _actionSetMinScalarColour = new ActionSetMinScalarColour( "Minimum Colour for Scalar Visualisations");
    _actionSetMaxScalarColour = new ActionSetMaxScalarColour( "Maximum Colour for Scalar Visualisations");
    _actionSetNumScalarColours = new ActionSetNumScalarColours( "Number of Colours for Scalar Visualisations");
    _actionChangeSurfaceMappingRange = new ActionChangeSurfaceMappingRange( "Min/Max Range for Scalar Visualisations");

    _actionToggleScalarLegend = new ActionToggleScalarLegend( "Show Scalar Legend");
    _actionToggleStereoRendering = new ActionToggleStereoRendering( "Show 3D (Red/Blue)", QIcon(":/icons/3D_GLASSES"));

    _actionRotateX90 = new ActionRotateModel( "Rotate 90 degrees about X", QIcon(":/icons/ROTATE_X90"), cv::Vec3d(1,0,0), 90);
    _actionRotateY90 = new ActionRotateModel( "Rotate 90 degrees about Y", QIcon(":/icons/ROTATE_Y90"), cv::Vec3d(0,1,0), 90);
    _actionRotateZ90 = new ActionRotateModel( "Rotate 90 degrees about Z", QIcon(":/icons/ROTATE_Z90"), cv::Vec3d(0,0,1), 90);
    _actionScaleModel = new ActionScaleModel( "Rescale", QIcon(":/icons/RESIZE_MODEL"));

    _actionInvertNormals = new ActionInvertNormals( "Invert Polygon Normals", QIcon(":/icons/NORMAL_FLIP"));
}   // end createActions



void ClinifaceMain::setupMainViewer()
{
    using namespace FaceTools;

    FMV* lv = _mfmv->leftViewer();
    FMV* cv = _mfmv->centreViewer();
    FMV* rv = _mfmv->rightViewer();

    // Set move actions
    FaceAction* actionMoveLC = new ActionMoveView( cv, lv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveCL = new ActionMoveView( lv, cv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    FaceAction* actionMoveCR = new ActionMoveView( rv, cv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveRC = new ActionMoveView( cv, rv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    _mfmv->setMoveLeftToCentreAction(  actionMoveLC->qaction());
    _mfmv->setMoveCentreToLeftAction(  actionMoveCL->qaction());
    _mfmv->setMoveCentreToRightAction( actionMoveCR->qaction());
    _mfmv->setMoveRightToCentreAction( actionMoveRC->qaction());

    // Set copy actions
    FaceAction* actionCopyLC = new ActionCopyView( cv, lv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyCL = new ActionCopyView( lv, cv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
    FaceAction* actionCopyCR = new ActionCopyView( rv, cv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyRC = new ActionCopyView( cv, rv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
    _mfmv->setCopyLeftToCentreAction(  actionCopyLC->qaction());
    _mfmv->setCopyCentreToLeftAction(  actionCopyCL->qaction());
    _mfmv->setCopyCentreToRightAction( actionCopyCR->qaction());
    _mfmv->setCopyRightToCentreAction( actionCopyRC->qaction());

    // Register the actions
    FAM::registerAction( actionMoveLC);
    FAM::registerAction( actionMoveCL);
    FAM::registerAction( actionMoveCR);
    FAM::registerAction( actionMoveRC);

    FAM::registerAction( actionCopyLC);
    FAM::registerAction( actionCopyCL);
    FAM::registerAction( actionCopyCR);
    FAM::registerAction( actionCopyRC);
}   // end setupMainViewer


void ClinifaceMain::createMetrics()
{
    _actionShowScanInfo = new ActionShowScanInfo( "Assessment Information", QIcon(":/icons/IMAGE_DETAILS"), Qt::Key_I);
    _actionShowModelProperties = new ActionShowModelProperties( "Model Properties", QIcon(":/icons/MODEL_PROPERTIES"), QString("`"));
    _actionShowMetrics = new ActionShowMetrics( "Show Metrics", QIcon(":/icons/METRICS1"), Qt::Key_M);
    ActionUpdateThumbnail* actionUpdateThumbnail = new ActionUpdateThumbnail;

    FAM::registerAction( actionUpdateThumbnail);
    FAM::registerAction( _actionShowScanInfo);
    FAM::registerAction( _actionShowModelProperties);
    FAM::registerAction( _actionShowMetrics);
    FAM::registerAction( new ActionUpdateMeasurements);

    _actionShowScanInfo->setThumbnailUpdater( actionUpdateThumbnail);
    _actionShowMetrics->setShowScanInfoAction( _actionShowScanInfo->qaction());
}   // end createMetrics



// public
ClinifaceMain::ClinifaceMain()
    : QMainWindow(nullptr), _ui(new Ui::ClinifaceMain)
{
    _ui->setupUi(this);
    setWindowTitle( appName());
    setAcceptDrops(true);   // Accept dropping of files onto this widget
    setContextMenuPolicy(Qt::NoContextMenu);

    _mfmv = new FaceTools::MultiFaceModelViewer( this);
    ModelSelector::addViewer( _mfmv->leftViewer());
    ModelSelector::addViewer( _mfmv->centreViewer(), true/*default*/); 
    ModelSelector::addViewer( _mfmv->rightViewer());
    ModelSelector::setStatusBar( _ui->statusBar);

    FAM::get( this);  // Creates

    setupMainViewer();

    _pathsInteractor = FaceTools::Interactor::PathsInteractor::create( _pathsVis);

    createActions();
    createMetrics();

    registerActions();

    QWidget* cwidget = new QWidget;
    cwidget->setLayout( new QVBoxLayout);
    cwidget->layout()->setContentsMargins(0,0,0,0);
    cwidget->layout()->addWidget( _mfmv);
    setCentralWidget( cwidget);
    _mfmv->layout()->setContentsMargins(1,1,1,1);

    _prefsDialog = new PreferencesDialog(this);
    connect( _prefsDialog, &PreferencesDialog::onUpdated, [](){ FAM::get()->doEvent( Event::NONE);});
    connect( _prefsDialog, &PreferencesDialog::showHelp, [this](){ _helpAss->show("preferences.html");});
    Preferences::updateApplication();   // Initialise loaded preferences

    _ploader = new ClinifacePluginsLoader( this);
    _ploader->dialog()->setWindowTitle( appName() + QString(" | Plugins"));

    createToolBar();
    createFileMenu();
    createViewMenu();
    createCameraMenu();
    createMetricsMenu();
    createTransformMenu();
    createGeometryMenu();
    createContextMenu();

    static const QString htmldir = QDir( QApplication::applicationDirPath()).filePath( HTML_DIR);
    _helpAss = new QTools::HelpAssistant( htmldir, this);
    _helpAss->addContent( HTML_PLUGINS_DIR);

    createHelpMenu();

    _mtrigger = new FaceTools::Interactor::MovementTrigger;
    connect( _mtrigger, &FaceTools::Interactor::MovementTrigger::onEvent, &*FAM::get(), &FAM::doEvent);

    // Set the plugin UI points for loaded plugins
    _ppoints.set("View", _ui->menu_View);
    _ppoints.set("Camera", _ui->menu_Camera);
    _ppoints.set("Metrics", _ui->menu_Metrics);
    _ppoints.set("Geometry", _ui->menu_Geometry);
    _ppoints.set("Transform", _ui->menu_Transform);
    _ppoints.set("Help", _ui->menu_Help);
    _ppoints.set("Main", _ui->mainToolBar);
    _ppoints.set("Visualisations", _ui->visToolBar);
    _ppoints.set("Scalar Mapping", _ui->scmapToolBar);

    // Load the plugins
    connect( _ploader, &ClinifacePluginsLoader::onAttachToMenu, this, &ClinifaceMain::_doOnAttachPluginActionToUI);
    _ploader->loadPlugins( QApplication::applicationDirPath() + "/plugins");

    // Refresh the contents here because plugins must dump their help docs into the prescribed place first.
    if ( !_helpAss->refreshContents(":/data/HELP_TOC"))
        std::cerr << "Help TOC refresh failure!" << std::endl;

    // Update viewer selection model lists and window title whenever an action finishes.
    connect( &*FAM::get(), &FaceActionManager::onUpdate, _mfmv, &FaceTools::MultiFaceModelViewer::doOnUpdateModelLists);
    connect( &*FAM::get(), &FaceActionManager::onUpdate, this, &ClinifaceMain::_doOnUpdate);
    connect( &*FAM::get(), &FaceActionManager::onShowHelp, [this]( const QString& tok){ _helpAss->show(tok);});

    // Locate centrally on desktop
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), qApp->desktop()->availableGeometry()));
    _doOnUpdate(nullptr);
}   // end ctor


// public
ClinifaceMain::~ClinifaceMain()
{
    delete _helpAss;
    delete _mtrigger;
    delete _cmenu;
    delete _ploader;
    delete _ui;
}   // end dtor


// protected virtual
QSize ClinifaceMain::sizeHint() const { return QSize( 1024, 680);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname)
{
    return _actionLoadFaceModels->loadModel( fname);
}   // end loadModel


// protected virtual
void ClinifaceMain::dragEnterEvent( QDragEnterEvent *evt)
{
    if ( evt->mimeData()->hasText())
    {
        QString fname = evt->mimeData()->text();
        if ( fname.startsWith("file:///"))
        {
            fname = fname.remove(0, QString("file:///").size()).trimmed();
            if ( FaceTools::FileIO::FMM::canRead( fname.toStdString()))
                evt->acceptProposedAction();
        }   // end if
    }   // end if
}   // end dragEnterEvent


// protected virtual
void ClinifaceMain::dropEvent( QDropEvent *evt)
{
    QString fname = evt->mimeData()->text();
    fname = fname.remove(0, QString("file:///").size()).trimmed();
    if ( loadModel(fname))
        evt->acceptProposedAction();
}   // end dropEvent


// protected virtual
void ClinifaceMain::closeEvent( QCloseEvent* evt)
{
    if ( _actionCloseAllFaceModels->isEnabled() && !_actionCloseAllFaceModels->execute(Event::USER))
        evt->ignore();
}   // end closeEvent


void ClinifaceMain::_doOnUpdate( const FaceTools::FM* fm)
{
    QString wtitle = QString( "%1 %2").arg( appName(), APP_VERSION_STRING);

    if ( fm)
    {
        QString mfile = FaceTools::FileIO::FMM::filepath(fm).c_str();
        if ( !fm->isSaved())
            mfile += " (*)";
        wtitle = QString("%1 | %2").arg( appName(), mfile);
    }   // end if

    setWindowTitle( wtitle);
}   // end _doOnUpdate


void ClinifaceMain::_doOnAttachPluginActionToUI( FaceAction* act)
{
    // Append the plugin action to where it wants to be on the menu and toolbar.
    _ppoints.appendToMenu( act->qaction(), act->attachToMenu());
    _ppoints.appendToToolBar( act->qaction(), act->attachToToolBar());

    // Embed the helpfile content inside the Cliniface HTML template.
    const QString content = producePluginHelpContent( act);
    if ( !content.isEmpty())
    {
        QString tok = _helpAss->addContent( HTML_PLUGINS_DIR, content);
        if ( !tok.isEmpty())
            act->helpFile(tok); // Set the token back in the action to use.
    }   // end if
}   // end _doOnAttachPluginActionToUI

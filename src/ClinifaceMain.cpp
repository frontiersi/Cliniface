/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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
#include <Cliniface_Config.h>
#include <AboutDialog.h>

#include <FaceTools/Action/ActionRedo.h>
#include <FaceTools/Action/ActionUndo.h>
#include <FaceTools/Action/ActionSmooth.h>
#include <FaceTools/Action/ActionReflect.h>
#include <FaceTools/Action/ActionCopyView.h>
#include <FaceTools/Action/ActionMoveView.h>
#include <FaceTools/Action/ActionSetFocus.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Action/ActionFillHoles.h>
#include <FaceTools/Action/ActionUpdateU3D.h>
#include <FaceTools/Action/ActionAlignModel.h>
#include <FaceTools/Action/ActionDetectFace.h>
#include <FaceTools/Action/ActionFixNormals.h>
#include <FaceTools/Action/ActionScaleModel.h>
#include <FaceTools/Action/ActionToggleMask.h>
#include <FaceTools/Action/ActionCentreModel.h>
#include <FaceTools/Action/ActionRotateModel.h>
#include <FaceTools/Action/ActionMapSymmetry.h>
#include <FaceTools/Action/ActionMapCurvature.h>
#include <FaceTools/Action/ActionInvertNormals.h>
#include <FaceTools/Action/ActionAlignLandmarks.h>
#include <FaceTools/Action/ActionCloseFaceModel.h>
#include <FaceTools/Action/ActionExportMetaData.h>
#include <FaceTools/Action/ActionImportMetaData.h>
#include <FaceTools/Action/ActionResetDetection.h>
#include <FaceTools/Action/ActionBackfaceCulling.h>
#include <FaceTools/Action/ActionUpdateThumbnail.h>
#include <FaceTools/Action/ActionRestoreLandmarks.h>
#include <FaceTools/Action/ActionSetSurfaceColour.h>
#include <FaceTools/Action/ActionCloseAllFaceModels.h>
#include <FaceTools/Action/ActionOrientCameraToFace.h>
#include <FaceTools/Action/ActionSetMinScalarColour.h>
#include <FaceTools/Action/ActionSetMaxScalarColour.h>
#include <FaceTools/Action/ActionUpdateGrowthData.h>
#include <FaceTools/Action/ActionUpdateMeasurements.h>
#include <FaceTools/Action/ActionSetNumScalarColours.h>
#include <FaceTools/Action/ActionChangeSurfaceMappingRange.h>

#include <FaceTools/Action/FaceActionManager.h>
#include <FaceTools/Action/ModelSelector.h>

#include <FaceTools/Report/ReportManager.h>

#include <FaceTools/Vis/OutlinesVisualisation.h>
#include <FaceTools/Vis/LabelsVisualisation.h>
#include <FaceTools/Vis/PlaneVisualisation.h>
#include <FaceTools/Vis/LandmarkLabelsView.h>
#include <FaceTools/Vis/VertexLabelsView.h>
#include <FaceTools/Vis/PolyLabelsView.h>

#include <FaceTools/MiscFunctions.h>    // loadTextFromFile

#include <QLabel>
#include <QScreen>
#include <QMimeData>
#include <QApplication>
#include <QDesktopWidget>
#include <QWhatsThis>
#include <QWidgetAction>
#include <QVBoxLayout>
using Cliniface::ClinifaceMain;
using FaceTools::Vec3f;
using FMM = FaceTools::FileIO::FaceModelManager;
using MS = FaceTools::Action::ModelSelector;


namespace {

void showDialog( QDialog *d)
{
    d->show();
    d->raise();
    d->activateWindow();
}   // end showDialog


QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, [d](){ showDialog(d);});
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
    os << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">" << Qt::endl
       << "<html>" << Qt::endl
       << "<head>" << Qt::endl
       << "<title>" << dname << "</title>" << Qt::endl
       << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../styles/main.css\">" << Qt::endl
       << "<script src=\"../scripts/topButton.js\"></script>" << Qt::endl
       << "<button onclick=\"topFunction()\" id=\"topButton\" title=\"Back to top\">Back to top</button>" << Qt::endl
       << "</head>" << Qt::endl
       << "<body id=\"_top\">" << Qt::endl
       << "<center><a href=\"" << APP_WEBSITE << "\"><img src=\"../images/logo_357x110.png\" alt=\"Cliniface Logo\"></a></center>" << Qt::endl
       << "<center><h1>" << dname << "</h1></center>" << Qt::endl
       << "<hr>" << Qt::endl
       << FaceTools::loadTextFromFile(helpFile) << Qt::endl // Write out the help file contents
       // Write out the footer
       << "<br>" << Qt::endl
       << "<hr>" << Qt::endl
       << "<center>" << Qt::endl
       << "<a href=\"#_top\">Back to top</a>" << Qt::endl
       << "<strong><p>&copy; " << APP_CR_YEARS << " " << APP_ORGANISATION << " &amp; " << APP_AUTHOR_NAME << "</p><strong>" << Qt::endl
       << "</center>" << Qt::endl
       << "</body>" << Qt::endl
       << "</html>" << Qt::endl;

    return content;
}   // end producePluginHelpContent

}   // end namespace


void ClinifaceMain::_registerActions()
{
    FAM::registerAction( _actLoad);
    FAM::registerAction( _actSaveFaceModel);
    FAM::registerAction( _actSaveAsFaceModel);
    FAM::registerAction( _actExportMask);
    FAM::registerAction( _actExportMetaData);
    FAM::registerAction( _actImportMetaData);
    FAM::registerAction( _actExportPDF);
    FAM::registerAction( _actCloseFaceModel);
    FAM::registerAction( _actCloseAllFaceModels);

    FAM::registerAction( _actRedo);
    FAM::registerAction( _actUndo);
    FAM::registerAction( _actEditLandmarks);
    FAM::registerAction( _actShowLandmarks);
    FAM::registerAction( _actVisLandmarkLabels);
    FAM::registerAction( _actEditPaths);
    FAM::registerAction( _actToggleMask);
    FAM::registerAction( _actVisOutlines);
    FAM::registerAction( _actVisMedianPlane);
    FAM::registerAction( _actVisTransversePlane);
    //FAM::registerAction( _actVisFrontalPlane);
    FAM::registerAction( _actVisWireframe);
    FAM::registerAction( _actVisTexture);
    FAM::registerAction( _actVisPolyLabels);
    FAM::registerAction( _actVisVertexLabels);

    FAM::registerAction( _actOrientCameraToF);
    FAM::registerAction( _actOrientCameraToL);
    FAM::registerAction( _actOrientCameraToLQ);
    FAM::registerAction( _actOrientCameraToR);
    FAM::registerAction( _actOrientCameraToRQ);
    FAM::registerAction( _actOrientCameraToT);
    FAM::registerAction( _actOrientCameraToTQ);
    FAM::registerAction( _actOrientCameraToB);
    FAM::registerAction( _actOrientCameraToBQ);
    FAM::registerAction( _actCentreModel);
    FAM::registerAction( _actDetectFace);
    FAM::registerAction( _actResetDetection);
    FAM::registerAction( _actAlignLandmarks);
    FAM::registerAction( _actRestoreLandmarks);
    FAM::registerAction( _actSetParallelProjection);
    FAM::registerAction( _actSaveScreenshot);
    FAM::registerAction( _actSmooth);
    FAM::registerAction( _actReflect);
    FAM::registerAction( _actToggleAxes);
    FAM::registerAction( _actToggleCameraActorInteraction);
    FAM::registerAction( _actSynchroniseCameras);
    FAM::registerAction( _actMarquee);
    FAM::registerAction( _actSetFocus);
    FAM::registerAction( _actRadialSelect);
    FAM::registerAction( _actDiscardManifold);
    FAM::registerAction( _actRemoveManifolds);
    FAM::registerAction( _actMakeLeftFace);
    FAM::registerAction( _actMakeRightFace);
    FAM::registerAction( _actAlignModel);
    FAM::registerAction( _actFillHoles);
    FAM::registerAction( _actSetOpacity);
    FAM::registerAction( _actBackfaceCulling);

    FAM::registerAction( _actRenamePath);
    FAM::registerAction( _actAddPath);
    FAM::registerAction( _actDeletePath);
    FAM::registerAction( _actDeleteAllPaths);
    FAM::registerAction( _actRestoreSingleLandmark);

    //FAM::registerAction( _actSetMinScalarColour);
    //FAM::registerAction( _actSetMaxScalarColour);
    FAM::registerAction( _actSetNumScalarColours);
    FAM::registerAction( _actChangeSurfaceMappingRange);
    FAM::registerAction( _actToggleScalarLegend);
    FAM::registerAction( _actRotateX90);
    FAM::registerAction( _actRotateY90);
    FAM::registerAction( _actRotateZ90);
    FAM::registerAction( _actScaleModel);

#ifndef NDEBUG
    FAM::registerAction( _actFixNormals);
#endif
    FAM::registerAction( _actInvertNormals);
    FAM::registerAction( _actExtractFace);
    FAM::registerAction( _actToggleStereoRendering);

    FAM::registerAction( new ActionUpdateGrowthData);
    FAM::registerAction( new ActionUpdateMeasurements);
    FAM::registerAction( new ActionMapCurvature);
    FAM::registerAction( new ActionMapSymmetry);
    ActionUpdateU3D *actUpdateU3D = new ActionUpdateU3D;
    //actUpdateU3D->setLocked( true);  // Uncomment for debug
    FAM::registerAction( actUpdateU3D);

    connect( _ui->action_VisualisationsToolbar, &QAction::triggered, [=](bool v){ _ui->visToolBar->setVisible(v); _ui->scmapToolBar->setVisible(v);});
    connect( _ui->action_Exit, &QAction::triggered, this, &ClinifaceMain::close);
}   // end _registerActions


void ClinifaceMain::_createFileMenu()
{
    _ui->menu_File->addAction( _actLoad->qaction());
    _ui->menu_File->addAction( _actSaveFaceModel->qaction());
    _ui->menu_File->addAction( _actSaveAsFaceModel->qaction());
    _ui->menu_File->addAction( _actExportMask->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actImportMetaData->qaction());
    _ui->menu_File->addAction( _actExportMetaData->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actUndo->qaction());
    _ui->menu_File->addAction( _actRedo->qaction());
    _ui->menu_File->addSeparator();
    _ui->menu_File->addAction( _actShowScanInfo->qaction());
    _ui->menu_File->addAction( _actShowMeshInfo->qaction());

    if ( FaceTools::Report::ReportManager::isAvailable())
        _ui->menu_File->addAction( _actExportPDF->qaction());

    _ppoints.set("File", _ui->menu_File, _ui->menu_File->addSeparator());

    _ui->menu_File->addAction( _actCloseFaceModel->qaction());
    _ui->menu_File->addAction( _actCloseAllFaceModels->qaction());
    _ui->menu_File->addSeparator();
    _ui->action_Exit->setShortcut( QKeySequence::Quit);
    _ui->menu_File->addAction( _ui->action_Exit);
}   // end _createFileMenu


void ClinifaceMain::_createViewMenu()
{
    _ui->menu_View->addAction( _actVisTexture->qaction());
    _ui->menu_View->addAction( _actVisWireframe->qaction());
    _ui->menu_View->addAction( _actVisOutlines->qaction());
    _ui->menu_View->addAction( _actToggleMask->qaction());
    _ui->menu_View->addSeparator();
    _ui->menu_View->addAction( _actVisMedianPlane->qaction());
    _ui->menu_View->addAction( _actVisTransversePlane->qaction());
    //_ui->menu_View->addAction( _actVisFrontalPlane->qaction());
    _ui->menu_View->addSeparator();

    _ui->menu_View->addAction( _actRadialSelect->qaction());
    _ui->menu_View->addAction( _actVisPolyLabels->qaction());
    _ui->menu_View->addAction( _actVisVertexLabels->qaction());

    QMenu *surfaceMenu = _ui->menu_View->addMenu( "&Surface Mapping...");
    _ppoints.set("Surface Mapping", surfaceMenu);
    surfaceMenu->setIcon( QIcon(":/icons/COLOURS"));
    surfaceMenu->addAction( _actToggleScalarLegend->qaction());
    //surfaceMenu->addAction( _actSetMinScalarColour->qaction());
    //surfaceMenu->addAction( _actSetMaxScalarColour->qaction());
    surfaceMenu->addSeparator();

    _ui->menu_View->addSeparator();

    _ui->menu_View->addAction( _actBackfaceCulling->qaction());
    _ui->menu_View->addAction( _actToggleAxes->qaction());
    _ui->menu_View->addAction( _actSaveScreenshot->qaction());
    _ui->menu_View->addAction( _actToggleStereoRendering->qaction());
    _ui->menu_View->addAction( _ui->action_VisualisationsToolbar);

    /****** _ui->visToolBar ******/
    _ui->visToolBar->addAction( _actVisTexture->qaction());
    _ui->visToolBar->addAction( _actVisWireframe->qaction());
    _ui->visToolBar->addAction( _actVisOutlines->qaction());
    //_ui->visToolBar->addAction( _actToggleMask->qaction());
    _ui->visToolBar->addSeparator();
    _ui->visToolBar->addAction( _actShowLandmarks->qaction());
    _ui->visToolBar->addAction( _actShowMetrics->qaction());
    _ui->visToolBar->addAction( _actEditPaths->qaction());
    _ui->visToolBar->addAction( _actRadialSelect->qaction());
    _ui->visToolBar->addSeparator();
    //_ui->visToolBar->addAction( _actBackfaceCulling->qaction());
    _ui->visToolBar->addWidget( _actSetOpacity->widget());
    _ui->visToolBar->addAction( _actToggleAxes->qaction());

    /****** _ui->scmapToolBar ******/
    _ui->scmapToolBar->addWidget( _actChangeSurfaceMappingRange->widget());
    _ui->scmapToolBar->addWidget( _actSetNumScalarColours->widget());
    _ui->scmapToolBar->addSeparator();

    /*
    QWidget* empty = new QWidget;
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ui->scmapToolBar->addWidget( empty);
    */
}   // end _createViewMenu


void ClinifaceMain::_createCameraMenu()
{
    _ui->menu_Camera->addAction( _actOrientCameraToF->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToL->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToLQ->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToRQ->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToR->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToT->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToTQ->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToB->qaction());
    _ui->menu_Camera->addAction( _actOrientCameraToBQ->qaction());
    _ui->menu_Camera->addSeparator();
    _ui->menu_Camera->addAction( _actSetParallelProjection->qaction());
    _ui->menu_Camera->addAction( _actSynchroniseCameras->qaction());
    _ui->menu_Camera->addAction( _actMarquee->qaction());
}   // end _createCameraMenu


void ClinifaceMain::_createTransformMenu()
{
    _ui->menu_Transform->addAction( _actAlignModel->qaction());
    _ui->menu_Transform->addAction( _actCentreModel->qaction());
    _ui->menu_Transform->addAction( _actScaleModel->qaction());
    _ui->menu_Transform->addAction( _actReflect->qaction());
    _ui->menu_Transform->addSeparator();
    _ui->menu_Transform->addAction( _actRotateX90->qaction());
    _ui->menu_Transform->addAction( _actRotateY90->qaction());
    _ui->menu_Transform->addAction( _actRotateZ90->qaction());
    _ui->menu_Transform->addSeparator();
    _ui->menu_Transform->addAction( _actToggleCameraActorInteraction->qaction());
}   // end _createTransformMenu


void ClinifaceMain::_createGeometryMenu()
{
#ifndef NDEBUG
    _ui->menu_Geometry->addAction( _actFixNormals->qaction());
#endif
    _ui->menu_Geometry->addAction( _actInvertNormals->qaction());
    _ui->menu_Geometry->addAction( _actFillHoles->qaction());
    _ui->menu_Geometry->addAction( _actSmooth->qaction());
    _ui->menu_Geometry->addAction( _actExtractFace->qaction());
    _ui->menu_Geometry->addAction( _actMakeLeftFace->qaction());
    _ui->menu_Geometry->addAction( _actMakeRightFace->qaction());
}   // end _createGeometryMenu


void ClinifaceMain::_createMetricsMenu()
{
    _ui->menu_Metrics->addAction( _actDetectFace->qaction());
    _ui->menu_Metrics->addAction( _actResetDetection->qaction());
    _ui->menu_Metrics->addAction( _actEditLandmarks->qaction());
    _ui->menu_Metrics->addAction( _actShowLandmarks->qaction());
    _ui->menu_Metrics->addSeparator();
    _ui->menu_Metrics->addAction( _actShowMetrics->qaction());
    _ui->menu_Metrics->addAction( _actShowPhenotypes->qaction());
    _ui->menu_Metrics->addAction( _actEditPaths->qaction());
    _ui->menu_Metrics->addAction( _actDeleteAllPaths->qaction());
}   // end _createMetricsMenu


void ClinifaceMain::_createHelpMenu()
{
    _ui->action_Help->setShortcut( QKeySequence::HelpContents);
    connect( _ui->action_Help, &QAction::triggered, [this](){ _helpAss->show();});
    _ui->menu_Help->addAction( _ui->action_Help);
    _ui->menu_Help->addAction( QWhatsThis::createAction());
    _ui->menu_Help->addSeparator();
    _ui->action_Preferences->setShortcut( QKeySequence::Preferences);
    _ui->menu_Help->addAction( connectDialog( _ui->action_Preferences, _prefsDialog));
    _ui->menu_Help->addAction( connectDialog( _ui->action_Plugins, _ploader->dialog()));
    _ui->menu_Help->addAction( connectDialog( _ui->action_About, new AboutDialog(this)));
    _ui->menu_Help->addAction( _updatesDialog->checkForUpdateAction());
}   // end _createHelpMenu


void ClinifaceMain::_createToolBar()
{
    _ui->mainToolBar->addAction( _actLoad->qaction());
    _ui->mainToolBar->addAction( _actSaveFaceModel->qaction());
    _ui->mainToolBar->addAction( _actShowScanInfo->qaction());
    if ( FaceTools::Report::ReportManager::isAvailable())
        _ui->mainToolBar->addAction( _actExportPDF->qaction());
    _ui->mainToolBar->addSeparator();
    _ui->mainToolBar->addAction( _actUndo->qaction());
    _ui->mainToolBar->addAction( _actRedo->qaction());
    _ui->mainToolBar->addSeparator();
    _ui->mainToolBar->addAction( _actFillHoles->qaction());
    _ui->mainToolBar->addAction( _actSmooth->qaction());
    _ui->mainToolBar->addAction( _actExtractFace->qaction());
    _ui->mainToolBar->addAction( _actDetectFace->qaction());

    _ui->mainToolBar->addSeparator();

    _ui->mainToolBar->addAction( _actAlignModel->qaction());
    //_ui->mainToolBar->addAction( _actCentreModel->qaction());
    //_ui->mainToolBar->addAction( _actScaleModel->qaction());
    //_ui->mainToolBar->addAction( _actReflect->qaction());
    _ui->mainToolBar->addAction( _actToggleCameraActorInteraction->qaction());

    _ui->mainToolBar->addSeparator();

    _ui->mainToolBar->addAction( _actOrientCameraToL->qaction());
    _ui->mainToolBar->addAction( _actOrientCameraToF->qaction());
    _ui->mainToolBar->addAction( _actOrientCameraToR->qaction());
    _ui->mainToolBar->addAction( _actSetParallelProjection->qaction());
    _ui->mainToolBar->addAction( _actSynchroniseCameras->qaction());

    // Append a space and then the logo to the toolbar.
    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ui->mainToolBar->addWidget(emptySpacer);
    QLabel* label = new QLabel;
    label->setText( QString("<a href=\"") + APP_WEBSITE + QString("\"><img src=\":/logos/TOOLBAR_LOGO\"></a>"));
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);
    _ui->mainToolBar->addWidget(label);
}   // end _createToolBar


void ClinifaceMain::_createContextMenu()
{
    _cmenuHandler->addAction( _actSetFocus);
    _cmenuHandler->addSeparator();
    _cmenuHandler->addAction( _actAddPath);
    _cmenuHandler->addAction( _actRenamePath);
    _cmenuHandler->addAction( _actDeletePath);
    _cmenuHandler->addAction( _actRestoreSingleLandmark);
    _cmenuHandler->addSeparator();
    _cmenuHandler->addAction( _actRadialSelect);
    _cmenuHandler->addAction( _actDiscardManifold);
    _cmenuHandler->addAction( _actRemoveManifolds);

    // Necessary to add these actions to the main widget otherwise they can't fire if the context menu isn't displayed!
    this->addAction(_actSetFocus->qaction());
    this->addAction(_actAddPath->qaction());
    this->addAction(_actRenamePath->qaction());
    this->addAction(_actDeletePath->qaction());
}   // end _createContextMenu


void ClinifaceMain::_createHandlers()
{
    using namespace FaceTools::Interactor;
    _cmenuHandler = ContextMenuHandler::create();
    _pathsHandler = PathsHandler::create();
    _lmksHandler = LandmarksHandler::create();
    _rselHandler = RadialSelectHandler::create();
    _aclkHandler = ActionClickHandler::create();

    MS::registerHandler( &*_aclkHandler);
    MS::registerHandler( &*_cmenuHandler);
    MS::registerHandler( &*_rselHandler);
    MS::registerHandler( &*_pathsHandler);
    MS::registerHandler( &*_lmksHandler);
    MS::finishRegisteringHandlers();
}   // end _createHandlers


void ClinifaceMain::_createActions()
{
    using namespace FaceTools::Report;
    using namespace FaceTools::Vis;

    _actLoad = new ActionLoad( "Open", QIcon(":/icons/LOAD"));
    _actSaveAsFaceModel = new ActionSaveAsFaceModel( "Save As", QIcon(":/icons/SAVE_AS"));
    _actSaveFaceModel = new ActionSaveFaceModel( "Save", QIcon(":/icons/SAVE"));
    _actSaveFaceModel->setSaveAsAction( _actSaveAsFaceModel);
    _actExportMask = new ActionExportMask( "Export Mask", QIcon(":/icons/SAVE_AS"));

    _actExportPDF = new ActionExportPDF( "Create Template Report", QIcon(":/icons/PDF"), QKeySequence::Print);
    _actCloseFaceModel = new ActionCloseFaceModel( "Close", QIcon(":/icons/CLOSE"));
    _actCloseAllFaceModels = new ActionCloseAllFaceModels( "Close All", QIcon(":/icons/CLOSE_ALL"));
    _actExportMetaData = new ActionExportMetaData( "Export Metadata", QIcon(":/icons/CODE_FILE"));
    _actImportMetaData = new ActionImportMetaData( "Import Metadata", QIcon(":/icons/CODE_FILE"));

    _actRedo = new ActionRedo( "Redo", QIcon(":/icons/REDO"));
    _actUndo = new ActionUndo( "Undo", QIcon(":/icons/UNDO"));

    _actVisTexture = new ActionVisualise( "Texture", QIcon(":/icons/TEXTURE"), new TextureVisualisation, Qt::Key_T);
    _actVisTexture->addTriggerEvent( Event::LOADED_MODEL);
    _actVisTexture->addRefreshEvent( Event::VIEW_CHANGE);
    _actVisWireframe = new ActionVisualise( "Wireframe", QIcon(":/icons/WIREFRAME"), new WireframeVisualisation, Qt::Key_W);
    _actVisOutlines = new ActionVisualise( "Manifold Boundaries", QIcon(":/icons/OUTLINES"), new OutlinesVisualisation, Qt::Key_O);
    _actVisMedianPlane = new ActionVisualise( "Median Plane", QIcon(":/icons/XPLANE"), new PlaneVisualisation(0), Qt::Key_P);
    _actVisTransversePlane = new ActionVisualise( "Transverse Plane", QIcon(":/icons/YPLANE"), new PlaneVisualisation(1), Qt::Key_Q);
    //_actVisFrontalPlane = new ActionVisualise( "Frontal Plane", QIcon(":/icons/ZPLANE"), new PlaneVisualisation(2), Qt::Key_R);
    _actToggleMask = new ActionToggleMask( "Show Correspondence Mask", QIcon(":/icons/MASK"), Qt::Key_M);

    _actVisPolyLabels = new ActionVisualise( "Triangle Labels", QIcon(":/icons/NUMBERS"), new LabelsVisualisation<PolyLabelsView>, Qt::SHIFT + Qt::Key_F);
    _actVisVertexLabels = new ActionVisualise( "Vertex Labels", QIcon(":/icons/NUMBERS"), new LabelsVisualisation<VertexLabelsView>, Qt::SHIFT + Qt::Key_V);

    // Landmarks
    _actEditLandmarks = new ActionEditLandmarks( "Edit Landmarks", QIcon(":/icons/MARKER"), Qt::SHIFT + Qt::Key_L);
    _actRestoreSingleLandmark = new ActionRestoreSingleLandmark( "Restore Landmark Position", QIcon(":/icons/RESTORE"));
    _actRestoreLandmarks = new ActionRestoreLandmarks( "Restore Detected Landmarks", QIcon(":/icons/RESTORE"));
    _actAlignLandmarks = new ActionAlignLandmarks( "Align Landmark Positions", QIcon(":/icons/ALIGN_CENTRE"));
    _actShowLandmarks = new ActionVisualise( "Show Landmarks", QIcon(":/icons/MARKER"), &_lmksHandler->visualisation(), Qt::Key_L);
    _actShowLandmarks->setToolTip( "Toggle the facial landmarks on and off.");
    _actShowLandmarks->addTriggerEvent( Event::LOADED_MODEL | Event::MASK_CHANGE);
    _actShowLandmarks->addRefreshEvent( Event::MESH_CHANGE);
    _actVisLandmarkLabels = new ActionVisualise( "Show Landmark Labels", QIcon(":/icons/TAGS"), new LabelsVisualisation<LandmarkLabelsView>);
    _actVisLandmarkLabels->setToolTip( "Toggle the landmark labels on and off.");
    _actEditLandmarks->setShowLandmarksAction( _actShowLandmarks);
    _actEditLandmarks->setAlignLandmarksAction( _actAlignLandmarks);
    _actEditLandmarks->setRestoreLandmarksAction( _actRestoreLandmarks);
    _actEditLandmarks->setShowLandmarkLabelsAction( _actVisLandmarkLabels);

    _actOrientCameraToF = new ActionOrientCameraToFace("View Front Profile", QIcon(":/icons/ORIENT_CAMERA"), Qt::Key_Space, 1.0f, 1, 0.0f);
    _actOrientCameraToF->addTriggerEvent( Event::VIEWER_CHANGE);
    _actOrientCameraToF->setToolTip( tr("Set the camera to look at the front profile."));

    _actOrientCameraToL = new ActionOrientCameraToFace("View Right Profile", QIcon(":/icons/LOOK_RIGHT"), Qt::Key_Left, 1.0f, 1, -EIGEN_PI/2);
    _actOrientCameraToL->setToolTip( tr("Set the camera to look at the subject's right profile."));
    _actOrientCameraToLQ = new ActionOrientCameraToFace("View Right Quarter", QIcon(":/icons/LOOK_RIGHT"), Qt::SHIFT + Qt::Key_Left, 1.0f, 1, -EIGEN_PI/4);
    _actOrientCameraToLQ->setToolTip( tr("Set the camera to look at the subject's front-right profile."));
    _actOrientCameraToR = new ActionOrientCameraToFace("View Left Profile", QIcon(":/icons/LOOK_LEFT"), Qt::Key_Right, 1.0f, 1, EIGEN_PI/2);
    _actOrientCameraToR->setToolTip( tr("Set the camera to look at the subject's left profile."));
    _actOrientCameraToRQ = new ActionOrientCameraToFace("View Left Quarter", QIcon(":/icons/LOOK_LEFT"), Qt::SHIFT + Qt::Key_Right, 1.0f, 1, EIGEN_PI/4);
    _actOrientCameraToRQ->setToolTip( tr("Set the camera to look at the subject's front-left profile."));
    _actOrientCameraToT = new ActionOrientCameraToFace("View From Above", QIcon(), Qt::Key_Up, 1.0f, 0, -EIGEN_PI/2);
    _actOrientCameraToT->setToolTip( tr("Set the camera to look down at the subject from above."));
    _actOrientCameraToTQ = new ActionOrientCameraToFace("View Top Quarter", QIcon(), Qt::SHIFT + Qt::Key_Up, 1.0f, 0, -EIGEN_PI/4);
    _actOrientCameraToTQ->setToolTip( tr("Set the camera to look at the subject from above and the front."));
    _actOrientCameraToB = new ActionOrientCameraToFace("View From Below", QIcon(), Qt::Key_Down, 1.0f, 0, EIGEN_PI/2);
    _actOrientCameraToB->setToolTip( tr("Set the camera to look up at the subject from below."));
    _actOrientCameraToBQ = new ActionOrientCameraToFace("View Bottom Quarter", QIcon(), Qt::SHIFT + Qt::Key_Down, 1.0f, 0, EIGEN_PI/4);
    _actOrientCameraToBQ->setToolTip( tr("Set the camera to look at the subject from below and the front."));

    _actCentreModel = new ActionCentreModel("Centre Model", QIcon(":/icons/CENTRE"), Qt::SHIFT + Qt::Key_O);
    _actAlignModel = new ActionAlignModel( "Align Model", QIcon(":/icons/TRANSFORM"), Qt::SHIFT + Qt::Key_Space);

    _actDiscardManifold = new ActionDiscardManifold( "Discard Manifold", QIcon(":/icons/FACE"));
    _actRemoveManifolds = new ActionRemoveManifolds( "Discard OTHER Manifolds", QIcon(":/icons/FACE"));

    _actDetectFace = new ActionDetectFace( "Detect Face/Landmarks", QIcon(":/icons/DETECT_FACE"));
    _actResetDetection = new ActionResetDetection( "Discard Face Detection", QIcon(":/icons/RESET_DETECT_FACE"));

    _actSetParallelProjection = new ActionSetParallelProjection( "Orthographic Projection", QIcon(":/icons/ORTHOGRAPHIC"), Qt::Key_E);

    _actSaveScreenshot = new ActionSaveScreenshot( "Take Screenshot (All Viewers)", QIcon(":/icons/SCREENSHOT"), Qt::Key_S);
    _actSmooth = new ActionSmooth( "Smooth Surface", QIcon(":/icons/SHAVE1"));
    _actReflect = new ActionReflect( "Reflect Laterally", QIcon(":/icons/REFLECT"), Qt::SHIFT + Qt::Key_R);

    _actToggleAxes = new ActionToggleAxes( "World Axes", QIcon(":/icons/AXES"), Qt::Key_A);

    _actToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Manually Reposition", QIcon(":/icons/MOVE_MODEL"), Qt::Key_Z);
    _actSynchroniseCameras = new ActionSynchroniseCameras( "Synchronise Cameras", QIcon(":/icons/SYNCH_CAMERAS"));
    _actMarquee = new ActionMarquee("Marquee Mode", QIcon(":/icons/PROJECTOR"));

    _actSetFocus = new ActionSetFocus( "Set Camera Focus", QIcon(":/icons/FOCUS"), Qt::Key_F);
    _aclkHandler->addLeftDoubleClickAction( _actSetFocus);

    _actRadialSelect = new ActionRadialSelect( "Select Radial Area", QIcon(":/icons/LASSO"));
    _actExtractFace = new ActionExtractFace( "Extract Facial Region", QIcon(":/icons/CUT_OUT"));

    _actMakeLeftFace = new ActionMakeHalfFace( "Create Right Symmetric Face", QIcon(":/icons/LEFT_FACE"));
    _actMakeLeftFace->setPlane( Vec3f(-1,0,0));

    _actMakeRightFace = new ActionMakeHalfFace( "Create Left Symmetric Face", QIcon(":/icons/RIGHT_FACE"));
    _actMakeRightFace->setPlane( Vec3f(1,0,0));

    _actFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"));
    _actSetOpacity = new ActionSetOpacity( "Surface Opacity");
    _actBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"), Qt::Key_B);

    _actEditPaths = new ActionEditPaths( "Show Calliper Measurements", QIcon(":/icons/CALIPERS"), Qt::SHIFT + Qt::Key_C);
    _actAddPath = new ActionAddPath( "Add Calliper Measurement", QIcon(":/icons/CALIPERS"), Qt::Key_C);
    _actDeletePath = new ActionDeletePath( "Delete Calliper Measurement", QIcon(":/icons/ERASER"), Qt::Key_Delete);
    _actRenamePath = new ActionRenamePath( "Rename Calliper Measurement", QIcon(":/icons/EDIT"), Qt::CTRL + Qt::Key_C);
    _actDeleteAllPaths = new ActionDeleteAllPaths( "Delete ALL Calliper Measurements", QIcon(":/icons/ERASER"));

    //_actSetSurfaceColour = new ActionSetSurfaceColour( "Base Colour", this);
    //_actSetMinScalarColour = new ActionSetMinScalarColour( "Minimum Colour");
    //_actSetMaxScalarColour = new ActionSetMaxScalarColour( "Maximum Colour");
#ifndef _WIN32
    // Lock these functions for now on Linux because Qt isn't behaving well with GTK
    //_actSetMinScalarColour->setLocked(true);
    //_actSetMaxScalarColour->setLocked(true);
#endif
    _actSetNumScalarColours = new ActionSetNumScalarColours( "Number of Colours for Scalar Visualisations");
    _actChangeSurfaceMappingRange = new ActionChangeSurfaceMappingRange( "Min/Max Range for Scalar Visualisations");

    _actToggleScalarLegend = new ActionToggleScalarLegend( "Scalar Legend");
    _actToggleStereoRendering = new ActionToggleStereoRendering( "3D (Red/Blue)", QIcon(":/icons/3D_GLASSES"));

    _actRotateX90 = new ActionRotateModel( "Rotate 90 degrees about X", QIcon(":/icons/ROTATE_X90"), Vec3f(1,0,0), 90);
    _actRotateY90 = new ActionRotateModel( "Rotate 90 degrees about Y", QIcon(":/icons/ROTATE_Y90"), Vec3f(0,1,0), 90);
    _actRotateZ90 = new ActionRotateModel( "Rotate 90 degrees about Z", QIcon(":/icons/ROTATE_Z90"), Vec3f(0,0,1), 90);
    _actScaleModel = new ActionScaleModel( "Rescale", QIcon(":/icons/RESIZE_MODEL"));
#ifndef NDEBUG
    _actFixNormals = new ActionFixNormals( "Fix Incongruent Normals", QIcon(":/icons/NORMAL_FLIP"));
#endif
    _actInvertNormals = new ActionInvertNormals( "Invert Normals", QIcon(":/icons/NORMAL_FLIP"));
}   // end _createActions


void ClinifaceMain::_setupMainViewer()
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
}   // end _setupMainViewer


//#include <QDockWidget>
void ClinifaceMain::_createMetrics()
{
    _actShowScanInfo = new ActionShowScanInfo( "Assessment Information", QIcon(":/icons/IMAGE_DETAILS"), Qt::SHIFT + Qt::Key_I);
    _actShowMeshInfo = new ActionShowMeshInfo( "Model Properties", QIcon(":/icons/MODEL_PROPERTIES"), QString("`"));
    _actShowMetrics = new ActionShowMetrics( "Measurements Browser", QIcon(":/icons/METRICS1"), Qt::SHIFT + Qt::Key_M);
    _actShowPhenotypes = new ActionShowPhenotypes( "HPO Browser", QIcon(":/icons/FACEQ"), Qt::SHIFT + Qt::Key_H);
    ActionUpdateThumbnail* actUpdateThumbnail = new ActionUpdateThumbnail;

    FAM::registerAction( actUpdateThumbnail);
    FAM::registerAction( _actShowScanInfo);
    FAM::registerAction( _actShowMeshInfo);
    FAM::registerAction( _actShowMetrics);
    FAM::registerAction( _actShowPhenotypes);

    _actShowScanInfo->setThumbnailUpdater( actUpdateThumbnail);

    using namespace FaceTools::Widget;
    MetricsDialog *mdialog = static_cast<MetricsDialog*>( _actShowMetrics->widget());
    mdialog->setShowScanInfoAction( _actShowScanInfo->qaction());
    mdialog->setShowPhenotypesAction( _actShowPhenotypes->qaction());
    PhenotypesDialog *pdialog = static_cast<PhenotypesDialog*>( _actShowPhenotypes->widget());

    connect( mdialog, &MetricsDialog::onUpdateMatchingPhenotypes, [pdialog]( const IntSet &hids){ pdialog->showPhenotypes(hids);});
    // When user selects an HPO term from the MeasurementsBrowser, set this as the selected row in the HPO Browser.
    connect( mdialog, &MetricsDialog::onSelectedHPOTerm, [pdialog]( int hid){ pdialog->selectHPO( hid);});
    // When user selects a row in the HPO Browser, select it in the Measurements Browser but *only* if
    // currently in atypical match mode!
    connect( pdialog, &PhenotypesDialog::onSelectedHPOTerm,
            [mdialog]( int hid){ mdialog->selectHPO( mdialog->isShowingAtypical() ? hid : -1);});
    /*
    QDockWidget *dock = new QDockWidget( tr("Measurements Browser"), this);
    dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget( mdialog);
    addDockWidget( Qt::RightDockWidgetArea, dock);
    */
}   // end _createMetrics


void ClinifaceMain::_loadPlugins()
{
    std::cerr << "Loading plugins... (requiring token " << APP_PLUGIN_TOKEN << ")" << std::endl;
    connect( _ploader, &ClinifacePluginsLoader::onAttachToMenu, this, &ClinifaceMain::_doOnAttachPluginActionToUI);
    // First load the plugins that come with the base platform.
    _ploader->loadPlugins( QApplication::applicationDirPath() + "/plugins");
    // Then load the plugins in the user's Cliniface configuration directory.
    // Don't do this if it's a debug build since the user directory will contain release version plugins.
#ifdef NDEBUG
    const QString userPluginsDir = QDir::home().filePath( QString(".%1/plugins").arg(EXE_NAME));
    _ploader->loadPlugins( userPluginsDir);
#endif
}   // end _loadPlugins


// public
ClinifaceMain::ClinifaceMain()
    : QMainWindow(nullptr), _ui(new Ui::ClinifaceMain)
{
    _ui->setupUi(this);
    setWindowTitle( appName());
    setAcceptDrops(true);   // Accept dropping of files onto this widget
    setContextMenuPolicy(Qt::NoContextMenu);

    _mfmv = new FaceTools::MultiFaceModelViewer( this);
    MS::addViewer( _mfmv->leftViewer());
    MS::addViewer( _mfmv->centreViewer(), true/*default*/); 
    MS::addViewer( _mfmv->rightViewer());
    MS::setStatusBar( _ui->statusBar);

    FAM::get( this);  // Creates and passes self in as the parent widget for all added actions

    _setupMainViewer();

    _createHandlers();
    _createActions();
    _createMetrics();

    _updatesDialog = new UpdatesDialog(this);
    _prefsDialog = new PreferencesDialog(this);
    // Cause a MODEL_SELECT event which will cause all actions to refresh
    connect( _prefsDialog, &PreferencesDialog::onUpdated, [](){ FAM::get()->doEvent( Event::MODEL_SELECT);});
    connect( _prefsDialog, &PreferencesDialog::showHelp, [this](){ _helpAss->show("preferences.html");});

    _ploader = new ClinifacePluginsLoader( this);
    _ploader->dialog()->setWindowTitle( appName() + QString(" | Plugins"));

    _registerActions();

    QWidget* cwidget = new QWidget;
    cwidget->setLayout( new QVBoxLayout);
    cwidget->layout()->setContentsMargins(0,0,0,0);
    cwidget->layout()->addWidget( _mfmv);
    setCentralWidget( cwidget);
    _mfmv->layout()->setContentsMargins(1,1,1,1);

    _createToolBar();
    _createFileMenu();
    _createViewMenu();
    _createCameraMenu();
    _createMetricsMenu();
    _createTransformMenu();
    _createGeometryMenu();
    _createContextMenu();

    static const QString htmldir = QDir( QApplication::applicationDirPath()).absoluteFilePath( HTML_DIR);
    _helpAss = new QTools::HelpAssistant( htmldir, this);
    _helpAss->addContent( HTML_PLUGINS_DIR);

    _createHelpMenu();

    // Set the plugin UI points for loaded plugins
    _ppoints.set("View", _ui->menu_View);
    _ppoints.set("Camera", _ui->menu_Camera);
    _ppoints.set("Metrics", _ui->menu_Metrics);
    _ppoints.set("Geometry", _ui->menu_Geometry);
    _ppoints.set("Transform", _ui->menu_Transform);
    _ppoints.set("Help", _ui->menu_Help);
    _ppoints.set("Main", _ui->mainToolBar);
    _ppoints.set("Visualisations", _ui->visToolBar);
    _ppoints.set("Surface Mapping", _ui->scmapToolBar);

    _loadPlugins();

    // Refresh the contents here because plugins must dump their help docs into the prescribed place first.
    if ( !_helpAss->refreshContents(":/data/HELP_TOC"))
        std::cerr << "Help TOC refresh failure!" << std::endl;

    // Update viewer selection model lists and window title whenever the selected model changes.
    connect( &*FAM::get(), &FAM::onUpdateSelected, _mfmv, &FaceTools::MultiFaceModelViewer::doOnUpdateModelLists);
    connect( &*FAM::get(), &FAM::onUpdateSelected, this, &ClinifaceMain::_doOnUpdateSelected);
    connect( &*FAM::get(), &FAM::onShowHelp, [this]( const QString& tok){ _helpAss->show(tok);});

    // Locate centrally on desktop
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), QGuiApplication::primaryScreen()->geometry()));
    _doOnUpdateSelected();
}   // end ctor


// public
ClinifaceMain::~ClinifaceMain()
{
    delete _helpAss;
    delete _ploader;
    delete _ui;
}   // end dtor


// protected virtual
QSize ClinifaceMain::sizeHint() const { return QSize( 840, 700);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname) { return _actLoad->load( fname);}


void ClinifaceMain::checkForUpdate() { _updatesDialog->checkForUpdateAction()->trigger();}


// protected virtual
void ClinifaceMain::dragEnterEvent( QDragEnterEvent *evt)
{
    if ( evt->mimeData()->hasText())
    {
        const QString fname = QUrl( evt->mimeData()->text()).toLocalFile().trimmed();
        if ( FMM::canRead( fname))
            evt->acceptProposedAction();
    }   // end if
}   // end dragEnterEvent


// protected virtual
void ClinifaceMain::dropEvent( QDropEvent *evt)
{
    const QString fname = QUrl( evt->mimeData()->text()).toLocalFile().trimmed();
    if ( loadModel( fname))
        evt->acceptProposedAction();
}   // end dropEvent


// protected virtual
void ClinifaceMain::closeEvent( QCloseEvent *evt)
{
    if ( _actCloseAllFaceModels->isEnabled() && !_actCloseAllFaceModels->execute(Event::USER))
        evt->ignore();
}   // end closeEvent


void ClinifaceMain::_doOnUpdateSelected()
{
    QString wtitle = QString( "%1 %2").arg( appName(), APP_VERSION_STRING);

    const FaceTools::FM *fm = MS::selectedModel();
    if ( fm)
    {
        QString mfile = FMM::filepath(fm);
        if ( fm->assessmentsCount() > 1)
            mfile += " [" + fm->currentAssessment()->assessor() + "]";
        if ( !fm->isSaved())
            mfile += " (*)";
        wtitle = QString("%1 | %2").arg( appName(), mfile);
    }   // end if

    setWindowTitle( wtitle);
    setFocus( Qt::PopupFocusReason);
}   // end _doOnUpdateSelected


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

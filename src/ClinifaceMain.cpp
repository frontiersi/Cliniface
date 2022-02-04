/************************************************************************
 * Copyright (C) 2022 SIS Research Ltd & Richard Palmer
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
#include <FaceTools/Action/ActionClose.h>
#include <FaceTools/Action/ActionCloseAll.h>
#include <FaceTools/Action/ActionSelect.h>
#include <FaceTools/Action/ActionSmooth.h>
#include <FaceTools/Action/ActionCopyView.h>
#include <FaceTools/Action/ActionMapDelta.h>
#include <FaceTools/Action/ActionMoveView.h>
#include <FaceTools/Action/ActionSetFocus.h>
#include <FaceTools/Action/ActionVisualise.h>
#include <FaceTools/Action/ActionFillHoles.h>
#include <FaceTools/Action/ActionUpdateU3D.h>
#include <FaceTools/Action/ActionAlignModel.h>
#include <FaceTools/Action/ActionDetectFace.h>
#include <FaceTools/Action/ActionFixNormals.h>
#include <FaceTools/Action/ActionToggleMask.h>
#include <FaceTools/Action/ActionCentreModel.h>
#include <FaceTools/Action/ActionMapSymmetry.h>
#include <FaceTools/Action/ActionResizeModel.h>
#include <FaceTools/Action/ActionRotateModel.h>
#include <FaceTools/Action/ActionUpdateStats.h>
#include <FaceTools/Action/ActionOrientCamera.h>
#include <FaceTools/Action/ActionRotateCamera.h>
#include <FaceTools/Action/ActionMapCurvature.h>
#include <FaceTools/Action/ActionReflectModel.h>
#include <FaceTools/Action/ActionInvertNormals.h>
#include <FaceTools/Action/ActionMirrorLandmarks.h>
#include <FaceTools/Action/ActionExportMetaData.h>
#include <FaceTools/Action/ActionImportMetaData.h>
#include <FaceTools/Action/ActionResetDetection.h>
#include <FaceTools/Action/ActionBackfaceCulling.h>
#include <FaceTools/Action/ActionUpdateThumbnail.h>
#include <FaceTools/Action/ActionRestoreLandmarks.h>
#include <FaceTools/Action/ActionSetSurfaceColour.h>
#include <FaceTools/Action/ActionSetMinScalarColour.h>
#include <FaceTools/Action/ActionSetMaxScalarColour.h>
#include <FaceTools/Action/ActionUpdateMeasurements.h>
#include <FaceTools/Action/ActionSetNumScalarColours.h>
#include <FaceTools/Action/ActionChangeColourMappingRange.h>

#include <FaceTools/Action/FaceActionManager.h>

#include <FaceTools/Report/ReportManager.h>

#include <FaceTools/Vis/WireframeVisualisation.h>
#include <FaceTools/Vis/OutlinesVisualisation.h>
#include <FaceTools/Vis/TextureVisualisation.h>
#include <FaceTools/Vis/LabelsVisualisation.h>
#include <FaceTools/Vis/PlaneVisualisation.h>
#include <FaceTools/Vis/LandmarkLabelsView.h>
#include <FaceTools/Vis/VertexLabelsView.h>
#include <FaceTools/Vis/PolyLabelsView.h>

#include <FaceTools/MiscFunctions.h>    // loadTextFromFile
#include <FaceTools/ModelSelect.h>

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
using MS = FaceTools::ModelSelect;


namespace {

QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, d, &QDialog::open);
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
       << "<body id=\"_top\">" << Qt::endl << Qt::endl
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
    // Lowest priority
    FAM::registerAction( _actRedo, this);
    FAM::registerAction( _actUndo, this);
    FAM::registerAction( _actLoad, this);
    FAM::registerAction( _actSave, this);
    FAM::registerAction( _actSaveAs, this);
    FAM::registerAction( _actClose, this);
    FAM::registerAction( _actCloseAll, this);
    FAM::registerAction( _actExportPDF, this);
    FAM::registerAction( _actExportMask, this);
    FAM::registerAction( _actExportMetaData, this);
    FAM::registerAction( _actImportMetaData, this);
    FAM::registerAction( _actSaveScreenshot, this);

    // Interactions are low priority because data and views must be up to date first
    FAM::registerAction( _actAddPath, this);
    FAM::registerAction( _actCopyPath, this);
    FAM::registerAction( _actDeletePath, this);
    FAM::registerAction( _actRenamePath, this);
    FAM::registerAction( _actCopyAllPaths, this);
    FAM::registerAction( _actRadialSelect, this);
    FAM::registerAction( _actDeleteAllPaths, this);
    FAM::registerAction( _actRestoreSingleLandmark, this);
    FAM::registerAction( _actToggleCameraActorInteraction, this);
    ActionUpdateU3D *actUpdateU3D = new ActionUpdateU3D;
    //actUpdateU3D->setLocked( true);  // Uncomment for debug
    FAM::registerAction( actUpdateU3D, this);

    // Camera actions rely on data changes being finished
    FAM::registerAction( _actMarquee, this);
    FAM::registerAction( _actSetFocus, this);
    FAM::registerAction( _actCentreModel, this);
    FAM::registerAction( _actOrientCameraToF, this);
    FAM::registerAction( _actOrientCameraToL, this);
    FAM::registerAction( _actOrientCameraToR, this);
    FAM::registerAction( _actBackfaceCulling, this);
    FAM::registerAction( _actOrientCameraToB, this);
    FAM::registerAction( _actOrientCameraToT, this);
    FAM::registerAction( _actOrientCameraToLQ, this);
    FAM::registerAction( _actOrientCameraToRQ, this);
    FAM::registerAction( _actOrientCameraToTQ, this);
    FAM::registerAction( _actOrientCameraToBQ, this);
    FAM::registerAction( _actRotateCameraL, this);
    FAM::registerAction( _actRotateCameraR, this);
    FAM::registerAction( _actRotateCameraU, this);
    FAM::registerAction( _actRotateCameraD, this);
    FAM::registerAction( _actSynchroniseCameras, this);
    FAM::registerAction( _actSetParallelProjection, this);
    FAM::registerAction( _actToggleStereo, this);
    FAM::registerAction( new ActionSelect, this);

    // Visualisations require the mesh and meta data to be up to date
    FAM::registerAction( _actEditPaths, this);
    FAM::registerAction( _actToggleMask, this);
    FAM::registerAction( _actToggleAxes, this);
    FAM::registerAction( _actToggleLegend, this);
    FAM::registerAction( _actVisWireframe, this);
    FAM::registerAction( _actVisPolyLabels, this);
    FAM::registerAction( _actVisMedianPlane, this);
    FAM::registerAction( _actVisVertexLabels, this);
    FAM::registerAction( _actVisLandmarkLabels, this);
    //FAM::registerAction( _actVisFrontalPlane, this);
    FAM::registerAction( _actVisTransversePlane, this);
    //FAM::registerAction( _actSetMinScalarColour, this);
    //FAM::registerAction( _actSetMaxScalarColour, this);
    FAM::registerAction( _actSetNumScalarColours, this);
    FAM::registerAction( _actChangeColourMappingRange, this);
    FAM::registerAction( _actShowLandmarks, this);
    FAM::registerAction( _actVisTexture, this);
    FAM::registerAction( _actSetOpacity, this);

    // Mesh changes
    FAM::registerAction( _actSmooth, this);
    FAM::registerAction( _actFillHoles, this);
    FAM::registerAction( _actRotateX90, this);
    FAM::registerAction( _actRotateY90, this);
    FAM::registerAction( _actRotateZ90, this);
    FAM::registerAction( _actAlignModel, this);
    FAM::registerAction( _actResizeModel, this);
    FAM::registerAction( _actExtractFace, this);
    FAM::registerAction( _actMakeLeftFace, this);
    FAM::registerAction( _actReflectModel, this);
    FAM::registerAction( _actMakeRightFace, this);
    FAM::registerAction( _actInvertNormals, this);
    FAM::registerAction( _actDiscardManifold, this);
    FAM::registerAction( _actRemoveManifolds, this);
#ifndef NDEBUG
    FAM::registerAction( _actFixNormals, this);
#endif
    FAM::registerAction( new ActionMapDelta, this);
    FAM::registerAction( new ActionMapSymmetry, this);
    FAM::registerAction( new ActionMapCurvature, this);

    // Require metadata to be up to date
    FAM::registerAction( _actDetectFace, this);
    FAM::registerAction( _actEditLandmarks, this);
    FAM::registerAction( _actResetDetection, this);
    FAM::registerAction( _actMirrorLandmarks, this);
    FAM::registerAction( _actRestoreLandmarks, this);

    // Metadata changes
    FAM::registerAction( new ActionUpdateStats, this);
    FAM::registerAction( new ActionUpdateMeasurements, this);

    connect( _ui->action_VisualisationsToolbar, &QAction::triggered, [=](bool v){ _ui->visToolBar->setVisible(v); _ui->scmapToolBar->setVisible(v);});
    connect( _ui->action_Exit, &QAction::triggered, this, &ClinifaceMain::close);
}   // end _registerActions


void ClinifaceMain::_createFileMenu()
{
    _ui->menu_File->addAction( _actLoad->qaction());
    _ui->menu_File->addAction( _actSave->qaction());
    _ui->menu_File->addAction( _actSaveAs->qaction());
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

    _ui->menu_File->addAction( _actClose->qaction());
    _ui->menu_File->addAction( _actCloseAll->qaction());
    _ui->menu_File->addSeparator();
    _ui->action_Exit->setShortcut( QKeySequence::Quit);
    _ui->menu_File->addAction( _ui->action_Exit);
}   // end _createFileMenu


void ClinifaceMain::_createViewMenu()
{
    _ui->menu_View->addAction( _actVisTexture->qaction());
    _ui->menu_View->addAction( _actVisWireframe->qaction());
    _ui->menu_View->addAction( _actToggleMask->qaction());
    _ui->menu_View->addSeparator();
    _ui->menu_View->addAction( _actVisMedianPlane->qaction());
    _ui->menu_View->addAction( _actVisTransversePlane->qaction());
    //_ui->menu_View->addAction( _actVisFrontalPlane->qaction());
    //_ui->menu_View->addSeparator();

    _ui->menu_View->addAction( _actRadialSelect->qaction());
    _ui->menu_View->addAction( _actVisPolyLabels->qaction());
    _ui->menu_View->addAction( _actVisVertexLabels->qaction());

    QMenu *surfaceMenu = _ui->menu_View->addMenu( "&Colour Mapping...");
    _ppoints.set("Colour Mapping", surfaceMenu);
    surfaceMenu->setIcon( QIcon(":/icons/COLOURS"));
    surfaceMenu->addAction( _actToggleLegend->qaction());
    surfaceMenu->addSeparator();

    _ui->menu_View->addSeparator();

    _ui->menu_View->addAction( _actBackfaceCulling->qaction());
    _ui->menu_View->addAction( _actToggleAxes->qaction());
    _ui->menu_View->addAction( _actSaveScreenshot->qaction());
    _ui->menu_View->addAction( _actToggleStereo->qaction());
    _ui->menu_View->addAction( _ui->action_VisualisationsToolbar);

    /****** _ui->visToolBar ******/
    _ui->visToolBar->addAction( _actVisTexture->qaction());
    _ui->visToolBar->addAction( _actShowLandmarks->qaction());
    _ui->visToolBar->addAction( _actShowMetrics->qaction());
    _ui->visToolBar->addAction( _actEditPaths->qaction());
    //_ui->visToolBar->addAction( _actRadialSelect->qaction());
    _ui->visToolBar->addSeparator();
    _ui->visToolBar->addWidget( _actSetOpacity->widget());
    _ui->visToolBar->addAction( _actToggleAxes->qaction());

    /****** _ui->scmapToolBar ******/
    _ui->scmapToolBar->addWidget( _actChangeColourMappingRange->widget());
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
    _ui->menu_Camera->addAction( _actRotateCameraL->qaction());
    _ui->menu_Camera->addAction( _actRotateCameraR->qaction());
    _ui->menu_Camera->addAction( _actRotateCameraU->qaction());
    _ui->menu_Camera->addAction( _actRotateCameraD->qaction());
    _ui->menu_Camera->addSeparator();
    _ui->menu_Camera->addAction( _actSetParallelProjection->qaction());
    _ui->menu_Camera->addAction( _actSynchroniseCameras->qaction());
    _ui->menu_Camera->addAction( _actMarquee->qaction());
}   // end _createCameraMenu


void ClinifaceMain::_createTransformMenu()
{
    _ui->menu_Transform->addAction( _actAlignModel->qaction());
    _ui->menu_Transform->addAction( _actCentreModel->qaction());
    _ui->menu_Transform->addAction( _actResizeModel->qaction());
    _ui->menu_Transform->addAction( _actReflectModel->qaction());
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
    _ui->menu_Metrics->addAction( _actRestoreLandmarks->qaction());
    _ui->menu_Metrics->addAction( _actEditLandmarks->qaction());
    _ui->menu_Metrics->addAction( _actShowLandmarks->qaction());
    _ui->menu_Metrics->addSeparator();
    _ui->menu_Metrics->addAction( _actShowMetrics->qaction());
    _ui->menu_Metrics->addAction( _actShowPhenotypes->qaction());
    _ui->menu_Metrics->addAction( _actEditPaths->qaction());
    _ui->menu_Metrics->addAction( _actCopyAllPaths->qaction());
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
    _ui->menu_Help->addAction( connectDialog( _ui->action_CheckUpdate, _updatesDialog));
}   // end _createHelpMenu


void ClinifaceMain::_createToolBar()
{
    _ui->mainToolBar->addAction( _actLoad->qaction());
    _ui->mainToolBar->addAction( _actSave->qaction());
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
    _ui->mainToolBar->addAction( _actToggleCameraActorInteraction->qaction());

    _ui->mainToolBar->addSeparator();

    _ui->mainToolBar->addAction( _actOrientCameraToL->qaction());
    _ui->mainToolBar->addAction( _actOrientCameraToF->qaction());
    _ui->mainToolBar->addAction( _actOrientCameraToR->qaction());
    _ui->mainToolBar->addAction( _actSetParallelProjection->qaction());
    _ui->mainToolBar->addAction( _actSynchroniseCameras->qaction());

    /*
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
    */
}   // end _createToolBar


void ClinifaceMain::_createContextMenu()
{
    _cmenuHandler->addAction( _actSetFocus);
    _cmenuHandler->addSeparator();
    _cmenuHandler->addAction( _actAddPath);
    _cmenuHandler->addAction( _actRenamePath);
    _cmenuHandler->addAction( _actCopyPath);
    _cmenuHandler->addAction( _actDeletePath);
    _cmenuHandler->addSeparator();
    _cmenuHandler->addAction( _actRestoreSingleLandmark);
    _cmenuHandler->addAction( _actRadialSelect);
    _cmenuHandler->addAction( _actCentreModel);

    // Necessary to add these actions to the main widget otherwise they can't
    // fire using their keyboard shortcuts if the context menu isn't displayed!
    this->addAction(_actSetFocus->qaction());
    this->addAction(_actAddPath->qaction());
    this->addAction(_actRenamePath->qaction());
    this->addAction(_actCopyPath->qaction());
    this->addAction(_actDeletePath->qaction());
    this->addAction(_actCentreModel->qaction());
}   // end _createContextMenu


void ClinifaceMain::_createHandlers()
{
    using namespace FaceTools::Interactor;
    _cmenuHandler = ContextMenuHandler::create();
    _pathsHandler = PathsHandler::create();
    _lmksHandler = LandmarksHandler::create();
    _rselHandler = RadialSelectHandler::create();
    _aclkHandler = ActionClickHandler::create();
    //_svalHandler = SurfaceValueHandler::create();

    MS::registerHandler( _aclkHandler.get());
    MS::registerHandler( _cmenuHandler.get());
    MS::registerHandler( _rselHandler.get());
    MS::registerHandler( _pathsHandler.get());
    MS::registerHandler( _lmksHandler.get());
    //MS::registerHandler( _svalHandler.get());
    MS::finishRegisteringHandlers();
}   // end _createHandlers


void ClinifaceMain::_createActions()
{
    using namespace FaceTools::Report;
    using namespace FaceTools::Vis;

    _actLoad = new ActionLoad( "Open", QIcon(":/icons/LOAD"));
    _actSave = new ActionSave( "Save", QIcon(":/icons/SAVE"));
    _actSaveAs = new ActionSaveAs( "Save As", QIcon(":/icons/SAVE_AS"));
    _actSave->setSaveAsAction( _actSaveAs);
    _actExportMask = new ActionExportMask( "Export Mask", QIcon(":/icons/SAVE_AS"));

    _actExportPDF = new ActionExportPDF( "Generate Report", QIcon(":/icons/PDF"), QKeySequence::Print);
    _actClose = new ActionClose( "Close", QIcon(":/icons/CLOSE"));
    _actCloseAll = new ActionCloseAll( "Close All", QIcon(":/icons/CLOSE_ALL"));
    _actExportMetaData = new ActionExportMetaData( "Export Metadata", QIcon(":/icons/CODE_FILE"));
    _actImportMetaData = new ActionImportMetaData( "Import Metadata", QIcon(":/icons/CODE_FILE"));

    _actRedo = new ActionRedo( "Redo", QIcon(":/icons/REDO"));
    _actUndo = new ActionUndo( "Undo", QIcon(":/icons/UNDO"));

    _actVisTexture = new ActionVisualise( "Texture", QIcon(":/icons/TEXTURE"), new TextureVisualisation, Qt::Key_T);
    _actVisTexture->addTriggerEvent( Event::LOADED_MODEL);
    _actVisWireframe = new ActionVisualise( "Wireframe", QIcon(":/icons/WIREFRAME"), new WireframeVisualisation, Qt::Key_W);
    _actVisMedianPlane = new ActionVisualise( "Median Plane", QIcon(":/icons/XPLANE"), new PlaneVisualisation(0), Qt::Key_P);
    _actVisMedianPlane->addRefreshEvent( Event::LOADED_MODEL | Event::MASK_CHANGE);
    _actVisTransversePlane = new ActionVisualise( "Transverse Plane", QIcon(":/icons/YPLANE"), new PlaneVisualisation(1), Qt::Key_Q);
    _actVisTransversePlane->addRefreshEvent( Event::LOADED_MODEL | Event::MASK_CHANGE);
    _actToggleMask = new ActionToggleMask( "Show Correspondence Mask", QIcon(":/icons/MASK"), Qt::Key_M);
    _actToggleMask->addRefreshEvent( Event::LOADED_MODEL | Event::MASK_CHANGE);

    _actVisPolyLabels = new ActionVisualise( "Triangle Labels", QIcon(":/icons/NUMBERS"), new LabelsVisualisation<PolyLabelsView>, Qt::SHIFT + Qt::Key_F);
    _actVisVertexLabels = new ActionVisualise( "Vertex Labels", QIcon(":/icons/NUMBERS"), new LabelsVisualisation<VertexLabelsView>, Qt::SHIFT + Qt::Key_V);

    // Landmarks
    _actEditLandmarks = new ActionEditLandmarks( "Edit Landmarks", QIcon(":/icons/MARKER"), Qt::SHIFT + Qt::Key_L);
    _actRestoreSingleLandmark = new ActionRestoreSingleLandmark( "Reset Landmark", QIcon(":/icons/RESTORE"));
    _actRestoreLandmarks = new ActionRestoreLandmarks( "Reset Landmarks", QIcon(":/icons/RESTORE"));
    _actMirrorLandmarks = new ActionMirrorLandmarks( "Mirror Landmarks", QIcon(":/icons/ALIGN_CENTRE"));
    _actShowLandmarks = new ActionVisualise( "Show Landmarks", QIcon(":/icons/MARKER"), &_lmksHandler->visualisation(), Qt::Key_L);
    _actShowLandmarks->setToolTip( "Toggle the facial landmarks on and off.");
    _actShowLandmarks->addTriggerEvent( Event::LOADED_MODEL | Event::MASK_CHANGE);
    // Note that the landmarks visualisation needs to be refreshed in the case of a metadata change.
    _actShowLandmarks->addRefreshEvent( Event::MESH_CHANGE | Event::LANDMARKS_CHANGE | Event::METADATA_CHANGE);
    _actVisLandmarkLabels = new ActionVisualise( "Show Landmark Labels", QIcon(":/icons/TAGS"), new LabelsVisualisation<LandmarkLabelsView>);
    _actVisLandmarkLabels->setToolTip( "Toggle the landmark labels on and off.");
    _actVisLandmarkLabels->addRefreshEvent( Event::LOADED_MODEL | Event::MASK_CHANGE | Event::LANDMARKS_CHANGE);
    _actEditLandmarks->setShowLandmarksAction( _actShowLandmarks);
    _actEditLandmarks->setMirrorLandmarksAction( _actMirrorLandmarks);
    //_actEditLandmarks->setRestoreLandmarksAction( _actRestoreLandmarks);
    _actEditLandmarks->setShowLandmarkLabelsAction( _actVisLandmarkLabels);

    _actOrientCameraToF = new ActionOrientCamera("View Front Profile", QIcon(":/icons/ORIENT_CAMERA"), Qt::Key_Space, 1.0f, 1, 0.0f);
    _actOrientCameraToF->addTriggerEvent( Event::VIEWER_CHANGE);
    _actOrientCameraToF->setToolTip( tr("Set the camera to look at the front profile."));

    _actOrientCameraToL = new ActionOrientCamera("View Right Profile", QIcon(":/icons/LOOK_RIGHT"), Qt::Key_Left, 1.0f, 1, -EIGEN_PI/2);
    _actOrientCameraToL->setToolTip( tr("Set the camera to look at the subject's right profile."));
    _actOrientCameraToLQ = new ActionOrientCamera("View Right Quarter", QIcon(":/icons/LOOK_RIGHT"), Qt::SHIFT + Qt::Key_Left, 1.0f, 1, -EIGEN_PI/4);
    _actOrientCameraToLQ->setToolTip( tr("Set the camera to look at the subject's front-right profile."));
    _actOrientCameraToR = new ActionOrientCamera("View Left Profile", QIcon(":/icons/LOOK_LEFT"), Qt::Key_Right, 1.0f, 1, EIGEN_PI/2);
    _actOrientCameraToR->setToolTip( tr("Set the camera to look at the subject's left profile."));
    _actOrientCameraToRQ = new ActionOrientCamera("View Left Quarter", QIcon(":/icons/LOOK_LEFT"), Qt::SHIFT + Qt::Key_Right, 1.0f, 1, EIGEN_PI/4);
    _actOrientCameraToRQ->setToolTip( tr("Set the camera to look at the subject's front-left profile."));
    _actOrientCameraToT = new ActionOrientCamera("View From Above", QIcon(), Qt::Key_Up, 1.0f, 0, -EIGEN_PI/2);
    _actOrientCameraToT->setToolTip( tr("Set the camera to look down at the subject from above."));
    _actOrientCameraToTQ = new ActionOrientCamera("View Top Quarter", QIcon(), Qt::SHIFT + Qt::Key_Up, 1.0f, 0, -EIGEN_PI/4);
    _actOrientCameraToTQ->setToolTip( tr("Set the camera to look at the subject from above and the front."));
    _actOrientCameraToB = new ActionOrientCamera("View From Below", QIcon(), Qt::Key_Down, 1.0f, 0, EIGEN_PI/2);
    _actOrientCameraToB->setToolTip( tr("Set the camera to look up at the subject from below."));
    _actOrientCameraToBQ = new ActionOrientCamera("View Bottom Quarter", QIcon(), Qt::SHIFT + Qt::Key_Down, 1.0f, 0, EIGEN_PI/4);
    _actOrientCameraToBQ->setToolTip( tr("Set the camera to look at the subject from below and the front."));

    _actRotateCameraR = new ActionRotateCamera( "Yaw Camera Right", Qt::CTRL + Qt::Key_Left, 0.0f, -5);
    _actRotateCameraL = new ActionRotateCamera( "Yaw Camera Left", Qt::CTRL + Qt::Key_Right, 0.0f, 5);
    _actRotateCameraU = new ActionRotateCamera( "Pitch Camera Down", Qt::CTRL + Qt::Key_Up, 5, 0.0f);
    _actRotateCameraD = new ActionRotateCamera( "Pitch Camera Up", Qt::CTRL + Qt::Key_Down, -5, 0.0f);

    _actCentreModel = new ActionCentreModel("Centre Model", QIcon(":/icons/CENTRE"), Qt::SHIFT + Qt::Key_O);
    _actAlignModel = new ActionAlignModel( "Align Model", QIcon(":/icons/TRANSFORM"), Qt::SHIFT + Qt::Key_Space);

    _actDiscardManifold = new ActionDiscardManifold( "Remove This Manifold", QIcon(":/icons/REM_FACE_1"));
    _actRemoveManifolds = new ActionRemoveManifolds( "Remove Other Manifolds", QIcon(":/icons/REM_FACE_N"));

    _actDetectFace = new ActionDetectFace( "Detect Landmarks", QIcon(":/icons/DETECT_FACE"));
    _actResetDetection = new ActionResetDetection( "Discard Detection", QIcon(":/icons/RESET_DETECT_FACE"));

    _actSetParallelProjection = new ActionSetParallelProjection( "Orthographic Projection", QIcon(":/icons/ORTHOGRAPHIC"), Qt::Key_E);

    _actSaveScreenshot = new ActionSaveScreenshot( "Take Screenshot (All Viewers)", QIcon(":/icons/SCREENSHOT"), Qt::Key_S);
    _actSmooth = new ActionSmooth( "Smooth Surface", QIcon(":/icons/SHAVE1"));
    _actReflectModel = new ActionReflectModel( "Reflect Model", QIcon(":/icons/REFLECT"), Qt::SHIFT + Qt::Key_R);

    _actToggleAxes = new ActionToggleAxes( "World Axes", QIcon(":/icons/AXES"), Qt::Key_A);

    _actToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Manually Reposition", QIcon(":/icons/MOVE_MODEL"), Qt::Key_Z);
    _actSynchroniseCameras = new ActionSynchroniseCameras( "Synchronise Cameras", QIcon(":/icons/SYNCH_CAMERAS"));
    _actMarquee = new ActionMarquee("Marquee Mode", QIcon(":/icons/PROJECTOR"));

    _actSetFocus = new ActionSetFocus( "Set Camera Focus", QIcon(":/icons/FOCUS"), Qt::Key_F);
    _aclkHandler->addLeftDoubleClickAction( _actSetFocus);

    _actRadialSelect = new ActionRadialSelect( "Select Radial Area", QIcon(":/icons/LASSO"));
    _actExtractFace = new ActionExtractFace( "Extract (Facial) Region", QIcon(":/icons/CUT_OUT"));

    _actMakeLeftFace = new ActionMakeHalfFace( "Create Right Symmetric Face", QIcon(":/icons/LEFT_FACE"));
    _actMakeLeftFace->setPlane( Vec3f(-1,0,0));

    _actMakeRightFace = new ActionMakeHalfFace( "Create Left Symmetric Face", QIcon(":/icons/RIGHT_FACE"));
    _actMakeRightFace->setPlane( Vec3f(1,0,0));

    _actFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"));
    _actSetOpacity = new ActionSetOpacity( "Surface Opacity");
    _actBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"), Qt::Key_B);

    _actEditPaths = new ActionEditPaths( "Show User Measurements", QIcon(":/icons/CALIPERS"), Qt::SHIFT + Qt::Key_C);
    _actAddPath = new ActionAddPath( "Add User Measurement", QIcon(":/icons/CALIPERS"), Qt::Key_C);
    _actCopyPath = new ActionCopyPath( "Copy User Measurement", QIcon(":/icons/COPY"));
    _actDeletePath = new ActionDeletePath( "Delete User Measurement", QIcon(":/icons/ERASER"), Qt::Key_Delete);
    _actRenamePath = new ActionRenamePath( "Rename User Measurement", QIcon(":/icons/EDIT"), Qt::CTRL + Qt::Key_C);
    _actCopyAllPaths = new ActionCopyAllPaths( "Copy ALL User Measurements", QIcon(":/icons/COPY"));
    _actDeleteAllPaths = new ActionDeleteAllPaths( "Delete ALL User Measurements", QIcon(":/icons/ERASER"));

    //_actSetSurfaceColour = new ActionSetSurfaceColour( "Base Colour", this);
    //_actSetMinScalarColour = new ActionSetMinScalarColour( "Minimum Colour");
    //_actSetMaxScalarColour = new ActionSetMaxScalarColour( "Maximum Colour");
#ifndef _WIN32
    // Lock these functions for now on Linux because Qt isn't behaving well with GTK
    //_actSetMinScalarColour->setLocked(true);
    //_actSetMaxScalarColour->setLocked(true);
#endif
    _actSetNumScalarColours = new ActionSetNumScalarColours( "Number of Colours for Colour Visualisations");
    _actChangeColourMappingRange = new ActionChangeColourMappingRange( "Min/Max Range for Colour Visualisations");

    _actToggleLegend = new ActionToggleLegend( "Scalar Legend");
    _actToggleStereo = new ActionToggleStereo( "3D (Red/Blue)", QIcon(":/icons/3D_GLASSES"));

    _actRotateX90 = new ActionRotateModel( "Rotate 90 degrees about X", QIcon(":/icons/ROTATE_X90"), Vec3f(1,0,0), 90);
    _actRotateY90 = new ActionRotateModel( "Rotate 90 degrees about Y", QIcon(":/icons/ROTATE_Y90"), Vec3f(0,1,0), 90);
    _actRotateZ90 = new ActionRotateModel( "Rotate 90 degrees about Z", QIcon(":/icons/ROTATE_Z90"), Vec3f(0,0,1), 90);
    _actResizeModel = new ActionResizeModel( "Resize Model", QIcon(":/icons/RESIZE_MODEL"));
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
    FAM::registerAction( actionMoveLC, this);
    FAM::registerAction( actionMoveCL, this);
    FAM::registerAction( actionMoveCR, this);
    FAM::registerAction( actionMoveRC, this);

    FAM::registerAction( actionCopyLC, this);
    FAM::registerAction( actionCopyCL, this);
    FAM::registerAction( actionCopyCR, this);
    FAM::registerAction( actionCopyRC, this);
}   // end _setupMainViewer


void ClinifaceMain::_createMetrics()
{
    _actShowScanInfo = new ActionShowScanInfo( "Assessment Information", QIcon(":/icons/IMAGE_DETAILS"), Qt::SHIFT + Qt::Key_I);
    _actShowMeshInfo = new ActionShowMeshInfo( "Model Properties", QIcon(":/icons/MODEL_PROPERTIES"), QString("`"));
    _actShowMetrics = new ActionShowMetrics( "Measurements Browser", QIcon(":/icons/METRICS1"), Qt::SHIFT + Qt::Key_M);
    _actShowMeshInfo->setDiscardManifoldAction( _actDiscardManifold);
    _actShowMeshInfo->setRemoveManifoldsAction( _actRemoveManifolds);
    _actShowPhenotypes = new ActionShowPhenotypes( "HPO Browser", QIcon(":/icons/FACEQ"), Qt::SHIFT + Qt::Key_H);
    ActionUpdateThumbnail* actUpdateThumbnail = new ActionUpdateThumbnail;

    FAM::registerAction( actUpdateThumbnail, this);
    FAM::registerAction( _actShowPhenotypes, this);
    FAM::registerAction( _actShowScanInfo, this);
    FAM::registerAction( _actShowMeshInfo, this);
    FAM::registerAction( _actShowMetrics, this);

    _actShowScanInfo->setThumbnailUpdater( actUpdateThumbnail);

    using namespace FaceTools::Widget;
    MetricsDialog *mdialog = static_cast<MetricsDialog*>( _actShowMetrics->widget());
    mdialog->setShowScanInfoAction( _actShowScanInfo->qaction());
    mdialog->setShowHPOsAction( _actShowPhenotypes->qaction());
    PhenotypesDialog *pdialog = static_cast<PhenotypesDialog*>( _actShowPhenotypes->widget());
    ScanInfoDialog *sdialog = static_cast<ScanInfoDialog*>( _actShowScanInfo->widget());

    // When HPOs are matched, update in HPOsDialog and ScaInfoDialog
    connect( mdialog, &MetricsDialog::onMatchHPOs,
            [=]( const IntSet &hids)
            {
                pdialog->showHPOs(hids);
                sdialog->refreshNotableHPOs();
            }); // 
    // When user selects HPO term from MeasurementsBrowser, set as selected row in HPOsDialog
    connect( mdialog, &MetricsDialog::onSelectHPO,
            [pdialog]( int hid){ pdialog->selectHPO( hid);});
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

    _setupMainViewer();
    _createHandlers();
    _createActions();
    _createMetrics();

    _updatesDialog = new UpdatesDialog(this);
    _prefsDialog = new PreferencesDialog(this);
    connect( _prefsDialog, &PreferencesDialog::onUpdated, [](){ FAM::raise( Event::MODEL_SELECT);});
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
    _ppoints.set("Model", _ui->menu_Geometry);
    _ppoints.set("Transform", _ui->menu_Transform);
    _ppoints.set("Help", _ui->menu_Help);
    _ppoints.set("Main", _ui->mainToolBar);
    _ppoints.set("Visualisations", _ui->visToolBar);
    _ppoints.set("Colour Mapping", _ui->scmapToolBar);

    _loadPlugins();

    // Refresh the contents here because plugins must dump
    // their help docs into the prescribed place first.
    _helpAss->refreshContents(":/data/HELP_TOC");

    // Update viewer selection model lists and window title whenever the selected model changes.
    connect( FAM::get(), &FAM::onUpdateSelected, _mfmv, &FaceTools::MultiFaceModelViewer::doOnUpdateModelLists);
    connect( FAM::get(), &FAM::onUpdateSelected, this, &ClinifaceMain::_doOnUpdateSelected);
    connect( FAM::get(), &FAM::onShowHelp, [this]( const QString& tok){ _helpAss->show(tok);});

    FAM::finalise();

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
QSize ClinifaceMain::sizeHint() const { return QSize( 860, 700);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname) { return _actLoad->load( fname);}


void ClinifaceMain::checkForUpdate() { _updatesDialog->checkForUpdate();}


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
    if ( FMM::numOpen() > 0 && !_actCloseAll->execute(Event::USER))
        evt->ignore();
}   // end closeEvent


void ClinifaceMain::_doOnUpdateSelected()
{
    QString wtitle = QString( "%1 %2").arg( appName(), APP_VERSION_STRING);

    const FaceTools::FM *fm = MS::selectedModel();
    if ( fm)
    {
        QString mfile = FMM::filepath(*fm);
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

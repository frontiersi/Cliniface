/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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
#include <HelpDialog.h>

#include <ActionRemesh.h>
#include <ActionSmooth.h>
#include <ActionReflect.h>
#include <ActionSetFocus.h>
#include <ActionVisualise.h>
#include <ActionFillHoles.h>
#include <ActionDetectFace.h>
#include <ActionCopyViewer.h>
#include <ActionMoveViewer.h>
#include <ActionRotateModel.h>
#include <ActionShowScanInfo.h>
#include <ActionGetComponent.h>
#include <ActionInvertNormals.h>
#include <ActionAlignLandmarks.h>
#include <ActionMapSurfaceData.h>
#include <ActionSaveFaceModels.h>
#include <ActionBackfaceCulling.h>
#include <ActionCloseFaceModels.h>
#include <ActionComponentSelect.h>
#include <ActionSaveAsFaceModel.h>
#include <ActionLoadDirFaceModels.h>
#include <ActionCloseAllFaceModels.h>
#include <ActionTransformToStandardPosition.h>

#include <ActionSetSurfaceColour.h>
#include <ActionSetMinScalarColour.h>
#include <ActionSetMaxScalarColour.h>
#include <ActionSetNumScalarColours.h>
#include <ActionChangeSurfaceMappingRange.h>

#include <FeaturesDetector.h>
#include <FaceShapeLandmarks2DDetector.h>

#include <GeneManager.h>
#include <HPOTermManager.h>
#include <SyndromeManager.h>
#include <LandmarksManager.h>
#include <MetricCalculatorManager.h>
#include <MetricCalculatorTypeRegistry.h>
#include <InterlandmarkMetricCalculatorType.h>

#include <FaceModelU3DFileHandler.h>
#include <FaceModelAssImpFileHandlerFactory.h>
#include <FaceModelOBJFileHandler.h>
#include <FaceModelXMLFileHandler.h>
#include <LandmarksVisualisation.h>
#include <BoundingVisualisation.h>
#include <OutlinesVisualisation.h>
#include <SampleReport.h>

#include <QLabel>
#include <QMimeData>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidgetAction>
using Cliniface::ClinifaceMain;


namespace {
QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, [d](){ d->show(); d->raise(); d->activateWindow();});
    return a;
}   // end connectDialog
}   // end namespace


void ClinifaceMain::registerActions()
{
    _fam->addAction( _actionLoadFaceModels);
    _fam->addAction( _actionLoadDirFaceModels);
    _fam->addAction( _actionSaveFaceModels);
    _fam->addAction( _actionSaveAsFaceModel);
    _fam->addAction( _actionExportPDF);
    _fam->addAction( _actionCloseFaceModels);
    _fam->addAction( _actionCloseAllFaceModels);

    _fam->addAction( _actionEditLandmarks);
    _fam->addAction( _actionEditPaths);
    _fam->addAction( _actionVisOutlines);
    _fam->addAction( _actionVisWireframe);
    _fam->addAction( _actionVisTexture);
    _fam->addAction( new ActionComponentSelect( _meei));

    _fam->addAction( new ActionMapSurfaceData);
    _fam->addAction( _actionOrientCameraToFrontFace);
    _fam->addAction( _actionOrientCameraToLeftFace);
    _fam->addAction( _actionOrientCameraToRightFace);
    _fam->addAction( _actionTransformToStandardPosition);
    _fam->addAction( _actionGetComponent);
    _fam->addAction( _actionDetectFace);
    _fam->addAction( _actionResetCamera);
    _fam->addAction( _actionSetParallelProjection);
    _fam->addAction( _actionSaveScreenshot);
    _fam->addAction( _actionSmooth);
    _fam->addAction( _actionReflect);
    _fam->addAction( _actionRemesh);
    _fam->addAction( _actionToggleAxes);
    _fam->addAction( _actionToggleCameraActorInteraction);
    _fam->addAction( _actionSynchroniseCameraMovement);
    _fam->addAction( _actionMarquee);
    _fam->addAction( _actionSetFocus);
    _fam->addAction( _actionRadialSelect);
    _fam->addAction( _actionCrop);
    _fam->addAction( _actionAlignLandmarks);
    _fam->addAction( _actionFillHoles);
    _fam->addAction( _actionSetOpacity);
    _fam->addAction( _actionBackfaceCulling);

    _fam->addAction( _actionRenamePath);
    _fam->addAction( _actionAddPath);
    _fam->addAction( _actionDeletePath);

    //_fam->addAction( _actionRenameLandmark);
    //_fam->addAction( _actionAddLandmark);
    //_fam->addAction( _actionDeleteLandmark);

    //_fam->addAction( _actionSetSurfaceColour);
    _fam->addAction( _actionSetMinScalarColour);
    _fam->addAction( _actionSetMaxScalarColour);
    _fam->addAction( _actionSetNumScalarColours);
    _fam->addAction( _actionChangeSurfaceMappingRange);
    _fam->addAction( _actionToggleScalarLegend);
    _fam->addAction( _actionRotateX90);
    _fam->addAction( _actionRotateY90);
    _fam->addAction( _actionRotateZ90);

    _fam->addAction( _actionInvertNormals);
    _fam->addAction( _actionToggleFXAA);
    _fam->addAction( _actionToggleStereoRendering);
    _fam->addAction( _actionShowScanInfo);
}   // end registerActions



void ClinifaceMain::createFileMenu()
{
    QAction *sep1 = new QAction;
    QAction *sep2 = new QAction;
    sep1->setSeparator(true);
    sep2->setSeparator(true);

    QList<QAction*> ioactions;
    ioactions << _actionLoadFaceModels->qaction()
              << _actionLoadDirFaceModels->qaction()
              << _actionSaveFaceModels->qaction()
              << _actionSaveAsFaceModel->qaction()
              << sep1
              << _actionShowScanInfo->qaction();

    if ( ActionExportPDF::isAvailable())
        ioactions << _actionExportPDF->qaction();

    ioactions << sep2
              <<_actionCloseFaceModels->qaction()
              << _actionCloseAllFaceModels->qaction();

    ui->menu_File->addActions( ioactions);
    ui->menu_File->addSeparator();
    ui->menu_File->addAction( ui->action_Exit);
}   // end createFileMenu


// private
void ClinifaceMain::createViewMenu()
{
    ui->menu_View->addAction( _actionVisTexture->qaction());
    ui->menu_View->addActions( _vorg->otherMenu());
    ui->menu_View->addAction( _actionVisWireframe->qaction());
    ui->menu_View->addAction( _actionVisOutlines->qaction());

    QMenu *surfaceMenu = ui->menu_View->addMenu( "&Scalar Mapping...");
    surfaceMenu->setIcon( QIcon(":/icons/COLOURS"));
    surfaceMenu->addActions( _vorg->surfaceMappingMenu());
    surfaceMenu->addSeparator();
    //surfaceMenu->addAction( _actionSetSurfaceColour->qaction());
    surfaceMenu->addAction( _actionSetMinScalarColour->qaction());
    surfaceMenu->addAction( _actionSetMaxScalarColour->qaction());
    surfaceMenu->addSeparator();
    surfaceMenu->addAction( _actionToggleScalarLegend->qaction());

    ui->menu_View->addSeparator();

    QMenu *cameraMenu = ui->menu_View->addMenu( "&Camera...");
    cameraMenu->setIcon( QIcon(":/icons/PROJECTOR"));
    cameraMenu->addAction( _actionSetParallelProjection->qaction());
    cameraMenu->addAction( _actionOrientCameraToLeftFace->qaction());
    cameraMenu->addAction( _actionOrientCameraToFrontFace->qaction());
    cameraMenu->addAction( _actionOrientCameraToRightFace->qaction());
    cameraMenu->addAction( _actionResetCamera->qaction());
    cameraMenu->addAction( _actionSynchroniseCameraMovement->qaction());
    cameraMenu->addAction( _actionMarquee->qaction());

    ui->menu_View->addAction( _actionBackfaceCulling->qaction());
    ui->menu_View->addAction( _actionToggleAxes->qaction());
    ui->menu_View->addAction( _actionSaveScreenshot->qaction());
    ui->menu_View->addAction( _actionToggleFXAA->qaction());
    ui->menu_View->addAction( _actionToggleStereoRendering->qaction());
    ui->menu_View->addAction( ui->action_VisualisationsToolbar);
    ui->menu_View->addAction( ui->action_ScalarMappingToolbar);

    /****** ui->visToolBar ******/
    ui->visToolBar->addAction( _actionVisTexture->qaction());
    ui->visToolBar->addActions( _vorg->otherToolbar());
    ui->visToolBar->addAction( _actionVisWireframe->qaction());
    ui->visToolBar->addAction( _actionVisOutlines->qaction());
    ui->visToolBar->addSeparator();
    ui->visToolBar->addAction( _actionEditLandmarks->qaction());
    ui->visToolBar->addAction( _actionShowMetrics->qaction());
    ui->visToolBar->addAction( _actionEditPaths->qaction());
    ui->visToolBar->addAction( _actionRadialSelect->qaction());
    ui->visToolBar->addSeparator();
    ui->visToolBar->addAction( _actionBackfaceCulling->qaction());
    ui->visToolBar->addWidget( _actionSetOpacity->getWidget());
    ui->visToolBar->addAction( _actionToggleAxes->qaction());

    /****** ui->scmapToolBar ******/
    ui->scmapToolBar->addActions( _vorg->surfaceMappingToolbar());
    ui->scmapToolBar->addSeparator();
    //ui->scmapToolBar->addAction( _actionSetSurfaceColour->qaction());
    ui->scmapToolBar->addAction( _actionSetMinScalarColour->qaction());
    ui->scmapToolBar->addAction( _actionSetMaxScalarColour->qaction());
    ui->scmapToolBar->addWidget( _actionSetNumScalarColours->getWidget());
    ui->scmapToolBar->addSeparator();
    ui->scmapToolBar->addWidget( _actionChangeSurfaceMappingRange->getWidget());
    QWidget* empty = new QWidget;
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->scmapToolBar->addWidget( empty);
}   // end createViewMenu


// private
void ClinifaceMain::createToolsMenu()
{
    ui->menu_Tools->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->menu_Tools->addAction( _actionRotateX90->qaction());
    ui->menu_Tools->addAction( _actionRotateY90->qaction());
    ui->menu_Tools->addAction( _actionRotateZ90->qaction());
    ui->menu_Tools->addAction( _actionReflect->qaction());
    ui->menu_Tools->addAction( _actionInvertNormals->qaction());
    ui->menu_Tools->addAction( _actionTransformToStandardPosition->qaction());
    ui->menu_Tools->addAction( _actionAlignLandmarks->qaction());

    ui->menu_Tools->addSeparator();

    ui->menu_Tools->addAction( _actionGetComponent->qaction());
    ui->menu_Tools->addAction( _actionFillHoles->qaction());
    ui->menu_Tools->addAction( _actionSmooth->qaction());
    ui->menu_Tools->addAction( _actionRemesh->qaction());
    ui->menu_Tools->addAction( _actionCrop->qaction());
}   // end createToolsMenu


// private
void ClinifaceMain::createMetricsMenu()
{
    ui->menu_Metrics->addAction( _actionDetectFace->qaction());
    ui->menu_Metrics->addAction( _actionEditLandmarks->qaction());
    ui->menu_Metrics->addAction( _actionShowMetrics->qaction());
    ui->menu_Metrics->addAction( _actionEditPaths->qaction());
    //ui->menu_Metrics->addAction( _actionRadialSelect->qaction());
    ui->menu_Metrics->addSeparator();
    ui->menu_Metrics->addAction( connectDialog( ui->action_MetricsInfo, _mdialog));
}   // end createMetricsMenu


// private
void ClinifaceMain::createHelpMenu()
{
    ui->menu_Help->addAction( connectDialog( ui->action_Help, new Cliniface::HelpDialog(this)));
    ui->menu_Help->addAction( connectDialog( ui->action_About, new Cliniface::AboutDialog(this)));
    ui->menu_Help->addAction( connectDialog( ui->action_Plugins, _ploader->dialog()));
}   // end createHelpMenu


void ClinifaceMain::createToolBar()
{
    ui->mainToolBar->addAction( _actionLoadFaceModels->qaction());
    ui->mainToolBar->addAction( _actionSaveFaceModels->qaction());
    ui->mainToolBar->addAction( _actionShowScanInfo->qaction());
    if ( ActionExportPDF::isAvailable())
        ui->mainToolBar->addAction( _actionExportPDF->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionDetectFace->qaction());
    ui->mainToolBar->addAction( _actionGetComponent->qaction());
    ui->mainToolBar->addAction( _actionFillHoles->qaction());
    ui->mainToolBar->addAction( _actionSmooth->qaction());
    ui->mainToolBar->addAction( _actionRemesh->qaction());
    ui->mainToolBar->addAction( _actionCrop->qaction());

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->mainToolBar->addAction( _actionRotateX90->qaction());
    ui->mainToolBar->addAction( _actionRotateY90->qaction());
    ui->mainToolBar->addAction( _actionRotateZ90->qaction());
    ui->mainToolBar->addAction( _actionReflect->qaction());
    ui->mainToolBar->addAction( _actionInvertNormals->qaction());
    ui->mainToolBar->addAction( _actionTransformToStandardPosition->qaction());
    ui->mainToolBar->addAction( _actionAlignLandmarks->qaction());

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction( _actionSetParallelProjection->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToLeftFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToFrontFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToRightFace->qaction());
    ui->mainToolBar->addAction( _actionResetCamera->qaction());
    ui->mainToolBar->addAction( _actionSynchroniseCameraMovement->qaction());
    ui->mainToolBar->addAction( _actionMarquee->qaction());

    // Append a space and then the logo to the toolbar.
    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(emptySpacer);
    QLabel* label = new QLabel;
    label->setText( QString("<a href=\"") + APP_WEBSITE + QString("\"><img src=\":/logos/TOOLBAR_LOGO\"/></a>"));
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);
    ui->mainToolBar->addWidget(label);
}   // end createToolBar


void ClinifaceMain::createContextMenu()
{
    _cmenu = new FaceTools::Interactor::ContextMenuInteractor;
    _cmenu->setViewer( _mfmv->centreViewer());

    _cmenu->addAction( _actionSetFocus);
    _cmenu->addAction( _actionRadialSelect);
    _cmenu->addSeparator();
    //_cmenu->addAction( _actionAddLandmark);
    //_cmenu->addAction( _actionRenameLandmark);
    //_cmenu->addAction( _actionDeleteLandmark);
    _cmenu->addAction( _actionAddPath);
    _cmenu->addAction( _actionRenamePath);
    _cmenu->addAction( _actionDeletePath);
}   // end createContextMenu



void ClinifaceMain::initFileIO()
{
    FaceTools::FileIO::FMM::setLoadLimit( MODEL_LOAD_MAX);

    QString pdfLatexFilePath = PDF_LATEX;
#ifdef _WIN32
    pdfLatexFilePath = QDir( QApplication::applicationDirPath()).filePath(PDF_LATEX);
#endif
    const QString idtfConverterFilePath = QDir( QApplication::applicationDirPath()).filePath(IDTF_CONVERTER);
    ActionExportPDF::init( pdfLatexFilePath.toStdString(), idtfConverterFilePath.toStdString());

    using namespace FaceTools::FileIO;
    FMM::add( new FaceModelXMLFileHandler);    // Default (preferred)
    FMM::add( new FaceModelOBJFileHandler);
    FMM::add( new FaceModelU3DFileHandler);
    //FaceModelAssImpFileHandlerFactory::printAvailableFormats( std::cerr);
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ask"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ase"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("fbx"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("dxf"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3s"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("q3o"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("acc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifc"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ifczip"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("raw"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("sib"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("nff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("enff"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("xgl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("zgl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ter"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ply"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("mot"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lws"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lwo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("lxo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("stl"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("blend"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ms3d"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("ndo"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("off"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("vta"));
    FMM::add( FaceModelAssImpFileHandlerFactory::make("smd"));
}   // end initFileIO


void ClinifaceMain::createActions()
{
    using namespace FaceTools::Report;
    using namespace FaceTools::Vis;

    _actionLoadFaceModels = new ActionLoadFaceModels( "&Open", QIcon(":/icons/LOAD"), this);
    _actionLoadDirFaceModels = new ActionLoadDirFaceModels( "Open &Dir", QIcon(":/icons/LOAD_DIR"), this);
    _actionSaveFaceModels = new ActionSaveFaceModels( "&Save", QIcon(":/icons/SAVE"), QKeySequence(Qt::CTRL + Qt::Key_S), this);
    _actionSaveAsFaceModel = new ActionSaveAsFaceModel( "Save &As", QIcon(":/icons/SAVE_AS"), this);
    _actionExportPDF = new ActionExportPDF( new SampleReport("Export to PDF"), QIcon(":/icons/PDF"), APP_CONTACT_EMAIL, this, ui->progressBar);
    _actionExportPDF->setLogoResource(":/logos/PDF_LOGO");
    _actionCloseFaceModels = new ActionCloseFaceModels( "&Close", QIcon(":/icons/CLOSE"), this);
    _actionCloseAllFaceModels = new ActionCloseAllFaceModels( "Close All", this);

    _actionVisTexture = new ActionVisualise( new TextureVisualisation, true /*show on load*/);
    _actionVisWireframe = new ActionVisualise( new WireframeVisualisation("Wireframe", QIcon(":/icons/WIREFRAME"), QKeySequence(Qt::Key_W)));
    _actionVisOutlines = new ActionVisualise( new OutlinesVisualisation("Outlines", QIcon(":/icons/OUTLINES"), QKeySequence(Qt::Key_O)));
    _actionEditLandmarks = new ActionEditLandmarks( "Landmarks", QIcon(":/icons/MARKER"), _meei, true/*vis on load*/);

    _actionOrientCameraToFrontFace = new ActionOrientCameraToFace("Orient Camera to Front Face", QIcon(":/icons/ORIENT_CAMERA"));
    _actionOrientCameraToFrontFace->setRespondToEvent( LOADED_MODEL);
    _actionOrientCameraToLeftFace = new ActionOrientCameraToFace("Orient Camera to Left Face", QIcon(":/icons/LOOK_RIGHT"), 450.0f, static_cast<float>(-CV_PI/2));
    _actionOrientCameraToRightFace = new ActionOrientCameraToFace("Orient Camera to Right Face", QIcon(":/icons/LOOK_LEFT"), 450.0f, static_cast<float>(CV_PI/2));

    _actionTransformToStandardPosition = new ActionTransformToStandardPosition("Transform to Standard Position", QIcon(":/icons/TRANSFORM"));

    _actionGetComponent = new ActionGetComponent( "Remove Non-Face Components", QIcon(":/icons/FACE"), ui->progressBar);
    _actionDetectFace = new ActionDetectFace( "Detect Face", QIcon(":/icons/DETECT_FACE"), this, ui->progressBar);

    _actionResetCamera = new ActionResetCamera( "Reset All Cameras", QIcon(":/icons/RESET_CAMERA"));
    _actionResetCamera->setRespondToEvent( LOADED_MODEL);
    _actionResetCamera->addViewer( _mfmv->leftViewer());
    _actionResetCamera->addViewer( _mfmv->centreViewer());
    _actionResetCamera->addViewer( _mfmv->rightViewer());

    _actionSetParallelProjection = new ActionSetParallelProjection( "Orthographic Projection", QIcon(":/icons/ORTHOGRAPHIC"));
    _actionSetParallelProjection->addViewer( _mfmv->leftViewer());
    _actionSetParallelProjection->addViewer( _mfmv->centreViewer());
    _actionSetParallelProjection->addViewer( _mfmv->rightViewer());

    _actionSaveScreenshot = new ActionSaveScreenshot( "Save Screenshot (All Viewers)", QIcon(":/icons/SCREENSHOT"));
    _actionSaveScreenshot->addViewer( _mfmv->leftViewer());
    _actionSaveScreenshot->addViewer( _mfmv->centreViewer());
    _actionSaveScreenshot->addViewer( _mfmv->rightViewer());

    _actionSmooth = new ActionSmooth( "Smooth Model", QIcon(":/icons/SHAVE"), ui->progressBar);
    _actionRemesh = new ActionRemesh( "Remesh Model", QIcon(":/icons/REMESH"), ui->progressBar);
    _actionReflect = new ActionReflect( "Mirror Reflect", QIcon(":/icons/REFLECT"), ui->progressBar);

    _actionToggleAxes = new ActionToggleAxes( "World Axes", QIcon(":/icons/AXES"));
    _actionToggleAxes->addViewer(_mfmv->leftViewer());
    _actionToggleAxes->addViewer(_mfmv->centreViewer());
    _actionToggleAxes->addViewer(_mfmv->rightViewer());

    _actionToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Toggle Model / Camera Interaction", QIcon(":/icons/MOVE_MODEL"));
    _actionSynchroniseCameraMovement = new ActionSynchroniseCameraMovement( "Synchronise Cameras", QIcon(":/icons/SYNCH_CAMERAS"),
                                                                             _actionToggleCameraActorInteraction->interactor());
    _actionSynchroniseCameraMovement->addViewer(_mfmv->leftViewer());
    _actionSynchroniseCameraMovement->addViewer(_mfmv->centreViewer());
    _actionSynchroniseCameraMovement->addViewer(_mfmv->rightViewer());

    _actionMarquee = new ActionMarquee("Auto-Rotate Views", QIcon(":/icons/MOVIE"), _actionToggleCameraActorInteraction->interactor());
    _actionMarquee->addViewer(_mfmv->leftViewer());
    _actionMarquee->addViewer(_mfmv->centreViewer());
    _actionMarquee->addViewer(_mfmv->rightViewer());

    _actionSetFocus = new ActionSetFocus( "Set Focus");
    _actionRadialSelect = new ActionRadialSelect( "Select Radial Area", QIcon(":/icons/LASSO"), _meei, ui->statusbar);
    _actionCrop = new ActionCrop( "Crop Model", QIcon(":/icons/SCALPEL"), _actionRadialSelect, ui->progressBar);
    _actionAlignLandmarks = new ActionAlignLandmarks( "Align Same Landmarks (ICP)", QIcon(":/icons/ALIGN"), ui->progressBar);
    _actionFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"), ui->progressBar);
    _actionSetOpacity = new ActionSetOpacity( "Model Opacity", 0.5, 0.1/*min opacity*/, this);
    _actionBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"));

    _actionEditPaths = new ActionEditPaths( "Paths", QIcon(":/icons/CALIPERS"), _meei, ui->statusbar);
    _actionAddPath = new ActionAddPath( "Add Path", QIcon(":/icons/CALIPERS"), _actionEditPaths);
    _actionRenamePath = new ActionRenamePath( "Rename Path", QIcon(":/icons/EDIT"), _actionEditPaths, this);
    _actionDeletePath = new ActionDeletePath( "Delete Path", QIcon(":/icons/ERASER"), _actionEditPaths);

    //_actionRenameLandmark = new ActionRenameLandmark( "Rename Landmark", QIcon(":/icons/EDIT"), _actionEditLandmarks, this);
    //_actionAddLandmark = new ActionAddLandmark( "Add Landmark", QIcon(":/icons/MARKER"), _actionEditLandmarks, this);
    //_actionDeleteLandmark = new ActionDeleteLandmark( "Delete Landmark", QIcon(":/icons/ERASER"), _actionEditLandmarks);

    //_actionSetSurfaceColour = new ActionSetSurfaceColour( "Set Base Surface Colour", this);
    _actionSetMinScalarColour = new ActionSetMinScalarColour( "Set Minimum Scalar Mapping Colour", this);
    _actionSetMaxScalarColour = new ActionSetMaxScalarColour( "Set Maximum Scalar Mapping Colour", this);
    _actionSetNumScalarColours = new ActionSetNumScalarColours( "Change Number of Scalar Mapping Colours", this);
    _actionChangeSurfaceMappingRange = new ActionChangeSurfaceMappingRange( "Change Min/Max Surface Mapping Range", this);

    _actionToggleScalarLegend = new ActionToggleScalarLegend;
    _actionToggleScalarLegend->addViewer( _mfmv->leftViewer());
    _actionToggleScalarLegend->addViewer( _mfmv->centreViewer());
    _actionToggleScalarLegend->addViewer( _mfmv->rightViewer());

    _actionToggleFXAA = new ActionToggleFXAA( "Use Antialiasing");
    _actionToggleFXAA->addViewer( _mfmv->leftViewer());
    _actionToggleFXAA->addViewer( _mfmv->centreViewer());
    _actionToggleFXAA->addViewer( _mfmv->rightViewer());

    _actionToggleStereoRendering = new ActionToggleStereoRendering( "3D Stereo Rendering", QIcon(":/icons/3D_GLASSES"));
    _actionToggleStereoRendering->addViewer( _mfmv->leftViewer());
    _actionToggleStereoRendering->addViewer( _mfmv->centreViewer());
    _actionToggleStereoRendering->addViewer( _mfmv->rightViewer());

    _actionRotateX90 = new ActionRotateModel( "Rotate Model in X Axis 90 degrees", QIcon(":/icons/ROTATE_X90"), cv::Vec3f(1,0,0), 90);
    _actionRotateY90 = new ActionRotateModel( "Rotate Model in Y Axis 90 degrees", QIcon(":/icons/ROTATE_Y90"), cv::Vec3f(0,1,0), 90);
    _actionRotateZ90 = new ActionRotateModel( "Rotate Model in Z Axis 90 degrees", QIcon(":/icons/ROTATE_Z90"), cv::Vec3f(0,0,1), 90);

    _actionInvertNormals = new ActionInvertNormals( "Invert Polygon Normals", QIcon(":/icons/NORMAL_FLIP"), ui->progressBar);

    _actionShowScanInfo = new ActionShowScanInfo( "Model Info", QIcon(":/icons/MODEL_INFO"), this);
}   // end createActions



void ClinifaceMain::setupMainViewer()
{
    using namespace FaceTools;

    FMV* lv = _mfmv->leftViewer();
    FMV* cv = _mfmv->centreViewer();
    FMV* rv = _mfmv->rightViewer();

    FaceAction* actionSaveL = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), lv);
    FaceAction* actionSaveC = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), cv);
    FaceAction* actionSaveR = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), rv);
    _mfmv->setLeftAction0(   actionSaveL->qaction());
    _mfmv->setCentreAction0( actionSaveC->qaction());
    _mfmv->setRightAction0(  actionSaveR->qaction());

    FaceAction* actionResetL = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), lv);
    FaceAction* actionResetC = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), cv);
    FaceAction* actionResetR = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), rv);
    _mfmv->setLeftAction1(   actionResetL->qaction());
    _mfmv->setCentreAction1( actionResetC->qaction());
    _mfmv->setRightAction1(  actionResetR->qaction());

    // Set move actions
    FaceAction* actionMoveLC = new ActionMoveViewer( cv, lv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveCL = new ActionMoveViewer( lv, cv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    FaceAction* actionMoveCR = new ActionMoveViewer( rv, cv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveRC = new ActionMoveViewer( cv, rv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    _mfmv->setMoveLeftToCentreAction(  actionMoveLC->qaction());
    _mfmv->setMoveCentreToLeftAction(  actionMoveCL->qaction());
    _mfmv->setMoveCentreToRightAction( actionMoveCR->qaction());
    _mfmv->setMoveRightToCentreAction( actionMoveRC->qaction());

    // Set copy actions
    FaceAction* actionCopyLC = new ActionCopyViewer( cv, lv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyCL = new ActionCopyViewer( lv, cv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
    FaceAction* actionCopyCR = new ActionCopyViewer( rv, cv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyRC = new ActionCopyViewer( cv, rv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
    _mfmv->setCopyLeftToCentreAction(  actionCopyLC->qaction());
    _mfmv->setCopyCentreToLeftAction(  actionCopyCL->qaction());
    _mfmv->setCopyCentreToRightAction( actionCopyCR->qaction());
    _mfmv->setCopyRightToCentreAction( actionCopyRC->qaction());

    // Register the actions
    _fam->addAction( actionResetL);
    _fam->addAction( actionResetC);
    _fam->addAction( actionResetR);

    _fam->addAction( actionSaveL);
    _fam->addAction( actionSaveC);
    _fam->addAction( actionSaveR);

    _fam->addAction( actionMoveLC);
    _fam->addAction( actionMoveCL);
    _fam->addAction( actionMoveCR);
    _fam->addAction( actionMoveRC);

    _fam->addAction( actionCopyLC);
    _fam->addAction( actionCopyCL);
    _fam->addAction( actionCopyCR);
    _fam->addAction( actionCopyRC);
}   // end setupMainViewer


void ClinifaceMain::createMetrics()
{
    using namespace FaceTools::Metric;
    MetricCalculatorTypeRegistry::addTemplateType( new InterlandmarkMetricCalculatorType);   // TODO make plugin

    FaceTools::Landmark::LandmarksManager::load( QDir( QApplication::applicationDirPath()).filePath( LANDMARKS_FILE));

    MetricCalculatorManager::load( QDir( QApplication::applicationDirPath()).filePath( METRICS_DIR));
    HPOTermManager::load( QDir( QApplication::applicationDirPath()).filePath( HPOS_FILE));
    GeneManager::load( QDir( QApplication::applicationDirPath()).filePath( GENES_FILE));
    SyndromeManager::load( QDir( QApplication::applicationDirPath()).filePath( SYNDROMES_FILE));

    // Ensure these actions created only after metrics loaded.
    _actionUpdateMetrics = new ActionUpdateMetrics;
    _actionShowMetrics = new ActionShowMetrics( "Metrics", QIcon(":/icons/METRICS"), _meei);
    _actionShowChart = new ActionShowChart( "Metric Chart", QIcon(":/icons/CHART"), this);

    _fam->addAction( _actionUpdateMetrics);
    _fam->addAction( _actionShowMetrics);
    _fam->addAction( _actionShowChart);
}   // end createMetrics


void ClinifaceMain::createDialogs()
{
    using namespace FaceTools::Widget;
    _mdialog = new MetricsDisplayDialog( this);
    _mdialog->setWindowTitle( APP_NAME + QString(" | Metrics"));
    _mdialog->setShowChartAction( _actionShowChart->qaction());
    _mdialog->setShowMetricsAction( _actionShowMetrics->qaction());
    _mdialog->setShowScanInfoAction( _actionShowScanInfo->qaction());
    connect( _mdialog, &MetricsDisplayDialog::onChangeOpacity, _actionSetOpacity, &ActionSetOpacity::setOpacityOnOverlap);
    connect( _actionShowMetrics, &ActionShowMetrics::onEnterMetric, _mdialog, &MetricsDisplayDialog::setMetricSelected);
    connect( _mdialog, &MetricsDisplayDialog::onSelectMetric, _actionShowChart, &ActionShowChart::setMetric);
    _mdialog->populate();
}   // end createDialogs


// public
ClinifaceMain::ClinifaceMain()
    : QMainWindow(nullptr), ui(new Ui::ClinifaceMain)
{
    ui->setupUi(this);
    setWindowTitle( APP_NAME);
    setAcceptDrops(true);   // Accept dropping of files onto this widget
    setContextMenuPolicy(Qt::NoContextMenu);

    FaceTools::registerTypes();

    std::string haarModels = QDir( QApplication::applicationDirPath()).filePath( HAAR_CASCADES_MODELS).toStdString();
    if ( !FaceTools::Detect::FeaturesDetector::initialise( haarModels))
        std::cerr << "[WARNING] Unable to initialise face detector (" << haarModels << ")" << std::endl;
    std::string faceShapeLandmarks = QDir( QApplication::applicationDirPath()).filePath( FACE_SHAPE_LANDMARKS).toStdString();
    if ( !FaceTools::Detect::FaceShapeLandmarks2DDetector::initialise( faceShapeLandmarks))
        std::cerr << "[WARNING] Unable to initialise landmarks detector (" << faceShapeLandmarks << ")" << std::endl;

    _mfmv = new FaceTools::MultiFaceModelViewer( this);
    _fam = new FaceActionManager( _mfmv->centreViewer());
    _vorg = new Cliniface::VisualisationsOrganiser(_fam);

    _fam->addViewer( _mfmv->leftViewer());
    _fam->addViewer( _mfmv->rightViewer());

    // Needed for some actions that check if models are under the mouse cursor.
    _meei = new FaceTools::Interactor::ModelEntryExitInteractor;
    _meei->setViewer(_mfmv->centreViewer());
    setupMainViewer();

    initFileIO();
    createActions();
    createMetrics();
    createDialogs();

    QWidget* cwidget = new QWidget;
    cwidget->setLayout( new QVBoxLayout);
    cwidget->layout()->setContentsMargins(0,0,0,0);
    cwidget->layout()->addWidget( _mfmv);
    cwidget->layout()->addWidget( ui->progressBar);
    setCentralWidget( cwidget);
    _mfmv->layout()->setContentsMargins(1,1,1,1);

    _ploader = new Cliniface::ClinifacePluginsLoader( _fam, this);
    _ploader->dialog()->setWindowTitle( APP_NAME + QString(" | Plugins"));
    _ploader->loadPlugins( QApplication::applicationDirPath() + "/plugins");

    createToolBar();
    createFileMenu();
    createViewMenu();
    createMetricsMenu();
    createToolsMenu();
    createHelpMenu();
    createContextMenu();

    registerActions();

    // Locate centrally on desktop
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), qApp->desktop()->availableGeometry()));

    // Update window title whenever an action finishes.
    connect( _fam, &FaceActionManager::onUpdateSelected, this, &ClinifaceMain::doOnUpdateSelected);
    doOnUpdateSelected(nullptr);
}   // end ctor


// public
ClinifaceMain::~ClinifaceMain()
{
    delete _cmenu;
    delete _meei;
    delete _vorg;
    delete _fam;
    delete _mdialog;
    delete _ploader;
    delete ui;
}   // end dtor


// protected virtual
QSize ClinifaceMain::sizeHint() const { return QSize( 1000, 800);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname)
{
    return _actionLoadFaceModels->loadModel( fname) != nullptr;
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
    if ( _actionCloseAllFaceModels->isEnabled() && !_actionCloseAllFaceModels->process())
        evt->ignore();
}   // end closeEvent


// private slot
void ClinifaceMain::doOnUpdateSelected( FaceTools::FM* fm)
{
    QString wtitle = APP_NAME;
    QString scanInfoTitle = APP_NAME + QString(" | Scan Info");

    if ( fm)
    {
        QString mfile = tr(FaceTools::FileIO::FMM::filepath(fm).c_str());
        if ( !fm->isSaved())
            mfile += " (*)";
        wtitle += " | " + mfile;
        scanInfoTitle += " | " + mfile;
    }   // end if

    setWindowTitle( wtitle);
    _actionShowScanInfo->dialog()->setWindowTitle( scanInfoTitle);
}   // end doOnUpdateSelected

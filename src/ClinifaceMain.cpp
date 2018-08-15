/************************************************************************
 * Copyright (C) 2017 Richard Palmer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
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
#include <LicensesDialog.h>

#include <ActionRemesh.h>
#include <ActionSmooth.h>
#include <ActionSetFocus.h>
#include <ActionVisualise.h>
#include <ActionFillHoles.h>
#include <ActionDetectFace.h>
#include <ActionCopyViewer.h>
#include <ActionMoveViewer.h>
#include <ActionSetOpacity.h>
#include <ActionGetComponent.h>
#include <ActionAlignLandmarks.h>
#include <ActionMapSurfaceData.h>
#include <ActionSaveFaceModels.h>
#include <ActionBackfaceCulling.h>
#include <ActionCloseFaceModels.h>
#include <ActionSaveAsFaceModel.h>
#include <ActionLoadDirFaceModels.h>
#include <ActionCloseAllFaceModels.h>
#include <ActionTransformToStandardPosition.h>

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
using Cliniface::ClinifaceMain;


namespace {
QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, d, &QDialog::show);
    QObject::connect( a, &QAction::triggered, d, &QDialog::raise);
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
    _fam->addAction( new FaceTools::Action::ActionVisualise( new FaceTools::Vis::BoundingVisualisation));

    _fam->addAction( new FaceTools::Action::ActionMapSurfaceData);
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
    _fam->addAction( _actionRenameLandmark);
    _fam->addAction( _actionAddLandmark);
    _fam->addAction( _actionDeleteLandmark);
}   // end registerActions



void ClinifaceMain::createFileMenu()
{
    QList<QAction*> ioactions;
    ioactions << _actionLoadFaceModels->qaction()
              << _actionLoadDirFaceModels->qaction()
              << _actionSaveFaceModels->qaction()
              << _actionSaveAsFaceModel->qaction();

    if ( FaceTools::Action::ActionExportPDF::isAvailable())
        ioactions << _actionExportPDF->qaction();

    ioactions << _actionCloseFaceModels->qaction()
              << _actionCloseAllFaceModels->qaction();

    ui->menu_File->addActions( ioactions);
    ui->menu_File->addSeparator();
    ui->menu_File->addAction( ui->action_Exit);
}   // end createFileMenu


// private
void ClinifaceMain::createViewMenu()
{
    /****** ui->menu_View ******/
    ui->menu_View->addAction( _actionOrientCameraToLeftFace->qaction());
    ui->menu_View->addAction( _actionOrientCameraToFrontFace->qaction());
    ui->menu_View->addAction( _actionOrientCameraToRightFace->qaction());
    ui->menu_View->addAction( _actionResetCamera->qaction());
    ui->menu_View->addAction( _actionSynchroniseCameraMovement->qaction());
    ui->menu_View->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->menu_View->addSeparator();
    ui->menu_View->addActions( _fam->visualisations().actions());
    ui->menu_View->addSeparator();
    ui->menu_View->addAction( _actionVisOutlines->qaction());
    ui->menu_View->addAction( _actionRadialSelect->qaction());
    ui->menu_View->addAction( _actionEditLandmarks->qaction());
    ui->menu_View->addAction( _actionEditPaths->qaction());
    ui->menu_View->addSeparator();
    ui->menu_View->addAction( _actionMarquee->qaction());
    ui->menu_View->addAction( _actionBackfaceCulling->qaction());
    ui->menu_View->addAction( _actionSetParallelProjection->qaction());
    ui->menu_View->addAction( _actionToggleAxes->qaction());
    ui->menu_View->addAction( ui->action_ShowVisToolbar);
    ui->menu_View->addAction( _actionSaveScreenshot->qaction());

    /****** ui->visToolBar ******/
    ui->visToolBar->addActions( _fam->visualisations().actions());
    ui->visToolBar->addSeparator();
    ui->visToolBar->addAction( _actionVisOutlines->qaction());
    ui->visToolBar->addAction( _actionRadialSelect->qaction());
    ui->visToolBar->addAction( _actionEditLandmarks->qaction());
    ui->visToolBar->addAction( _actionEditPaths->qaction());
    ui->visToolBar->addSeparator();
    ui->visToolBar->addAction( _actionBackfaceCulling->qaction());
    ui->visToolBar->addWidget( _actionSetOpacity->getWidget());
    ui->visToolBar->addAction( _actionSetParallelProjection->qaction());
    ui->visToolBar->addAction( _actionToggleAxes->qaction());
}   // end createViewMenu


// private
void ClinifaceMain::createToolsMenu()
{
    ui->menu_Tools->addAction( _actionDetectFace->qaction());
    ui->menu_Tools->addAction( _actionGetComponent->qaction());
    ui->menu_Tools->addAction( _actionFillHoles->qaction());
    ui->menu_Tools->addAction( _actionSmooth->qaction());
    ui->menu_Tools->addAction( _actionRemesh->qaction());
    ui->menu_Tools->addAction( _actionCrop->qaction());
    ui->menu_Tools->addSeparator();
    ui->menu_Tools->addAction( _actionTransformToStandardPosition->qaction());
    ui->menu_Tools->addAction( _actionAlignLandmarks->qaction());
}   // end createToolsMenu


// private
void ClinifaceMain::createHelpMenu()
{
    ui->menu_Help->addAction( connectDialog( ui->action_Help, new Cliniface::HelpDialog(this)));
    ui->menu_Help->addAction( connectDialog( ui->action_About, new Cliniface::AboutDialog(this)));
    //ui->menu_Help->addAction( connectDialog( ui->action_Licensing, new Cliniface::LicensesDialog(this)));
    _fam->dialog()->setWindowTitle( APP_NAME + QString(" | Plugins"));
    ui->menu_Help->addAction( connectDialog( ui->action_Plugins, _fam->dialog()));
}   // end createHelpMenu


void ClinifaceMain::createToolBar()
{
    ui->mainToolBar->addAction( _actionLoadFaceModels->qaction());
    ui->mainToolBar->addAction( _actionSaveFaceModels->qaction());
    if ( FaceTools::Action::ActionExportPDF::isAvailable())
        ui->mainToolBar->addAction( _actionExportPDF->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionDetectFace->qaction());
    ui->mainToolBar->addAction( _actionGetComponent->qaction());
    ui->mainToolBar->addAction( _actionFillHoles->qaction());
    ui->mainToolBar->addAction( _actionSmooth->qaction());
    ui->mainToolBar->addAction( _actionRemesh->qaction());
    ui->mainToolBar->addAction( _actionCrop->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionTransformToStandardPosition->qaction());
    ui->mainToolBar->addAction( _actionAlignLandmarks->qaction());
    ui->mainToolBar->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionSynchroniseCameraMovement->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToLeftFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToFrontFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToRightFace->qaction());
    ui->mainToolBar->addAction( _actionResetCamera->qaction());
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
    _cmenu->addAction( _actionAddLandmark);
    _cmenu->addAction( _actionRenameLandmark);
    _cmenu->addAction( _actionDeleteLandmark);
    _cmenu->addSeparator();
    _cmenu->addAction( _actionAddPath);
    _cmenu->addAction( _actionRenamePath);
    _cmenu->addAction( _actionDeletePath);
}   // end createContextMenu



void ClinifaceMain::initFileIO()
{
    QString pdfLatexFilePath = PDF_LATEX;
#ifdef _WIN32
    pdfLatexFilePath = QDir( QApplication::applicationDirPath()).filePath(PDF_LATEX);
#endif
    const QString idtfConverterFilePath = QDir( QApplication::applicationDirPath()).filePath(IDTF_CONVERTER);
    FaceTools::Action::ActionExportPDF::init( pdfLatexFilePath.toStdString(), idtfConverterFilePath.toStdString());

    using namespace FaceTools::FileIO;
    FaceModelManager* fmm = _fam->modelManager();
    fmm->add( new FaceModelXMLFileHandler);    // Default (preferred)
    fmm->add( new FaceModelOBJFileHandler);
    fmm->add( new FaceModelU3DFileHandler);
    //FaceModelAssImpFileHandlerFactory::printAvailableFormats( std::cerr);
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ask"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ase"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("fbx"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("dxf"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("q3s"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("q3o"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ac"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("acc"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ifc"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ifczip"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("raw"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("sib"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("nff"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("enff"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("xgl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("zgl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ter"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ply"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("mot"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lws"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lwo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lxo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("stl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("blend"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ms3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ndo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("off"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("vta"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("smd"));
}   // end initFileIO


void ClinifaceMain::createActions()
{
    FaceTools::FileIO::FaceModelManager *fmm = _fam->modelManager();
    using namespace FaceTools::Action;
    using namespace FaceTools::Report;
    using namespace FaceTools::Vis;

    _actionLoadFaceModels = new ActionLoadFaceModels( "&Open", QIcon(":/icons/LOAD"), QKeySequence(Qt::CTRL + Qt::Key_O), fmm->loader());
    _actionLoadDirFaceModels = new ActionLoadDirFaceModels( "Open &Dir", QIcon(":/icons/LOAD_DIR"), fmm->loader());
    _actionSaveFaceModels = new ActionSaveFaceModels( "&Save", QIcon(":/icons/SAVE"), QKeySequence(Qt::CTRL + Qt::Key_S), fmm, this);
    _actionSaveAsFaceModel = new ActionSaveAsFaceModel( "Save &As", QIcon(":/icons/SAVE_AS"), fmm, this);
    _actionExportPDF = new ActionExportPDF( new SampleReport("Create Report (Sample)"), QIcon(":/icons/PDF"), this, ui->progressBar);
    _actionExportPDF->setLogoResource(":/logos/PDF_LOGO");
    _actionCloseFaceModels = new ActionCloseFaceModels( "&Close", QIcon(":/icons/CLOSE"), QKeySequence(Qt::CTRL + Qt::Key_W), fmm, this);
    _actionCloseAllFaceModels = new ActionCloseAllFaceModels( "Close All", fmm, this);

    _actionVisOutlines = new ActionVisualise( new OutlinesVisualisation("Outlines", QIcon(":/icons/OUTLINES"), QKeySequence(Qt::Key_O)));
    _actionEditLandmarks = new ActionEditLandmarks( "Edit Landmarks", QIcon(":/icons/MARKER"), _feei, ui->statusbar);
    _actionEditLandmarks->addProcessOn( ChangeEvent( LOADED_MODEL, true));

    _actionOrientCameraToFrontFace = new ActionOrientCameraToFace("Orient Camera to Front Face", QIcon(":/icons/ORIENT_CAMERA"));
    _actionOrientCameraToFrontFace->addProcessOn( LOADED_MODEL);

    _actionOrientCameraToLeftFace = new ActionOrientCameraToFace("Orient Camera to Left Face", QIcon(":/icons/LOOK_RIGHT"));
    _actionOrientCameraToLeftFace->setAngleAboutUpVector( -CV_PI/2);

    _actionOrientCameraToRightFace = new ActionOrientCameraToFace("Orient Camera to Right Face", QIcon(":/icons/LOOK_LEFT"));
    _actionOrientCameraToRightFace->setAngleAboutUpVector( CV_PI/2);

    _actionTransformToStandardPosition = new ActionTransformToStandardPosition("Transform to Standard Position", QIcon(":/icons/TRANSFORM"));

    _actionGetComponent = new ActionGetComponent( "Remove Non-Face Components", QIcon(":/icons/FACE"), ui->progressBar);

    const QString haarModels = QDir( QApplication::applicationDirPath()).filePath( HAAR_CASCADES_MODELS);
    const QString faceShapeLandmarks = QDir( QApplication::applicationDirPath()).filePath( FACE_SHAPE_LANDMARKS);
    _actionDetectFace = new ActionDetectFace( "Detect Face", QIcon(":/icons/DETECT_FACE"),
                                              haarModels, faceShapeLandmarks, this, ui->progressBar);
    _actionDetectFace->execAfter( _actionTransformToStandardPosition);  // Transform to origin.
    _actionDetectFace->execAfter( _actionEditLandmarks);                // Show detected landmarks.
    _actionDetectFace->execAfter( _actionOrientCameraToFrontFace);      // Orient camera to front face.

    _actionResetCamera = new ActionResetCamera( "Reset All Cameras", QIcon(":/icons/RESET_CAMERA"));
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

    _actionSmooth = new ActionSmooth( "Smooth Face", QIcon(":/icons/SHAVE"), ui->progressBar);
    _actionRemesh = new ActionRemesh( "Remesh Face", QIcon(":/icons/REMESH"), ui->progressBar);

    _actionToggleAxes = new ActionToggleAxes( "World Axes", QIcon(":/icons/AXES"));
    _actionToggleAxes->addViewer(_mfmv->leftViewer());
    _actionToggleAxes->addViewer(_mfmv->centreViewer());
    _actionToggleAxes->addViewer(_mfmv->rightViewer());

    _actionToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Move Models", QIcon(":/icons/MOVE_MODEL"));
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
    _actionRadialSelect = new ActionRadialSelect( "Radial Select", QIcon(":/icons/LASSO"), _feei, ui->statusbar);
    _actionCrop = new ActionCrop( "Crop Face", QIcon(":/icons/SCALPEL"), _actionRadialSelect, ui->progressBar);
    _actionAlignLandmarks = new ActionAlignLandmarks( "Align Landmarks (ICP)", QIcon(":/icons/ALIGN"), ui->progressBar);
    _actionFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"), ui->progressBar);
    _actionSetOpacity = new ActionSetOpacity( "Model Opacity", 0.6/*opacity on overlap*/, 0.15/*min opacity*/, this);
    _actionBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"));

    _actionEditPaths = new ActionEditPaths( "Edit Paths", QIcon(":/icons/CALIPERS"), _feei, ui->statusbar);
    _actionAddPath = new ActionAddPath( "Add Path", QIcon(":/icons/CALIPERS"), _actionEditPaths);
    _actionRenamePath = new ActionRenamePath( "Rename Path", QIcon(":/icons/EDIT"), _actionEditPaths, this);
    _actionDeletePath = new ActionDeletePath( "Delete Path", QIcon(":/icons/ERASER"), _actionEditPaths);

    _actionRenameLandmark = new ActionRenameLandmark( "Rename Landmark", QIcon(":/icons/EDIT"), _actionEditLandmarks, this);
    _actionAddLandmark = new ActionAddLandmark( "Add Landmark", QIcon(":/icons/MARKER"), _actionEditLandmarks, this);
    _actionDeleteLandmark = new ActionDeleteLandmark( "Delete Landmark", QIcon(":/icons/ERASER"), _actionEditLandmarks);
}   // end createActions



void ClinifaceMain::setupMainViewer()
{
    using namespace FaceTools;
    using namespace FaceTools::Action;

    FaceModelViewer* lv = _mfmv->leftViewer();
    FaceModelViewer* cv = _mfmv->centreViewer();
    FaceModelViewer* rv = _mfmv->rightViewer();

    FaceAction* actionResetL = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), lv);
    FaceAction* actionResetC = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), cv);
    FaceAction* actionResetR = new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), rv);
    _mfmv->setLeftResetCameraAction(   actionResetL->qaction());
    _mfmv->setCentreResetCameraAction( actionResetC->qaction());
    _mfmv->setRightResetCameraAction(  actionResetR->qaction());

    FaceAction* actionSaveL = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), lv);
    FaceAction* actionSaveC = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), cv);
    FaceAction* actionSaveR = new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), rv);
    _mfmv->setLeftSaveImageAction(   actionSaveL->qaction());
    _mfmv->setCentreSaveImageAction( actionSaveC->qaction());
    _mfmv->setRightSaveImageAction(  actionSaveR->qaction());

    FaceTools::Action::ModelSelector* selector = _fam->selector();

    // Set move actions
    FaceAction* actionMoveLC = new ActionMoveViewer( cv, selector, lv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveCL = new ActionMoveViewer( lv, selector, cv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    FaceAction* actionMoveCR = new ActionMoveViewer( rv, selector, cv, "Move Right", QIcon(":/icons/MOVE_RIGHT"));
    FaceAction* actionMoveRC = new ActionMoveViewer( cv, selector, rv, "Move Left", QIcon(":/icons/MOVE_LEFT"));
    _mfmv->setMoveLeftToCentreAction(  actionMoveLC->qaction());
    _mfmv->setMoveCentreToLeftAction(  actionMoveCL->qaction());
    _mfmv->setMoveCentreToRightAction( actionMoveCR->qaction());
    _mfmv->setMoveRightToCentreAction( actionMoveRC->qaction());

    // Set copy actions
    FaceAction* actionCopyLC = new ActionCopyViewer( cv, selector, lv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyCL = new ActionCopyViewer( lv, selector, cv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
    FaceAction* actionCopyCR = new ActionCopyViewer( rv, selector, cv, "Copy Right", QIcon(":/icons/COPY_RIGHT"));
    FaceAction* actionCopyRC = new ActionCopyViewer( cv, selector, rv, "Copy Left", QIcon(":/icons/COPY_LEFT"));
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



// public
ClinifaceMain::ClinifaceMain() : QMainWindow(nullptr), ui(new Ui::ClinifaceMain)
{
    ui->setupUi(this);
    setAcceptDrops(true);   // Accept dropping of files onto this widget
    setContextMenuPolicy(Qt::NoContextMenu);

    _mfmv = new FaceTools::MultiFaceModelViewer( this);
    _fam = new FaceTools::Action::FaceActionManager( _mfmv->centreViewer(), 20/*model load limit*/, this);
    _fam->addViewer( _mfmv->leftViewer());
    _fam->addViewer( _mfmv->rightViewer());

    // Needed for some actions that check if models are under the mouse cursor.
    _feei = new FaceTools::Interactor::FaceEntryExitInteractor;
    _feei->setViewer(_mfmv->centreViewer());
    setupMainViewer();

    initFileIO();
    createActions();

    QWidget* cwidget = new QWidget;
    cwidget->setLayout( new QVBoxLayout);
    cwidget->layout()->setContentsMargins(0,0,0,0);
    cwidget->layout()->addWidget( _mfmv);
    cwidget->layout()->addWidget( ui->progressBar);
    setCentralWidget( cwidget);
    _mfmv->layout()->setContentsMargins(3,1,3,1);

    _fam->loadPlugins();

    createToolBar();
    createFileMenu();
    createViewMenu();
    createToolsMenu();
    createHelpMenu();
    createContextMenu();

    registerActions();

    // Locate centrally on desktop
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), qApp->desktop()->availableGeometry()));

    // Update window title whenever an action finishes.
    connect( _fam, &FaceTools::Action::FaceActionManager::onUpdateSelected, this, &ClinifaceMain::doOnUpdateSelected);
    doOnUpdateSelected(nullptr);
}   // end ctor


// public
ClinifaceMain::~ClinifaceMain()
{
    delete _cmenu;
    delete _feei;
    delete _fam;
    delete ui;
}   // end dtor


// protected virtual
QSize ClinifaceMain::sizeHint() const { return QSize( 800, 700);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname)
{
    const bool loadedOkay = _actionLoadFaceModels->loadModel( fname);
    if ( !loadedOkay)
        _fam->modelManager()->loader()->showLoadErrors();
    return loadedOkay;
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
            if ( _fam->modelManager()->canRead( fname.toStdString()))
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
void ClinifaceMain::doOnUpdateSelected( const FaceTools::FaceControl* fc)
{
    QString wtitle = APP_NAME;
    if ( fc)
    {
        FaceTools::FaceModel* fm = fc->data();
        QString smsg = "";
        if ( !fm->isSaved())
            smsg = "(*) ";
        wtitle = smsg + tr(_fam->modelManager()->filepath(fm).c_str());
    }   // end if
    setWindowTitle( wtitle);
}   // end doOnUpdateSelected

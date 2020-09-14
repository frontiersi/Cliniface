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

#ifndef Cliniface_CLINIFACE_MAIN_H
#define Cliniface_CLINIFACE_MAIN_H

#include <FaceTools/Action/ActionLoad.h>
#include <FaceTools/Action/ActionAddPath.h>
#include <FaceTools/Action/ActionMarquee.h>
#include <FaceTools/Action/ActionEditPaths.h>
#include <FaceTools/Action/ActionExportPDF.h>
#include <FaceTools/Action/ActionExportMask.h>
#include <FaceTools/Action/ActionToggleAxes.h>
#include <FaceTools/Action/ActionDeletePath.h>
#include <FaceTools/Action/ActionRenamePath.h>
#include <FaceTools/Action/ActionSetOpacity.h>
#include <FaceTools/Action/ActionExtractFace.h>
#include <FaceTools/Action/ActionShowMetrics.h>
#include <FaceTools/Action/ActionMakeHalfFace.h>
#include <FaceTools/Action/ActionShowMeshInfo.h>
#include <FaceTools/Action/ActionShowScanInfo.h>
#include <FaceTools/Action/ActionRadialSelect.h>
#include <FaceTools/Action/ActionEditLandmarks.h>
#include <FaceTools/Action/ActionSaveFaceModel.h>
#include <FaceTools/Action/ActionDeleteAllPaths.h>
#include <FaceTools/Action/ActionSaveScreenshot.h>
#include <FaceTools/Action/ActionShowPhenotypes.h>
#include <FaceTools/Action/ActionDiscardManifold.h>
#include <FaceTools/Action/ActionRemoveManifolds.h>
#include <FaceTools/Action/ActionSaveAsFaceModel.h>
#include <FaceTools/Action/ActionSynchroniseCameras.h>
#include <FaceTools/Action/ActionToggleScalarLegend.h>
#include <FaceTools/Action/ActionRestoreSingleLandmark.h>
#include <FaceTools/Action/ActionToggleStereoRendering.h>
#include <FaceTools/Action/ActionSetParallelProjection.h>
#include <FaceTools/Action/ActionToggleCameraActorInteraction.h>

#include <FaceTools/Interactor/ActionClickHandler.h>
#include <FaceTools/Interactor/ContextMenuHandler.h>
#include <FaceTools/Interactor/RadialSelectHandler.h>
#include <FaceTools/Interactor/LandmarksHandler.h>
#include <FaceTools/Interactor/PathsHandler.h>

#include <FaceTools/MultiFaceModelViewer.h>

#include <QTools/PluginUIPoints.h>
#include <QTools/HelpAssistant.h>

#include <QDragEnterEvent>
#include <QMainWindow>

#include "ClinifacePluginsLoader.h"
#include "PreferencesDialog.h"
#include "UpdatesDialog.h"
#include <lua.hpp>

using namespace FaceTools::Action;


namespace Ui { class ClinifaceMain;}

namespace Cliniface {

class ClinifaceMain : public QMainWindow
{ Q_OBJECT
public:
    ClinifaceMain();
    ~ClinifaceMain() override;

public slots:
    bool loadModel( const QString&);

protected:
    void dragEnterEvent( QDragEnterEvent*) override;
    void dropEvent( QDropEvent*) override;
    void closeEvent( QCloseEvent *) override;
    QSize sizeHint() const override;

private slots:
    void _doOnUpdateSelected();
    void _doOnAttachPluginActionToUI( FaceAction*);

private:
    Ui::ClinifaceMain *_ui;
    ClinifacePluginsLoader *_ploader;
    QTools::HelpAssistant *_helpAss;
    PreferencesDialog *_prefsDialog;
    UpdatesDialog *_updatesDialog;
    FaceTools::MultiFaceModelViewer *_mfmv;
    FaceTools::Interactor::PathsHandler::Ptr _pathsHandler;
    FaceTools::Interactor::LandmarksHandler::Ptr _lmksHandler;
    FaceTools::Interactor::RadialSelectHandler::Ptr _rselHandler;
    FaceTools::Interactor::ContextMenuHandler::Ptr _cmenuHandler;
    FaceTools::Interactor::ActionClickHandler::Ptr _aclkHandler;
    QTools::PluginUIPoints _ppoints;    // Plugin points for menus and toolbars

    FaceAction *_actRedo;
    FaceAction *_actUndo;
    FaceAction *_actSmooth;
    FaceAction *_actReflect;
    FaceAction *_actSetFocus;
    FaceAction *_actRotateX90;
    FaceAction *_actRotateY90;
    FaceAction *_actRotateZ90;
    FaceAction *_actFillHoles;
    FaceAction *_actAlignModel;
    FaceAction *_actDetectFace;
    FaceAction *_actFixNormals;
    FaceAction *_actScaleModel;
    FaceAction *_actToggleMask;
    FaceAction *_actVisTexture;
    FaceAction *_actCentreModel;
    FaceAction *_actVisOutlines;
    FaceAction *_actVisWireframe;
    FaceAction *_actInvertNormals;
    FaceAction *_actShowLandmarks;
    FaceAction *_actVisPolyLabels;
    FaceAction *_actAlignLandmarks;
    FaceAction *_actCloseFaceModel;
    FaceAction *_actExportMetaData;
    FaceAction *_actImportMetaData;
    FaceAction *_actResetDetection;

    FaceAction *_actVisMedianPlane;
    FaceAction *_actVisTransversePlane;
    //FaceAction *_actVisFrontalPlane;

    FaceAction *_actBackfaceCulling;
    FaceAction *_actOrientCameraToF;
    FaceAction *_actOrientCameraToL;
    FaceAction *_actOrientCameraToLQ;
    FaceAction *_actOrientCameraToR;
    FaceAction *_actOrientCameraToRQ;
    FaceAction *_actOrientCameraToT;
    FaceAction *_actOrientCameraToTQ;
    FaceAction *_actOrientCameraToB;
    FaceAction *_actOrientCameraToBQ;
    FaceAction *_actRestoreLandmarks;
    FaceAction *_actVisVertexLabels;
    FaceAction *_actVisLandmarkLabels;
    FaceAction *_actCloseAllFaceModels;

    //FaceAction *_actSetMinScalarColour;
    //FaceAction *_actSetMaxScalarColour;
    FaceAction *_actSetNumScalarColours;
    FaceAction *_actChangeSurfaceMappingRange;

    ActionLoad                           *_actLoad;
    ActionAddPath                        *_actAddPath;
    ActionMarquee                        *_actMarquee;
    ActionEditPaths                      *_actEditPaths;
    ActionExportPDF                      *_actExportPDF;
    ActionExportMask                     *_actExportMask;
    ActionToggleAxes                     *_actToggleAxes;
    ActionDeletePath                     *_actDeletePath;
    ActionRenamePath                     *_actRenamePath;
    ActionSetOpacity                     *_actSetOpacity;
    ActionExtractFace                    *_actExtractFace;
    ActionShowMetrics                    *_actShowMetrics;
    ActionShowMeshInfo                   *_actShowMeshInfo;
    ActionShowScanInfo                   *_actShowScanInfo;
    ActionMakeHalfFace                   *_actMakeLeftFace;
    ActionMakeHalfFace                   *_actMakeRightFace;
    ActionRadialSelect                   *_actRadialSelect;
    ActionEditLandmarks                  *_actEditLandmarks;
    ActionSaveFaceModel                  *_actSaveFaceModel;
    ActionDeleteAllPaths                 *_actDeleteAllPaths;
    ActionSaveScreenshot                 *_actSaveScreenshot;
    ActionShowPhenotypes                 *_actShowPhenotypes;
    ActionDiscardManifold                *_actDiscardManifold;
    ActionRemoveManifolds                *_actRemoveManifolds;
    ActionSaveAsFaceModel                *_actSaveAsFaceModel;
    ActionSynchroniseCameras             *_actSynchroniseCameras;
    ActionToggleScalarLegend             *_actToggleScalarLegend;
    ActionRestoreSingleLandmark          *_actRestoreSingleLandmark;
    ActionToggleStereoRendering          *_actToggleStereoRendering;
    ActionSetParallelProjection          *_actSetParallelProjection;
    ActionToggleCameraActorInteraction   *_actToggleCameraActorInteraction;

    void _createFileMenu();
    void _createViewMenu();
    void _createCameraMenu();
    void _createTransformMenu();
    void _createGeometryMenu();
    void _createMetricsMenu();
    void _createHelpMenu();
    void _createToolBar();
    void _createContextMenu();

    void _createHandlers();
    void _createActions();
    void _createMetrics();
    void _registerActions();

    void _setupMainViewer();
    void _loadPlugins();

    ClinifaceMain( const ClinifaceMain&) = delete;
    void operator=( const ClinifaceMain&) = delete;
};  // end class

}   // end namespace

#endif

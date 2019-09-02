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

#ifndef Cliniface_CLINIFACE_MAIN_H
#define Cliniface_CLINIFACE_MAIN_H

#include <ActionCrop.h>
#include <ActionAddPath.h>
#include <ActionMarquee.h>
#include <ActionEditPaths.h>
#include <ActionExportPDF.h>
#include <ActionToggleAxes.h>
#include <ActionDeletePath.h>
#include <ActionRenamePath.h>
#include <ActionSetOpacity.h>
#include <ActionResetCamera.h>
#include <ActionShowMetrics.h>
#include <ActionMakeHalfFace.h>
#include <ActionShowScanInfo.h>
#include <ActionRadialSelect.h>
#include <ActionEditLandmarks.h>
#include <ActionLoadFaceModels.h>
#include <ActionSaveScreenshot.h>
#include <ActionDiscardManifold.h>
#include <ActionRemoveManifolds.h>
#include <ActionOrientCameraToFace.h>
#include <ActionToggleScalarLegend.h>
#include <ActionShowModelProperties.h>
#include <ActionNonRigidRegistration.h>
#include <ActionToggleStereoRendering.h>
#include <ActionSetParallelProjection.h>
#include <ActionSynchroniseCameraMovement.h>
#include <ActionToggleCameraActorInteraction.h>

#include <ContextMenu.h>
#include <MetricsDialog.h>
#include <MovementTrigger.h>
#include <PathsInteractor.h>
#include <MultiFaceModelViewer.h>
#include <PathSetVisualisation.h>
#include <LandmarksVisualisation.h>
#include <PluginUIPoints.h> // QTools

#include <QDragEnterEvent>
#include <QMainWindow>

#include "ClinifacePluginsLoader.h"
#include <HelpAssistant.h>
#include "PreferencesDialog.h"
#include <Cliniface_Config.h>
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
    void _doOnUpdate( const FaceTools::FM*);
    void _doOnAttachPluginActionToUI( FaceAction*);

private:
    Ui::ClinifaceMain *_ui;
    ClinifacePluginsLoader *_ploader;
    QTools::HelpAssistant *_helpAss;
    PreferencesDialog *_prefsDialog;
    FaceTools::MultiFaceModelViewer *_mfmv;
    FaceTools::Vis::PathSetVisualisation _pathsVis;
    FaceTools::Vis::LandmarksVisualisation _landmarksVis;
    FaceTools::Interactor::PathsInteractor::Ptr _pathsInteractor;
    FaceTools::Interactor::ContextMenu *_cmenu;
    FaceTools::Interactor::MovementTrigger *_mtrigger;
    QTools::PluginUIPoints _ppoints;    // Plugin points for menus and toolbars

    FaceAction *_actionRedo;
    FaceAction *_actionUndo;
    FaceAction *_actionSmooth;
    FaceAction *_actionReflect;
    FaceAction *_actionAlignICP;
    FaceAction *_actionSetFocus;
    FaceAction *_actionRotateX90;
    FaceAction *_actionRotateY90;
    FaceAction *_actionRotateZ90;
    FaceAction *_actionFillHoles;
    FaceAction *_actionDetectFace;
    FaceAction *_actionScaleModel;
    FaceAction *_actionVisTexture;
    FaceAction *_actionVisOutlines;
    FaceAction *_actionVisWireframe;
    FaceAction *_actionInvertNormals;
    FaceAction *_actionAlignVertices;
    FaceAction *_actionSaveFaceModel;
    FaceAction *_actionVisPolyLabels;
    FaceAction *_actionAlignLandmarks;
    FaceAction *_actionCloseFaceModel;
    FaceAction *_actionExportMetaData;
    FaceAction *_actionImportMetaData;
    FaceAction *_actionResetDetection;
    FaceAction *_actionBackfaceCulling;
    FaceAction *_actionSaveAsFaceModel;
    FaceAction *_actionVisVertexLabels;
    FaceAction *_actionTransformToCentre;
    FaceAction *_actionLoadDirFaceModels;
    FaceAction *_actionCloseAllFaceModels;
    FaceAction *_actionOrientCameraToFrontFace;
    FaceAction *_actionTransformToStandardPosition;

    FaceAction *_actionSetMinScalarColour;
    FaceAction *_actionSetMaxScalarColour;
    FaceAction *_actionSetNumScalarColours;
    FaceAction *_actionChangeSurfaceMappingRange;

    ActionCrop                           *_actionCrop;
    ActionAddPath                        *_actionAddPath;
    ActionMarquee                        *_actionMarquee;
    ActionEditPaths                      *_actionEditPaths;
    ActionExportPDF                      *_actionExportPDF;
    ActionToggleAxes                     *_actionToggleAxes;
    ActionDeletePath                     *_actionDeletePath;
    ActionRenamePath                     *_actionRenamePath;
    ActionSetOpacity                     *_actionSetOpacity;
    ActionResetCamera                    *_actionResetCamera;
    ActionShowMetrics                    *_actionShowMetrics;
    ActionShowScanInfo                   *_actionShowScanInfo;
    ActionMakeHalfFace                   *_actionMakeLeftFace;
    ActionMakeHalfFace                   *_actionMakeRightFace;
    ActionRadialSelect                   *_actionRadialSelect;
    ActionEditLandmarks                  *_actionEditLandmarks;
    ActionLoadFaceModels                 *_actionLoadFaceModels;
    ActionSaveScreenshot                 *_actionSaveScreenshot;
    ActionDiscardManifold                *_actionDiscardManifold;
    ActionRemoveManifolds                *_actionRemoveManifolds;
    ActionOrientCameraToFace             *_actionOrientCameraToLeftFace;
    ActionOrientCameraToFace             *_actionOrientCameraToRightFace;
    ActionToggleScalarLegend             *_actionToggleScalarLegend;
    ActionShowModelProperties            *_actionShowModelProperties;
    ActionNonRigidRegistration           *_actionNonRigidRegistration;
    ActionToggleStereoRendering          *_actionToggleStereoRendering;
    ActionSetParallelProjection          *_actionSetParallelProjection;
    ActionSynchroniseCameraMovement      *_actionSynchroniseCameraMovement;
    ActionToggleCameraActorInteraction   *_actionToggleCameraActorInteraction;

    void createFileMenu();
    void createViewMenu();
    void createCameraMenu();
    void createTransformMenu();
    void createGeometryMenu();
    void createMetricsMenu();
    void createHelpMenu();
    void createToolBar();
    void createContextMenu();

    void createActions();
    void createMetrics();
    void registerActions();

    void setupMainViewer();

    ClinifaceMain( const ClinifaceMain&) = delete;
    void operator=( const ClinifaceMain&) = delete;
};  // end class

}   // end namespace

#endif

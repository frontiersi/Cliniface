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

#include <Action/ActionCrop.h>
#include <Action/ActionAddPath.h>
#include <Action/ActionMarquee.h>
#include <Action/ActionEditPaths.h>
#include <Action/ActionExportPDF.h>
#include <Action/ActionToggleAxes.h>
#include <Action/ActionDeletePath.h>
#include <Action/ActionRenamePath.h>
#include <Action/ActionSetOpacity.h>
#include <Action/ActionResetCamera.h>
#include <Action/ActionShowMetrics.h>
#include <Action/ActionMakeHalfFace.h>
#include <Action/ActionShowScanInfo.h>
#include <Action/ActionRadialSelect.h>
#include <Action/ActionEditLandmarks.h>
#include <Action/ActionLoadFaceModels.h>
#include <Action/ActionSaveScreenshot.h>
#include <Action/ActionDiscardManifold.h>
#include <Action/ActionRemoveManifolds.h>
#include <Action/ActionOrientCameraToFace.h>
#include <Action/ActionToggleScalarLegend.h>
#include <Action/ActionShowModelProperties.h>
#include <Action/ActionNonRigidRegistration.h>
#include <Action/ActionToggleStereoRendering.h>
#include <Action/ActionSetParallelProjection.h>
#include <Action/ActionSynchroniseCameraMovement.h>
#include <Action/ActionToggleCameraActorInteraction.h>

#include <Interactor/ContextMenu.h>
#include <Interactor/MovementNotifier.h>
#include <Interactor/PathsHandler.h>

#include <MultiFaceModelViewer.h>

#include <Vis/LandmarksVisualisation.h>

#include <Widget/MetricsDialog.h>

#include <PluginUIPoints.h> // QTools
#include <HelpAssistant.h>  // QTools

#include <QDragEnterEvent>
#include <QMainWindow>

#include "ClinifacePluginsLoader.h"
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
    FaceTools::Vis::LandmarksVisualisation _landmarksVis;
    FaceTools::Interactor::PathsHandler::Ptr _pathsHandler;
    FaceTools::Interactor::MovementNotifier *_mnotifier;
    FaceTools::Interactor::ContextMenu *_cmenu;
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

    void _createFileMenu();
    void _createViewMenu();
    void _createCameraMenu();
    void _createTransformMenu();
    void _createGeometryMenu();
    void _createMetricsMenu();
    void _createHelpMenu();
    void _createToolBar();
    void _createContextMenu();

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

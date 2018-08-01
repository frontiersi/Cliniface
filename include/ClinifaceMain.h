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

#ifndef Cliniface_CLINIFACE_MAIN_H
#define Cliniface_CLINIFACE_MAIN_H

#include <ActionCrop.h>
#include <ActionRemesh.h>
#include <ActionSmooth.h>
#include <ActionAddPath.h>
#include <ActionMarquee.h>
#include <ActionSetFocus.h>
#include <ActionVisualise.h>
#include <ActionEditPaths.h>
#include <ActionExportPDF.h>
#include <ActionFillHoles.h>
#include <ActionDeletePath.h>
#include <ActionDetectFace.h>
#include <ActionCopyViewer.h>
#include <ActionMoveViewer.h>
#include <ActionToggleAxes.h>
#include <ActionRenamePath.h>
#include <ActionSetOpacity.h>
#include <ActionAddLandmark.h>
#include <ActionResetCamera.h>
#include <ActionRadialSelect.h>
#include <ActionGetComponent.h>
#include <ActionEditLandmarks.h>
#include <ActionAlignLandmarks.h>
#include <ActionDeleteLandmark.h>
#include <ActionMapSurfaceData.h>
#include <ActionLoadFaceModels.h>
#include <ActionSaveFaceModels.h>
#include <ActionSaveScreenshot.h>
#include <ActionRenameLandmark.h>
#include <ActionBackfaceCulling.h>
#include <ActionCloseFaceModels.h>
#include <ActionSaveAsFaceModel.h>
#include <ActionLoadDirFaceModels.h>
#include <ActionCloseAllFaceModels.h>
#include <ActionOrientCameraToFace.h>
#include <ActionSetParallelProjection.h>
#include <ActionSynchroniseCameraMovement.h>
#include <ActionTransformToStandardPosition.h>
#include <ActionToggleCameraActorInteraction.h>
#include <ViewerInteractionManager.h>
#include <ContextMenuInteractor.h>
#include <MultiFaceModelViewer.h>
#include <FaceActionManager.h>

#include <QDragEnterEvent>
#include <QMainWindow>

#include <Cliniface_Config.h>

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
    void doOnUpdateSelected();

private:
    Ui::ClinifaceMain *ui;
    FaceActionManager* _fam;
    FaceTools::MultiFaceModelViewer* _mfmv;
    FaceTools::Interactor::ViewerInteractionManager* _interactions;
    FaceTools::Interactor::FaceEntryExitInteractor* _feei;
    FaceTools::Interactor::ContextMenuInteractor* _cmenu;

    // File IO
    ActionLoadFaceModels                *_actionLoadFaceModels;
    ActionLoadDirFaceModels             *_actionLoadDirFaceModels;
    ActionSaveFaceModels                *_actionSaveFaceModels;
    ActionSaveAsFaceModel               *_actionSaveAsFaceModel;
    ActionCloseFaceModels               *_actionCloseFaceModels;
    ActionCloseAllFaceModels            *_actionCloseAllFaceModels;

    ActionVisualise                     *_actionVisOutlines;

    // Tools
    ActionCrop                          *_actionCrop;
    ActionRemesh                        *_actionRemesh;
    ActionSmooth                        *_actionSmooth;
    ActionAddPath                       *_actionAddPath;
    ActionMarquee                       *_actionMarquee;
    ActionSetFocus                      *_actionSetFocus;
    ActionEditPaths                     *_actionEditPaths;
    ActionExportPDF                     *_actionExportPDF;
    ActionFillHoles                     *_actionFillHoles;
    ActionDeletePath                    *_actionDeletePath;
    ActionDetectFace                    *_actionDetectFace;
    ActionRenamePath                    *_actionRenamePath;
    ActionSetOpacity                    *_actionSetOpacity;
    ActionToggleAxes                    *_actionToggleAxes;
    ActionAddLandmark                   *_actionAddLandmark;
    ActionResetCamera                   *_actionResetCamera;
    ActionRadialSelect                  *_actionRadialSelect;
    ActionGetComponent                  *_actionGetComponent;
    ActionEditLandmarks                 *_actionEditLandmarks;
    ActionAlignLandmarks                *_actionAlignLandmarks;
    ActionRenameLandmark                *_actionRenameLandmark;
    ActionSaveScreenshot                *_actionSaveScreenshot;
    ActionDeleteLandmark                *_actionDeleteLandmark;
    ActionBackfaceCulling               *_actionBackfaceCulling;
    ActionOrientCameraToFace            *_actionOrientCameraToLeftFace;
    ActionOrientCameraToFace            *_actionOrientCameraToFrontFace;
    ActionOrientCameraToFace            *_actionOrientCameraToRightFace;
    ActionSetParallelProjection         *_actionSetParallelProjection;
    ActionSynchroniseCameraMovement     *_actionSynchroniseCameraMovement;
    ActionTransformToStandardPosition   *_actionTransformToStandardPosition;
    ActionToggleCameraActorInteraction  *_actionToggleCameraActorInteraction;

    void createFileMenu();
    void createViewMenu();
    void createToolsMenu();
    void createHelpMenu();
    void createToolBar();
    void createContextMenu();

    void createVisualisations();
    void createFileIO();
    void createActions();

    void setupMainViewer();

    ClinifaceMain( const ClinifaceMain&) = delete;
    void operator=( const ClinifaceMain&) = delete;
};  // end class

}   // end namespace

#endif

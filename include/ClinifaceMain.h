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
#include <ActionAddPath.h>
#include <ActionMarquee.h>
#include <ActionExportPDF.h>
#include <ActionEditPaths.h>
#include <ActionToggleAxes.h>
#include <ActionDeletePath.h>
#include <ActionRenamePath.h>
#include <ActionAddLandmark.h>
#include <ActionResetCamera.h>
#include <ActionRadialSelect.h>
#include <ActionEditLandmarks.h>
#include <ActionLoadFaceModels.h>
#include <ActionDeleteLandmark.h>
#include <ActionRenameLandmark.h>
#include <ActionSaveScreenshot.h>
#include <ActionOrientCameraToFace.h>
#include <ActionSetParallelProjection.h>
#include <ActionSynchroniseCameraMovement.h>
#include <ActionToggleCameraActorInteraction.h>

#include <FaceEntryExitInteractor.h>
#include <ContextMenuInteractor.h>
#include <MultiFaceModelViewer.h>
#include <FaceActionManager.h>
#include <QDragEnterEvent>
#include <QMainWindow>

#include <Cliniface_Config.h>

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
    void doOnUpdateSelected( const FaceTools::FaceControl*);

private:
    Ui::ClinifaceMain *ui;
    FaceTools::Action::FaceActionManager *_fam;
    FaceTools::MultiFaceModelViewer *_mfmv;
    FaceTools::Interactor::FaceEntryExitInteractor *_feei;
    FaceTools::Interactor::ContextMenuInteractor *_cmenu;

    FaceTools::Action::FaceAction *_actionRemesh;
    FaceTools::Action::FaceAction *_actionSmooth;
    FaceTools::Action::FaceAction *_actionSetFocus;
    FaceTools::Action::FaceAction *_actionFillHoles;
    FaceTools::Action::FaceAction *_actionDetectFace;
    FaceTools::Action::FaceAction *_actionSetOpacity;
    FaceTools::Action::FaceAction *_actionVisOutlines;
    FaceTools::Action::FaceAction *_actionGetComponent;
    FaceTools::Action::FaceAction *_actionAlignLandmarks;
    FaceTools::Action::FaceAction *_actionSaveFaceModels;
    FaceTools::Action::FaceAction *_actionBackfaceCulling;
    FaceTools::Action::FaceAction *_actionCloseFaceModels;
    FaceTools::Action::FaceAction *_actionSaveAsFaceModel;
    FaceTools::Action::FaceAction *_actionLoadDirFaceModels;
    FaceTools::Action::FaceAction *_actionCloseAllFaceModels;
    FaceTools::Action::FaceAction *_actionOrientCameraToFrontFace;
    FaceTools::Action::FaceAction *_actionTransformToStandardPosition;

    FaceTools::Action::ActionCrop                           *_actionCrop;
    FaceTools::Action::ActionAddPath                        *_actionAddPath;
    FaceTools::Action::ActionMarquee                        *_actionMarquee;
    FaceTools::Action::ActionEditPaths                      *_actionEditPaths;
    FaceTools::Action::ActionExportPDF                      *_actionExportPDF;
    FaceTools::Action::ActionToggleAxes                     *_actionToggleAxes;
    FaceTools::Action::ActionDeletePath                     *_actionDeletePath;
    FaceTools::Action::ActionRenamePath                     *_actionRenamePath;
    FaceTools::Action::ActionAddLandmark                    *_actionAddLandmark;
    FaceTools::Action::ActionResetCamera                    *_actionResetCamera;
    FaceTools::Action::ActionRadialSelect                   *_actionRadialSelect;
    FaceTools::Action::ActionEditLandmarks                  *_actionEditLandmarks;
    FaceTools::Action::ActionDeleteLandmark                 *_actionDeleteLandmark;
    FaceTools::Action::ActionLoadFaceModels                 *_actionLoadFaceModels;
    FaceTools::Action::ActionRenameLandmark                 *_actionRenameLandmark;
    FaceTools::Action::ActionSaveScreenshot                 *_actionSaveScreenshot;
    FaceTools::Action::ActionSetParallelProjection          *_actionSetParallelProjection;
    FaceTools::Action::ActionOrientCameraToFace             *_actionOrientCameraToLeftFace;
    FaceTools::Action::ActionOrientCameraToFace             *_actionOrientCameraToRightFace;
    FaceTools::Action::ActionSynchroniseCameraMovement      *_actionSynchroniseCameraMovement;
    FaceTools::Action::ActionToggleCameraActorInteraction   *_actionToggleCameraActorInteraction;

    void createFileMenu();
    void createViewMenu();
    void createToolsMenu();
    void createHelpMenu();
    void createToolBar();
    void createContextMenu();

    void initFileIO();
    void createActions();
    void registerActions();

    void setupMainViewer();

    ClinifaceMain( const ClinifaceMain&) = delete;
    void operator=( const ClinifaceMain&) = delete;
};  // end class

}   // end namespace

#endif

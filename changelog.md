#### 3.1.0 (in development)
- Radial selection added to context menu.
- Changed movement of radial selection to left-click and drag on centre reticule.
- Changed resize of radial selection area to require mouse hover over centre reticule.
- Highlight radial selection visualisation when mouse hovers over centre reticule.
- Made landmark/path handle size independent of camera zoom.
- Cross cursor icon when moving landmarks/path handles or face actors.
- Changed some notifications and status bar wordings.
- Can add landmarks/paths now without first enabling the corresponding visualisations.
- Side viewers prevented from collapsing entirely by user (or expanding when empty by dragging sides).
- Incorporated storage of facial "centre" in XML file.
- Changed implementation of ActionRadialSelect.
- Changed behaviour of actions that rely on ActionRadialSelect.
- Changes to action framework to allow conditional processing of actions based on delegate lambdas.
- Provided delegate lambda functionality for process flag determination on auto process of actions.
- Auto-setting of opacity now restores the moved view to full opacity upon moving out of overlap.
- Radial select visualisation turned off after crop.
- Consolidated surface/texture into a single actor.
- Changed implementation of wireframe view to retain surface.
- Changed texture visualisation into toggle on/off option.
- Changed initial face detection range to be further away.
- Changed refinement calculation of facial orientation upon landmark detection.
- Changed landmark detection routines.
- Changed landmark names.
- Added action to toggle scalar legend on/off.
- Added action to change surface colour of selected FaceViews.
- Added actions to rotate 90 degrees through X,Y,Z axes.
- Added action to apply mirror reflection.
- Added action to invert polygon normals.
- Added FXAA antialiasing toggle.
- Added stereo rendering toggle.
- Added asymmetry visualisation (plugin).
- New texture and wireframe visualisation icons.
- Removed vertices only view.
- Changed email links in sample PDF report.
- New scalar mapping toolbar.
- Re-organised some menu/toolbar options.
- Added camera and scalar mapping sub-menus.
- Renamed "Tools" to "Model" menu
- Improved accuracy of automated face orientation.
- Enabled world axes by default.
- Added sex, age, ethnicity, and capture date metadata to FaceModel.
- Updated XML file format to include new metadata.
- New image details dialog for updating metadata.
- Incorporated age into .3df file for FaceModels.
- Selected interlandmark distances calculated automatically on detection and output to .3df on save.
- New metric calculator and visualisations.
- Metrics dialog for subselection of visualisation.
- Dynamic loading of metric age mapped statistic distributions (only Interlandmark distances supported for now).
- Included thumbnail of image in XML file.
- Updated acknowledgements, copyright info, and application description.
- Bug fix: Poking transform now applies to all views on all visualisations - whether or not applied.
- Bug fix: Translation of surface actors now properly synchronised when translating the face actor.
- Bug fix: Set focus now synchronised across viewers.
- Bug fix: Newly detected landmarks now displayed across copied views when detected on one.
- Bug fix: Region selector now works correctly over models having > 1 component.
- Bug fix: Crash fixed when region selector is unable to adjust centre position.
- Bug fix: Crash fixed when region selection radius goes to zero.
- Bug fix: Fixed some initial positioning issues of radial selector when > 1 view of a model shown.
- Bug fix: Landmark captions no longer flicker when dragging.
- Bug fix: View copying resource deadlock issue fixed.
- Bug fix: Fixed action queueing after copy.
- Bug fix: Fixed memory leak in reapplication of visualisation actors when switching viewers.
- Bug fix: Opacity setting now correctly applies to all models in a single viewer (and at load).
- Bug fix: Opacity reset on other views in viewer when selected view moved/copied/closed.
- Bug fix: Logo on about page now links out to website.
- Bug fix: Camera synchronises to viewer attached to currently selected FaceView.

#### 3.0.6.180730 (current release)
- Remeshing algorithm changes to improve polygon integration with unchanged polygons.

#### 3.0.5.180729
- Bug fix: backface culling (and opacity) now persist between model resets.
- Bug fix: orientation of polygon normals on hole filling and remeshing now matches adjacent polygons.
- Bug fix: hole filling no longer an available action when multiple components showing and hole boundaries on any of them.
- Bug fix: queued actions in FaceActionManager are now executed when previous queued actions aren't entered.
- Bug fix: outline views of holes/boundaries now complete loop (RVTK issue).

#### 3.0.4.180724
- Fixed viewer rendering upon orientation change action.
- New hole filling algorithm.
- New vertex remeshing algorithm.
- Very large reduction in size of texmfs for PDF reporting.
- Bug fix: remeshing no longer causes texture map to disappear due to model copy.

#### 3.0.3.180718
- Fixed referencing of facetools and pdf generation folders relative to application directory.
- Updated location of GitHub repository to "Cliniface/releases" page.
- Renamed PDF export action and placed button on toolbar.
- Bug fix: Models are now viewed properly after loading from command line.

#### 3.0.2.180717
- Moved face centre to mean of eye centres and nose tip for more central placement.
- Backface culling now applies to all model views in directly selected viewers.
- Modified remesh to silently fail instead of crashing on problem.
- Face profile buttons act over all viewers if "triple camera" selected.
- Offline installer created.
- Bug fix: model filepath not updating on selection change (regression)
- Bug fix: fixed vertex key mapping in ObjModel.

#### 3.0.1.180712
- Camera/model movement interaction switching.
- One and two button mouse camera/model interaction.
- Orthographic projection mode.
- World coordinates visualisation toggle.
- Fixed model transform to standard position.
- Re-worked caliper path measurement tool.
- Caliper path info written to 3DF.
- Landmark add/delete/rename included on context menu.
- Path add/delete/rename included on context menu.
- Added option to synchronise camera movement between viewers.
- Model opacity select and auto semi-opacity on overlap.
- Backface culling toggle.
- Region select visualisation now using RFeatures::ObjModelRegionSelector.
- Left and right face profile view actions.
- Action to align faces across landmarks with same names.
- Installation/updates using Qt based maintenance tool.
- Landmark and path actors mapped for fast (hash) lookup.
- Interaction data change events handled separately from Actions.

#### 3.0.0.180612
- Version provided to Cathryn Poulton for Milan demo.

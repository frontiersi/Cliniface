#### 3.2.1
- Cliniface configuration now persists in user's home directory.
- Reduced initial height of UI window for lower resolution monitors.
- Removed fixing of triangle normals on model load by AssImp (v3.3.1) since broken in some cases.
- Subject age replaced with date of birth (3DF files remain backwards compatible).
- Now calculating orientation and face centre dynamically from landmarks.
- Transform to Standard Position now only available if model not already in standard position & orientation.
- Added initial centre and orientation plus detected centre and orientation to 3DF file format.
- Record initial model centre and orientation for reset to image capture position.
- New command line interface for batch landmark detection, face component finding and 3DF export.
- Added Cliniface version to main window title bar when no model selected.
- Report templates can now specify their own availability for generation given a model's metadata.
- Fixed reappearance of prior landmark detection errors after a successful detection.
- Fixed crash when matching phenotypes against a metric without growth data.
- Fixed crash when merging faces back together with metric visualisation text showing.
- Fixed appearance of cursor and text label when (de)selecting landmarks.

#### 3.2.0
- Added Linux version as AppImage.
- Moved rendering backend from QVTKWidget to QVTKOpenGLWidget.
- New ancestry / sex dropdown menus for ScanInfo and MetricChart.
- Close button for MetricChart dialog.
- Metrics now provide multiple growth curves for separate ethnicities/sex.
- Scan Info dialog available ancestry / ethnicity now sourced from available metrics.
- Reformatted Scan Info dialog.
- Scan Info cancel button now works properly.
- Removed intersex options - made disjunction of female / male.
- Sorting enabled on metrics and HPO dialog tables.
- Multiple selectable distributions per metric.
- Changed landmark alignment algorithm to Procrustes rigid superimposition.
- Transforming a face back to standard position now also resets the camera.
- Dynamic loading of Lua scripts for defining phenotypes and metrics.
- Phenotypes now tested and indicated ones now written out on save.
- Option to ignore ethnicity when plotting subject on charts.
- Removed some less used actions from main toolbar.
- Landmarks on the non-selected face now have a different (more muted) colour.
- Removed disabling of model selection on viewing of metrics.
- Backface culling enabled automatically on orientation change if orientation matches glabella normal.
- Slight increase in tool button sizes and other small style changes.
- Caliper measurements now draw a path that follows the model's surface curvature.
- Semi-opaque backgrounds for bottom left and right text actors for better visibility.
- Improved search for initial facial detection and estimation of orientation.
- New options dialog to mask certain landmarks from being moved upon redetection.
- Moved face detection/classification models into data directory.
- Added action to reset detections and transform model back to origin.
- Added study ID to scan info (and XML file output).
- Added ability to save growth curve charts as image files.
- New dynamically loaded Lua PDF report definitions.
- U3D models cache in background as soon as report generation selected.
- Transformed model coordinates for media9 inclusion in LaTeX reports.
- Changed interaction mode in PDF reports to spin around Z axis for better control.
- Editable user preferences file for location of pdflatex etc.
- Newly created PDF reports now open after save in PDF reader.
- Changed min/max range mapping for asymmetry visualisation to -/+ 10mm.
- Fix exported U3D models to now use ambient instead of point lighting.
- Fix FaceActionManager to now clean up properly after itself.
- Fix landmark and path captions to now scale correctly.
- Fix progress bar now updates outside of GUI thread (still not showing for many actions).
- Fix overlap to reset across all views of a model when deselecting metric visualisation.
- Fix memory leak concerning thumbnail creation.
- Fix ScanInfo dialog always flagging model as unsaved on okay even without changes.
- Fix crash on applying visualisation layers when not available after copy or move.
- Fix update to metric calculation after changing source data in chart dialog.

#### 3.1.0
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
- Fix Poking transform now applies to all views on all visualisations - whether or not applied.
- Fix Translation of surface actors now properly synchronised when translating the face actor.
- Fix Set focus now synchronised across viewers.
- Fix Newly detected landmarks now displayed across copied views when detected on one.
- Fix Region selector now works correctly over models having > 1 component.
- Fix Crash fixed when region selector is unable to adjust centre position.
- Fix Crash fixed when region selection radius goes to zero.
- Fix Fixed some initial positioning issues of radial selector when > 1 view of a model shown.
- Fix Landmark captions no longer flicker when dragging.
- Fix View copying resource deadlock issue fixed.
- Fix Fixed action queueing after copy.
- Fix Fixed memory leak in reapplication of visualisation actors when switching viewers.
- Fix Opacity setting now correctly applies to all models in a single viewer (and at load).
- Fix Opacity reset on other views in viewer when selected view moved/copied/closed.
- Fix Logo on about page now links out to website.
- Fix Camera synchronises to viewer attached to currently selected FaceView.

#### 3.0.6.180730 (current release)
- Remeshing algorithm changes to improve polygon integration with unchanged polygons.

#### 3.0.5.180729
- Fix backface culling (and opacity) now persist between model resets.
- Fix orientation of polygon normals on hole filling and remeshing now matches adjacent polygons.
- Fix hole filling no longer an available action when multiple components showing and hole boundaries on any of them.
- Fix queued actions in FaceActionManager are now executed when previous queued actions aren't entered.
- Fix outline views of holes/boundaries now complete loop (RVTK issue).

#### 3.0.4.180724
- Fixed viewer rendering upon orientation change action.
- New hole filling algorithm.
- New vertex remeshing algorithm.
- Very large reduction in size of texmfs for PDF reporting.
- Fix remeshing no longer causes texture map to disappear due to model copy.

#### 3.0.3.180718
- Fixed referencing of facetools and pdf generation folders relative to application directory.
- Updated location of GitHub repository to "Cliniface/releases" page.
- Renamed PDF export action and placed button on toolbar.
- Fix Models are now viewed properly after loading from command line.

#### 3.0.2.180717
- Moved face centre to mean of eye centres and nose tip for more central placement.
- Backface culling now applies to all model views in directly selected viewers.
- Modified remesh to silently fail instead of crashing on problem.
- Face profile buttons act over all viewers if "triple camera" selected.
- Offline installer created.
- Fix model filepath not updating on selection change (regression)
- Fix fixed vertex key mapping in ObjModel.

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

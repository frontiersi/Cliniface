# 6.0.3
- Fixed reversed palpebral fissure inclination angle.
- Fixed negated mandibular contour angle.
- Fixed save icon not greying out after initial save to 3DF.
- Removed old mesh references from non-enabled radial selector.
- Added nasal root depth measurement including Farkas 1981 statistics.
- Added HPO terms depressed and prominent nasal bridge.

# 6.0.2
- Removed help button from Preferences dialog due to modality clash bug.
- Fixed Windows repeatedly asking to authorise clinifaceUpdater on update.
- Fixed inability to update Linux AppImage version.
- Fixed broken CLI commands (thanks Harry).

# 6.0.1
- Added mentolabial angle.
- Reduced render window font sizes.
- Updated some landmark descriptions.
- Improved landmark placement accuracy.
- Small reduction in landmark dot radius.
- Added visualisations of surface differences.
- Capped maximum number of legend value labels.
- Surface visualisations now toggle on and off.
- Default opacity overlap reduction fixed at 0.01.
- Removed plain vector (vertex normal) visualisation.
- Dialogs try not to overlap the main viewer on opening.
- Overlap opacity now updates correctly when moving models.
- Removed wireframe and outline visualisations from toolbar.
- Degree of opacity can now be set per individual model view.
- Thumbnails of non-textured models are now properly lit.
- Each measurement now stores its own default statistics.
- Statistic auto-matching now applies individually to each model.
- Missing landmarks from earlier file versions automatically set.
- Camera distance, focus, and direction shown in status when moving camera.
- Metric charts now shows measurements out of range and with a warning.
- Camera orientation and distance now maintained upon changing focal point.
- Changed camera orientation action to three step alignment process.
- Corrected scaling of embedded models in generated reports.
- Camera synchronisation now enabled by default.
- Changed caption fonts for better readability.
- Increased size of undo/redo stack.
- Added custom report page dimensions.
- Added 5 degree stepped camera horizontal and vertical rotation.
- Model opacity now allowed to go all the way to zero (transparent).
- Removed filtering of metrics when selecting HPO in HPO terms dialog.
- Removed preference for changing amount of opacity change on overlap.
- Removed selection by clicking the views - use the dropdown instead.
- Removed preference for setting maximum number of models to load.
- Removed memory usage tracking bar from the preferences dialog.
- Removed preference for setting smoothing parameter.
- Removed parameters that configure face registration.
- Removed vertex / polygon identifier labels actions.
- Removed preference for model auto-focus on select.
- Removed radial selection tool from toolbar.
- Removed Cliniface logo from main toolbar.
- Reformatted PDF report template and removed orientation axes.
- Reformatted model properties dialog.
- Moved manifold removal actions to model properties dialog.
- Removed change to bounding box format depending on landmarks.
- Capture and birth date set as earliest file time for new (non-3DF) images.
- Added static 2D image of model to generated reports.
- Reformatted layout of assessment information dialog.
- Restricted maximum number of models within a viewer to two.
- Restricted maximum number of loaded models to two.
- Restricted number of views of a model to two.
- Updated portable MiKTeX for Windows.
- Added advisory on actions to take after face detection failure.
- Lengthy report generation now always happens in background.
- Fixed crash on failure to detect face.
- Fixed rare hang on align and detect actions.
- Fixed occasional crash when using the calliper tool.
- Fixed opacity widget not updating on model reselect.
- Fixed non-symmetric colour mapping for even colour counts.
- Fixed rare crash on completion of some asynchronous actions.
- Fixed inverted outer canthal nasal angle measurement.
- Fixed incorrect risizing of side viewer when merging model view.
- Fixed disappearing "Not stated" label in Assessment Info dialog.
- Fixed incorrect alignment of coregistered mask on model load.
- Fixed refreshing of clipping planes when zooming in deeply.
- Fixed landmark and path handle rescaling on model scale.
- Fixed inability to immediately produce PDFs from some models.
- Fixed jittery positioning of radial selection handle.
- Fixed spelling error (thanks Tracey ;D).
- Improved responsiveness and efficiency.
- Updated documentation.

# 5.3.2
- Added and updated mouth landmarks.
- Updated definition of the lip circularity measurement.
- Updated FASD plugin documentation.

# 5.3.1
- Updated FASD plugin documentation.

# 5.3.0
- Added inferior facial angle measurements.
- Added mid-labial inferius/superius landmarks.
- Changed lip circularity measurement to use new landmarks.
- Fixed bug when writing OBJs when facets don't have texture coordinates.

# 5.2.2
- Updating now consolidates patches from multiple versions.
- Patching of the AppImage version for Linux now possible.
- Updating now asks for elevated privileges if necessary.

# 5.2.1 (removed from release)
- Some formatting changes to the updates dialog.
- Removed donation links in dialogs.
- Fixed TLS not initialized bug.

# 5.2.0
## New Features
- Double left click now sets the camera focus.
- Camera focus can now be set on the surface behind other graphical elements.
- View presets use current camera distance for orientation only resetting distance if orientation matches preset.
- Direct distance size components shown in calliper measurement readout.
- Landmarks and calliper measurement visualisations are now toggled for all views of a single model.
- Face becomes semi-transparent when moving calliper measurements or hovering over the depth handle.
- Measurement precision for the calliper handle caption now matches the onscreen measurements readout (2 d.p.).
- Calliper measurements can now be added at the position of landmarks.
- New calliper measurements can now be added using the `C` key.
- Simplified calliper measurement renaming dialog and limited labels to be no more than 19 characters.
- Checking and advisement of new available versions at startup.
- Fixed crash if model closed while a calliper measurement or a landmark is being moved.
- Fixed crash if exporting a model to CSV format when no measurements yet recorded.
- Fixed landmarks visualisation "shudder" when actor interaction mode is active and a landmark is moved.
- Fixed the ability to set the visibility of individual landmarks from the landmarks dialog.
- Fixed landmark labels sometimes not disappearing after moving the mouse off them.
- Fixed redo inconsistencies when adjusting a sequence of different paths/landmarks.
- Fixed ability to remove individual calliper measurements using the `DEL` key.
- Fixed missing tooltips on opacity and visualisation adjustment spin boxes.
- Centred help browser in screen upon first opening.
- Right clicking a calliper measurement handle no longer activates reposition mode.
- Removed ability to change measurement units in preferences (require use of model rescaling tool).
- Undo/redo events for landmarks and calliper measurement now activate corresponding visualisations.
- Disabled ability to select a different model while in model interaction mode.
- Disabled broken 3MF file format importer.
- Updating a calliper measurement's depth handle no longer recalculates the entire path.
- Removed redundant processing of interaction events for improved performance.
- [Linux] Fixed file drag and drop.
- [Windows] Fixed calliper measurement handle detachment from cursor after initial add.

# 5.1.2
- Fixed report layout from letter to A4.
- Fixed issue where PDF reports not generating if spaces in username folder (Windows).
- Changed display formatting of bilateral measurements to indicate right, left, and mean values.

# 5.1.1
- Rebuilt against latest versions of third-party libraries.
- Models without texture now appear properly in 3D model embedded PDFs.
- Assessment info dialog now appears upon face redetection if DoB same as capture date.
- Fixed scaling issues on high DPI screens under Windows (blurry elements to be fixed).
- Fixed bug where L/R z-score for bilateral measurements was incorrectly swapped.
- Added export to CSV and set as default filetype when choosing to export data.
- Reformatted and added hyperlinks to HPO terms in assessment info dialog.
- Added hyperlinks in generated PDFs to online HPO terms database.
- Fixed occasional bad formatting of charts on generated reports.
- Added ability to generate reports straight from the command line.
- Added and improved report scripting functions when listing notable morphology.
- Included version number in reports.
- 3DF files now store textures in JPEG format for reduced size.
- Renamed "dysmorphology" summary to "morphology" summary.
- Disabled merging of statistics from different sources.
- Reworked command line options and interface.
- Added new help documentation on command line interface and metadata exporting.
- Changed to simple flat green/blue Cliniface logo throughout.
- Fixed ability to change subject's DoB to be ahead of image capture date.
- Removed "Aggregated" category from ethnicity selection menus.
- Added ability for users to record image and subject reference in the 3DF.
- Fixed possibility to select empty statistics in measurements dialog.
- Reduced OpenGL version on Windows to 2.0 (from 3.2) and added override environment variables.

# 5.0.3
- Updated mask registration applied to example images.
- Changed filenames in 3DF format and now saving mask to PLY format (format remains backwards compatible).
- Fixed inability to load models with file paths having non-latin characters.

# 5.0.2
- Fixed crash after attempting to read in corrupt model file.
- Fixed landmark label remaining visible sometimes after moving a landmark.
- Fixed status message not disappearing after some actions.
- Status bar messages now appear on request (repaint forced).
- Fixed landmarks dialog closing after redoing landmark detection.
- Camera oriented to front view now on opening landmarks dialog - not on closing.
- Face detection / mask registering can now be reperformed without having to select at least one landmark to overwrite.
- Changed default parameters for non-rigid registration.
- Added itemised list of questions in help FAQs.
- Added some explicit draw calls to ensure graphical rendering is GPU driven and improve responsiveness.
- Fixed bug where model could be saved out of alignment so would be in incorrect alignment when loaded back in.

# 5.0.1
- Fixed crash on exit when camera in parallel projection and viewer synchronised mode.
- Camera synchronisation now works as expected with cameras in parallel projection mode.
- When matching atypical measurements, selecting a term in the HPO Browser now also selects it in the Measurements Browser.
- Status message of long running asynchronous actions is refreshed every second.
- Model opacity now resets to 1.0 after all models closed.
- Closing Landmarks Dialog no longer resets Assessments Information if changes not applied.
- Prevented setting of image capture date in Assessments Information to be before DoB.
- Improved detection of initial (rigid) model alignment.
- Updated help docs.
- Set default face extraction radius to 100 instead of 110 mm.
- Changed command line options.
- Fixed undo from face detection not restoring model back to previous position.

# 5.0.0
- Compare the same facial measurements between two or more different faces.
- Facial detection / landmark placement now performed using non-rigid registration.
- Coregistred template mask saved out to 3DF file and added ability to flip between mapped mask and original surface.
- Using interpolated lighting (default) on model surfaces. New preferences option to toggle off/on.
- Measurements now dynamically update as landmarks change.
- Changed scalar mapping visualisations to use interpolated vertex texture mapping.
- Number of scalar mapping colour bands fixed to only even numbers.
- Consolidated HPO terms Telecanthus (HP:0000506) with Dystopia Canthorum which uses W index to establish presence of trait.
- Added new preference options.
- Exported OBJ no longer exports an MTL file if the model has no texture information.
- Save now forwards to SaveAs after metadata (e.g. landmarks, paths) added before already being saved in 3DF format.
- Model deselect removed - a model is always selected if one is loaded (remove bounding boxes via preferences for screenshots etc).
- With multiple open views of a model, clicking empty part of a different viewer now automatically selects the correspoding view.
- Changed default installation directory on Windows to "\Users\user_name\AppData\Local\Programs\Cliniface"
- Transforming the selected model into standard position/centre no longer changes the camera distance.
- Smooth now warns user if facial landmarks are present prior to working.
- Added action to fix inconsistent face (triangle) normals on meshes.
- Added greater transparency to landmark / path handles for more accurate surface placement.
- Added option to show all landmark labels (irrespective of whichever landmark cursor is over).
- Added STL filetype export support.
- Swapped left/right profile face labels (now with regard to subject).
- Removed X,Y,Z rotation icons from toolbar.
- Camera position now changed on model rescaling.
- Added camera views from beneath and above.
- Added anatomical planes (median, transverse, frontal).
- Undo/redo now also changes the camera view in the selected viewer for actions that also update the camera.
- Moved to OpenCV 4.1.1 entailing a new set of HaarCascades features for improved initial 2D eye/face detection.
- Moved to Eigen backend for all linear algebra (and integration with new r3d libraries).
- Added capacity for actions to scrap last created undo state if found not to be needed.
- Added delete all paths for current assessment.
- Post detection/correspondence mapping, correspondence mesh can be exported.
- Fixed busy cursor disappearing event if asynchronous actions are still working.
- Fixed black surface showing on models without texture after reflecting.
- Fixed formatting of memory usage bar in preferences dialog.
- Fixed 'close all' action not closing all open models.

# 4.1.0
- Updated desktop categories for Linux AppImage version.
- Modified implementation of ICP - now faster and more accurate.
- Link to examples in $HOME/.cliniface directory now set dynamically (removed on exit).
- Increased size of world axes grid.
- Changed to OpenCV 3.2 on Windows for consistency with Linux (also on 3.2).
- Path handle, landmarks, and radial selection handle coordinates now shown in status bar.
- Improved robustness of initial face/landmark detection.
- Model movement now propagates between viewers (when multiple views of the same model are open).
- Model views now prefer to place themselves on the centre viewer after merging or closing models.
- Different colours for polygon and vertex labels.
- Modified Windows installer to allow existing installations to be overwritten.
- Fixed undo/redo bug where state restoration was being performed on the selected model.
- Fixed incomplete restoration of model geometry after undo.
- Fixed render flicker when opening context menu.

# 4.0.3
- Cliniface now stores all user data in the .cliniface directory inside user's home directory.
- Created secondary location from which plugins can be loaded (from $HOME/.cliniface/plugins).
- Symlink to examples now always located at $HOME/.cliniface/examples.
- Renamed core platform plugin 'symmetry' to 'asymmetry'.
- Removed setting of unused mask path string in preferences.
- Update plugins documentation.

# 4.0.2
- Added polygon labels visualisation.
- Some minor bug fixes concerning the accuracy of surface point placement.

# 4.0.1
- Fixed corruption of scalar map upon mirror reflecting the model.
- Fixed rounding errors in surface point finding and point triangle membership routines.
- Fixed jittery movement of radial region selection handle.
- Manual distance measurements fail less often.
- Fixed creation of holes / edges when making "half" faces.
- Undo/redo tooltips update with the details of the action to be undone/redone.
- Ensured Plugins section of help documentation is available even without any plugins loaded.

# 4.0.0
- Model cleaning now less destructive - only removes unused vertices.
- New manifold discovery and per manifold boundary finding algorithms.
- Improved hole filling algorithm.
- Parameterised per model limit on maximum number of 2D triangulated manifolds (top N in terms of polygon count).
- Selection bounding cuboid now shows only a single cuboid for each model containing all manifolds.
- Added function to remove the selected manifold.
- Added function to remove all other manifolds except the selected one.
- Added function to export metadata in XML or JSON formats without saving to 3DF.
- Added function to import metadata from XML or JSON format.
- Added Undo/Redo functionality for certain actions.
- Added function to create left/right symmetric faces.
- Rotation and reflection actions now behave with respect to detected face if landmarks present.
- Transform to standard position is now clearer about when transforming to saved position or detected position.
- Model bounding box remains at an angle when model is not in its saved/detected orientation.
- Added more keyboard shortcuts for common actions.
- Added context sensitive help text to actions.
- Added process memory usage gauge in preferences dialog.
- Removed action that removed non-face components.
- Reordered context menu actions.
- Can now toggle surface/texture on regardless of whether texture available for model.
- Manifold outlines now shown with different colours.
- Added units to world axes (mm).
- Added centre object action.
- More efficient action event handling.
- Faster background processing of surface metrics.
- Added vertex labels visualisation.
- Fixed crash when detecting a face when another face present in same viewer without texture.
- More use of background processing for actions entailing extensive geometry changes.
- Renamed some actions for clarity.
- Changed smooth action icon for better distinction with crop action (scalpel).
- Removed set min/max scalar mapping colours from visualisation toolbar.
- Removed model remeshing from toolbar.
- Removed per viewer duplicated camera reset and screenshot buttons (functionality available from views menu).
- Set screenshot action in view menu to be per viewer and only enabled if a model selected in the viewer.
- Long standing hole filling bug causing crash fixed.
- Camera now automatically focus on model upon selection; toggle action added to view/camera menu to toggle.
- Added dropdown selection boxes for models beneath viewers as an alternative to clicking the models to select.
- Added support for exporting to PLY format.
- Removed generic model remesh (edge subdivision) action.
- Consolidated scalar visualisations toolbar into main visualisation toolbar for purposes of toggling toolbar on/off.
- Added a visualisation to display the vertex label IDs (useful when exploring the behaviour of algorithms on geometry).
- Fixed setting of near clipping plane to prevent small objects from being clipped when camera is close to them.
- New Model Properties dialog (accessed via File manu) giving details of model geometry.
- Removed vertex/polygon counts from "Scan Info" dialog (now renamed "Image Info").
- Enabled min/max scalar mapping and number of colours widgets only when a scalar visualisation set.
- Added confirmation check before removing manifolds.
- Asymmetry visualisation now available for models without landmarks (models with landmarks still use these to define orientation).
- Set default visible range of asymmetry visualisation to +/-5mm with available range still +/-10mm.
- Changed camera/model move toggle icon.
- New model resize action.
- New preferences dialog in file menu.
- Reformatted HPO terms dialog.
- Reordered placement of model transform tools in menu and on toolbar.
- Checkable metric visualisation action now shows/hides associated metric, phenotype, and chart dialogs.
- Antialiasing now turned on by default.
- Increased initial window layout dimensions.
- Improved dialog scaling on high DPI displays under Windows.
- Changed Image Info icon.
- Inner sphere handling when performing loop select now has a fixed apparent scale.
- Streamlined resource locking behaviour for more efficient concurrent processing.
- Surface mapping visualisations can now be toggled on and off while remaining exclusive.
- Camera synchronisation per viewer now relative to camera movements on viewer that the user is interacting with.
- Radius change amount now as a proportion of the initial model size.
- New option to use white rendering background.
- Moved anti-aliasing to preferences.
- Consolidated camera actions and moved as menu to main menu bar.
- Separated out Tools menu into Transform and Geometry menu.
- Moved toggle camera/actor interaction to camera menu.
- Model transformation actions now reset viewer interaction mode to camera on completion.
- Wait/busy cursor shown for long running operations.
- Status bar now shows running / finished actions.
- Major backend changes to storage efficiency of model vertices.
- Backface culling now applies to all views of all models in the selected model's viewer.
- Discovered phenotypic traits now shown in the assessment information dialog.
- Added model age in years and months to report template.
- Provided ethnic group categories from the Australian Bureau of Statistics.
- Provided separate entries for maternal and paternal ethnicity.
- New help system with documentation to match online.
- New sex/ethnicity matching for growth curve statistics against subject.
- Multiple assessments per file for several sets of landmarks / paths / notes.
- Automatic combining of growth curve stats for different ethnicities.
- 
- Reformatted PDF report elements.
- Added "What's This" icon in help menu.
- Fixed model lighting when switching between textured and scalar mapping modes.
- Fixed inability to reload an existing renamed 3DF file.
- Fixed bug causing occasional crash on model close due to hanging pointer within model entry/exit interactor on closed view.
- Fixed bug where model age not calculated to image capture but rather to the current date.
- Fixed bug on report generation with unsanitized text.

# 3.2.1
- Cliniface configuration now persists in user's home directory.
- Reduced initial height of UI window for lower resolution monitors.
- Removed fixing of triangle normals on model load by AssImp since broken in some cases.
- Moved to AssImp version 4.1.0.
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

# 3.2.0
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

# 3.1.0
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

# 3.0.6
- Remeshing algorithm changes to improve polygon integration with unchanged polygons.

# 3.0.5
- Fix backface culling (and opacity) now persist between model resets.
- Fix orientation of polygon normals on hole filling and remeshing now matches adjacent polygons.
- Fix hole filling no longer an available action when multiple components showing and hole boundaries on any of them.
- Fix queued actions in FaceActionManager are now executed when previous queued actions aren't entered.
- Fix outline views of holes/boundaries now complete loop (RVTK issue).

# 3.0.4
- Fixed viewer rendering upon orientation change action.
- New hole filling algorithm.
- New vertex remeshing algorithm.
- Very large reduction in size of texmfs for PDF reporting.
- Fix remeshing no longer causes texture map to disappear due to model copy.

# 3.0.3
- Fixed referencing of facetools and pdf generation folders relative to application directory.
- Updated location of GitHub repository to "Cliniface/releases" page.
- Renamed PDF export action and placed button on toolbar.
- Fix Models are now viewed properly after loading from command line.

# 3.0.2
- Moved face centre to mean of eye centres and nose tip for more central placement.
- Backface culling now applies to all model views in directly selected viewers.
- Modified remesh to silently fail instead of crashing on problem.
- Face profile buttons act over all viewers if "triple camera" selected.
- Offline installer created.
- Fix model filepath not updating on selection change (regression)
- Fix fixed vertex key mapping in ObjModel.

# 3.0.1
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

# 3.0.0
- Version provided to Cathryn Poulton for Milan demo.

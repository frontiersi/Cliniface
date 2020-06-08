Incorporate converter from TSB format.

Add a timeout for long running actions.

Allow addition of PDF files to 3DF.

Add in weight, height, head circumference and medical conditions, surgery, treatment to Assessment Info.

Make external link to HPO online an always visible button.

When selecting between faces with atypical measures, ensure when switching back
and the measurement displays as selected in the table again (after having
disappeared) that the visualisation is also set again.

When auto-extracting after landmarks present, ensure the extraction region encompasses them all.

Need differences summary for two/three faces.

Fix holes using 3DMM after detection.

Ensure subject marker is shown on graph even if out of range.

In Assessment Information, add fields for Weight, Height, and Head Circumference. Also show calculated age.

On closure of landmarks dialog, DONT EMIT EVENT (no alignment happening now).

Add option to delete .cliniface directory when uninstalling (Windows).

Fix colour banding properly to always have symmetric value either side of 0.

Check and update command line options.

Check and change tooltips to be multiline.

Allow models without textures to be embedded as PDFs in reports (possibly generate faux texture?).

Generating average face from a cohort.

When slicing a r3d::Mesh, holes sometimes - investigate.

Check inconsistent triangle normals - returning twisted when it shouldn't?

Fix checking if file can be read with weird characters in filepath (see Lenka's teeth models for example).

Make pupil detection look for darkest area within small region.

After every action, check model state against initial undo state (if present) and
only set as unsaved if there's a mismatch.

Allow for different opacity values for views in the same viewer.

Fix line drawing using local curvature based plane slicing.

Add "Add Plugin" button to plugins dialog.


Check stats:
1) Interpupillary
2) Nasal Root Width (obtain)

Import metadata directly from a 3DF.

Import/export from/to 3MF format (3D printing format).


## INDIA NEW HPO TERMS:
### Interlandmark based measurements:
* Ear, long
* Ear, short
* Columella, broad
* Columella, high insertion
* Columella, low insertion
* Columella, low-hanging

### Angles / relational to other landmarks:
* Ear, posterior angulation, increased
* Ear, low set
* Mouth, downturned corners of
* Mouth upturned corners of
* Chin, short
* Chin, tall

### Other Measurements / Metrics
* Prognathism
* Retrognathia
* Malar flattening/ malar prominence
* Midface prominence/ midface retrusion
* Hairline, low anterior/ hairline, high, anterior (need to sue texture and anatomical data for this)
* Philtrum deep
* Cornea diameter (if poss)
* Volume difference


#### Preferences
* Default syndrome(s)
* Default HPOs(s) and/or metrics

#### Actions
* Note taking.
* Freehand selection tool.
* Copy/paste models.

### Geometry
* Raise / sink face locally.
* Localised smoothing (use radial select tool)?
* Modify vertex positions by clicking and dragging.
* Flip normals for specific polygons.
* Interactive rotation about specific axes.

#### Visualisation
* Show scalar value in status bar and an arrow on legend as mouse moved over surface
* Vector map visualisation for differences
* Visualise difference of curvature maps


#### File I/O
* Extend 3DF file format to allow other kinds of models (hand, foot).
* Record movie from viewer for mpeg export.
* Recent files list.


#### Documentation
* Error logging.


#### Framework
* Require semaphore on actions for locked model.


#### Menus/Dialogs
* Check/download updates via Updates menu option?
* Offer to send error/output log as email attachment?


#### Preferences
* Different colours for landmarks between faces.
* Edit caliper handle size (fixed size vs scaled size).


#### Data
* Search within cohorts
  - For a particular HPO term or metric
  - Get the N most similar faces to the current from a cohort.

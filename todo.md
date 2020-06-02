Check Cliniface install procedure on Windows for locations etc.

Change tooltips to be multiline.

Allow models without textures to be embedded as PDFs in reports (possibly generate faux texture?).

Implement coregistration for generating average face.

When merging growth data, need to take in account the number of observations in each age bracket.

Add in surface distances for interlandmark distances (use face orientation to define plane intersection).

Check inconsistent triangle normals - returning twisted when it shouldn't?

Fix checking if file can be read with weird characters in filepath (see Lenka's teeth models for example).

Make pupil detection look for darkest area within small region.

After every action, check model state against initial undo state (if present) and
only set as unsaved if there's a mismatch.

Allow for different opacity values for views in the same viewer.

Update Windows uninstaller with checkbox to remove ${HOME}/.cliniface directory (uncheck by default).

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

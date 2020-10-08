# Next
- Ensure Windows build environment uses OpenSSL from NON-QT bundled version.
- Ensure all demographic mismatches are displayed in Cliniface.
- Add in notes about the Aboriginal reference range.
- Review and implement Harry's work.
- Generate average face from a user specified cohort (directory) - but check Harry's work.
- Need differences summary for two/three faces.
- Include stats from Kate (email).
## Search within cohorts (directory?)
  - For a particular HPO term or metric
  - Get the N most similar faces to the current from a cohort.

# Known bugs
- Rebuild normal vectors after recalculation of curvature.
- Help links to calliper measurement not going to that section in metrics.html.
- Load double clicked files in existing instance of Cliniface if available.
- Fix alignment after making half faces.
- Colour scaling for even number of labels is wrong.
- Landmark labels need to account for individual visibility.
- Coronal plane too far forward (though not used right now so fine to leave).
## Possible (investigate)
- When selecting between faces with atypical measures, ensure when switching back
  and the measurement displays as selected in the table again (after having
  disappeared) that the visualisation is also set (updated) again.
- When slicing a r3d::Mesh, holes sometimes.
- Check inconsistent triangle normals - returning twisted when it shouldn't?

# Framework / Interaction / Actions
- Add timeout for long running actions.
- Change status text when toggling "texture" (surface) on with model with no texture.
- When holding ALT and making camera movements, lock movement to the axes.
- On pressing shortcut key, cause action icon to depress for the duration of the action.
- After action, check model against initial state and set unsaved only if mismatch.
- Interactive rotation about specific axes.
- Flip normals for specific polygons.
- Modify vertex positions by clicking and dragging.
- Raise / sink mesh locally.
- Localised smoothing.
- Copy/paste models (for what?)
- Freehand selection tool (use for what?)

# Visualisations
- Show scalar value in status bar and an arrow on legend as mouse moved over surface.
- Fix colour banding properly to always have symmetric value either side of 0.
- Allow different opacity values for views in the same viewer.
- Vector map visualisation for differences.
- Visualise difference of curvature maps.
- Different colours for landmarks between faces in same viewer.
- Edit calliper handle size (fixed size vs scaled size)?
- Check if on high DPI display and resize graphical widgets (e.g. handles) accordingly.

# Documentation
- Check and change tooltips to be multiline (see Preferences).
- Error logging.
- Offer to send error/output log as email attachment?

# Face and Landmark Detection, Metric / Phenotype Analysis and Reporting
- Fix holes using 3DMM after detection.
- When opening landmarks dialog, place it to one side of the main window.
- Make pupil detection look for darkest area within small region.
- Ensure subject marker is shown on graph even if out of range.
- Adding notes to specific regions of the face (useful?)
- Add fields for Weight, Height, Head Circumference, and free text medical history (conditions, treatment).
- Display calculated age.
- In HPO dialog, make external link to HPO online a button.
- User specified default syndrome(s).
- Default HPOs(s) and/or metrics.
## Anthropometrics
- Check interpupillary
- Obtain Nasal Root Width
- Change Ptosis criteria to be below 2 standard deviations instead of 3 mm.
### INDIA NEW HPO TERMS:
#### Interlandmark based measurements:
- Ear, long
- Ear, short
- Columella, broad
- Columella, high insertion
- Columella, low insertion
- Columella, low-hanging
#### Angles / relational to other landmarks:
- Ear, posterior angulation, increased
- Ear, low set
- Mouth, downturned corners of
- Mouth upturned corners of
- Chin, short
- Chin, tall
#### Other Measurements / Metrics
- Facial depth
- Prognathism
- Retrognathia
- Malar flattening/ malar prominence
- Midface prominence/ midface retrusion
- Hairline, low anterior/ hairline, high, anterior (need to sue texture and anatomical data for this)
- Philtrum deep
- Cornea diameter (if poss)
- Volume difference

# File Import / Export
- Incorporate converter from TSB format.
- Allow addition of attachments to 3DF.
- Import metadata directly from a 3DF.
- Import/export from/to 3MF format (3D printing format).
- Extend 3DF file format to allow other kinds of models (hand, foot, jaw).
- Recent files list.
- Record movie from viewer for mpeg export.

# Auto-Update
- Make work on Windows if user installs to root access locations.
- Allow Linux users to download parts to temp file system then repackage if necessary.
- Read changes.md file separately to provide information about version.
- Have client download only the between version patch zips needed and consolidate before updating.

# Mesh Representation
- Improve smoothing to remove spikes entirely (e.g. eyelashes).
- Fix hole filling to reorganise mesh.
- Add function to redistribute vertices in mesh.
- Line drawing using local curvature based plane slicing.

# Installation / Plugins
- Add option to delete .cliniface directory when uninstalling (Windows).
- Add "Add Plugin" button to plugins dialog.

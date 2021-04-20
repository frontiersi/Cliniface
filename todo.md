# Version 6.0.2 todo
- On graphs, if two subjects loaded, show selected (filled in circle) and other (circle)
- Rename Extract Facial Region to Extract Region if the radial selection tool is active.
- Add docs about the reference ranges.

# Next
- TSB to 3DF convert from commandline
- Deal with different AM masks
- Fix loading 3DFs with different stored masks
- Use normals to decide which corresponding parts of the target and source surfaces to map.
- Centralise demographic mismatches and display everywhere (including report generation).
- Show scalar value in status bar and an arrow on legend as mouse moves over surface.
- Auto-restart after network update.
- Move discard non-face manifolds to menu.
- Manual entry of measurements.
- Review and implement Harry's work.
- Generate average face from a user specified cohort (directory) - but check Harry's work.
- Include stats from Kate (email).
- Term and synonym search for HPO terms.
## Search within cohorts (directory?)
  - For a particular HPO term or metric
  - Get the N most similar faces to the current from a cohort.

# Knownbugs
- In docs, links to anchor ids from other pages not working.
- Load double clicked files in existing instance of Cliniface if available.
- Landmark labels need to account for individual visibility.
- Coronal plane too far forward (though not used right now so fine to leave).
## Possible (investigate)
- When slicing a r3d::Mesh, holes sometimes.
- Check inconsistent triangle normals - returning twisted when it shouldn't?

# Framework / Interaction / Actions
- Add timeout for long running actions.
- Change status text when toggling "texture" (surface) on with model with no texture.
- When holding ALT and making camera movements, lock movement to the axes.
- After action, check model against initial state and set unsaved only if mismatch.
- Interactive rotation about specific axes.
- Normal flip for specific polygons.
- Modify vertex positions by clicking and dragging.
- Localised smoothing/cropping tool.

# Visualisations
- Edit calliper handle size (fixed size vs scaled size)?
- Check if on high DPI display and resize graphical widgets (e.g. handles) accordingly.

# Documentation
- Check and change tooltips to be multiline (see Preferences).
- Offer to send error/output log as email attachment?
- Error logging.

# Face and Landmark Detection, Metric / Phenotype Analysis and Reporting
- Fix holes using 3DMM after detection.
- Make pupil detection look for darkest area within small region.
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
- Import from VRML format files.
- Allow addition of attachments to 3DF.
- Import metadata directly from a 3DF.
- Import/export from/to 3MF format (3D printing format).
- Extend 3DF file format to allow other kinds of models (hand, foot, jaw).
- Recent files list.
- Record movie from viewer for mpeg export.

# Mesh Representation
- Improve smoothing to remove spikes entirely (e.g. eyelashes).
- Fix hole filling to reorganise mesh.
- Add function to redistribute vertices in mesh.
- Line drawing using local curvature based plane slicing.

# Installation / Plugins
- Add option to delete .cliniface directory when uninstalling (Windows).
- Add "Add Plugin" button to plugins dialog.

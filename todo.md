# TODO next (remove as implemented and add under corresponding version in changes.md)
Database:
- Centre handle on user measurements maps incorrectly on a freshly loaded
  model that isn't in aligned position. Probably not taking into account model transform matrix.
- Add action to copy over subject data from other (loaded) image.

## Version 7.0.0.20220131 (Prague)
- Local 3DF file indexing based on root location for 3DF storage
- When making symmetric face (and no landmarks), mirror points through plane
- Increase allowed length of user measurement name
- User measurements dialog to toggle visibility and update names
- When hovering over a user measurement, make it appear differently to the others

## Version 7.1.0.20220210
- Volumetric measurement
- Asymmetry report improvements
- Average face generation from selected 3DFs
- Landmark selection / hiding / flagging
- Manual entry of facial measurements
- User surface area measurement
- Fix calliper handle snap range after switching views when in orthographic view
- Fix rotations from transform menu not sticking after changing view.

## Version 8.0.0.20220501 (Babyface)
- Mahalanobis distance visualisation for facial cohorts
- Variation from cohort report
- Cohort mask averaging
- Improved coregistration

# Unassigned

## User measurements
- Allow mid path handle to be defined out of plane.
- Show angle at mid path handle as angles projected into standard planes.
- Add volumetric measurement.
- Copy user measurements to second loaded face or as a batch operation to a bunch in a directory.
- Make face transparent when hovering over handles (not just when moving them).
- If handle is over a landmark, show the landmark name too.
- Edit calliper handle size (fixed size vs scaled size)?
- Batch copying of user measurements to a bunch of other 3DF files based on AM correspondence.

## Camera
- When updating field of view (preferences) maintain apparent camera distance.
- Upon view change, update the camera/model move interaction action.
- When holding ALT and making camera movements, lock movement to the axes.
- Interactive rotation about specific axes?

## File I/O
- Generate average face from a user specified cohort (directory) - but check Harry's work.
- Search within cohorts (directory?)
    - For a particular HPO term or metric
    - Get the N most similar faces to the current from a cohort.
- After action, check model against initial state and set unsaved only if mismatch.
- When opening a registered file type by double clicking, open in existing
  Cliniface instance if found or a new instance if two models are already loaded.
- Add file menu to open an example from application directory.
- Add in Recent files menu.
- Add source and Z-scores for measurements in CSV file.
- On saving to a non-metadata format (ply etc), erase all metadata (since warning about it).
- Fix loading 3DFs with different stored masks
- Import from VRML format files.
- Allow addition of attachments to 3DF.
- Import metadata directly from a 3DF.
- Import/export from/to 3MF format (3D printing format).
- Extend 3DF file format to allow other kinds of models (hand, foot, jaw).
- Copy in action name for undo/redo states since action name may change.
- Record movie from viewer for mpeg export.

## Visualisations / UI
- Fix crash when centre model on right click calliper handle.
- Mahalanobis distance visualisation.
- Change status text when toggling "texture" (surface) on with model with no texture.
- When loading file with measurements showing - ensure 0.99 opacity set.
- Show landmark labels for the selected measurement in measurement mode.
- Allow rotation by arbitrary amount (make interactive too).
- Coronal plane too far forward (though not used right now so fine to leave).
- Show scalar value in status bar and an arrow on legend as mouse moves over surface.
- Check if on high DPI display and resize graphical widgets (e.g. handles) accordingly.

## Landmarks
- When moving landmarks, ensure bilateral landmarks remain on the correct face half.
- Show landmark labels for the left side of the face on the left side of the cursor.
- Customise which landmarks to use and flag reliability.
- Landmark labels need to account for individual visibility.
- Add info about where definitions of landmarks come from (Elements of Morphology).

## Stats and standard measurements
- Allow for manual entry/adjustment of facial measurements.
- Add measurement information about source, use etc in Lua definitions.
- On graphs, if two subjects loaded, show selected (filled in circle) and other (circle).
- Centralise demographic mismatches and display everywhere (including report generation).
- Add fields for Weight, Height, Head Circumference, and free text medical history (conditions, treatment).
- Add MONDO disease codes.
- Display subject's calculated age.
- Include Japanese stats from Kate (email).
- In HPO dialog, make external link to HPO online a button.
- On press HPO browser button, raise as top window.
- Allow user to specify default syndrome(s).
- Flag detected phenotypic traits not associated with the selected syndrome(s).
### Anthropometrics
- Check interpupillary
- Obtain Nasal Root Width
- Change Ptosis criteria to be below 2 standard deviations instead of 3 mm.
- Ear, long
- Ear, short
- Columella, broad
- Columella, high insertion
- Columella, low insertion
- Columella, low-hanging
- Ear, posterior angulation, increased
- Ear, low set
- Mouth, downturned corners of
- Mouth upturned corners of
- Chin, short
- Chin, tall
- Facial depth
- Prognathism
- Retrognathia
- Malar flattening/ malar prominence
- Midface prominence/ midface retrusion
- Hairline, low anterior/ hairline, high, anterior (need to sue texture and anatomical data for this)
- Philtrum depth
- Cornea diameter (if possible)
- Volume difference

## AM co-registration
- Full head models don't work with the alignment algorithm.
- Resize the AM mask prior to fitting based on estimation of eye distance found.
- Use the 2D facial detection method to discover basic facial features (Python process).
- Use vertex normals to decide corresponding parts of the target and source surfaces to map.
- Fix holes using 3DMM after detection.
- Patient equivalent face generation

## Mesh/Model Representation
- Add action to remove texture.
- Fix right symmetric face gap centreline gap.
- Add action to Centre Model on current focus.
- Rename Extract Facial Region to Extract Region if the radial selection tool is active.
- Check inconsistent triangle normals - returning twisted when it shouldn't? Not sure...
- Improve smoothing to remove spikes entirely (e.g. eyelashes).
- Fix hole filling to reorganise mesh.
- Add function to redistribute mesh vertices.
- Normal flip for specific polygons.
- Modify vertex positions by clicking and dragging.
- Localised smoothing/cropping tool.

## Report generation
- Remove unused files to fix PDF generation issue (wrong pdflatex being referenced?).
- Exported 3D models in PDF should use much lower geometry meshes for reduced file size.
- Improve asymmetry report and ensure documentation present.
- Direct production of PDFs

## Documentation
- Add info about the reference ranges to the documentation.
- Add info about generating the facial comparison report.
- In docs, links to anchor ids from other pages not working sometimes - BUG
- Check and change tooltips to be multiline (see Preferences).
- Log errors/issues and offer to email.
- Language Translations,Lyfe Languages

## Build / Installation / Plugins / Updates
- Fix auto-update of FASD module analysis after changing measurements (e.g. in-plane PFL)
- Auto-restart after network update.
- Add option to delete .cliniface directory when uninstalling (Windows).
- Add "Add Plugin" button to plugins dialog.
- Allow plugin dialog to be resized.
- Remove diagnostic references from FASD plugin (see Eamon's email).
- Ensure dependent libraries of IDTFConverter are included (libpng12).

## Miscellaneous
- Timeout for long running actions.

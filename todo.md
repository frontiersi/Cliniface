Change the Windows Installer to remove old installations first (need to look at registry?).

Make scaling procrustes and implement coregistration for generating average face.

Export non-textured models into reports using a coloured surface.

Check stats:
1) Interpupillary
2) Nasal Root Width (obtain)
3) Palpebral Fissure Width (Hall et al. redo and get N).

Generalised Procrustes and Non-Rigid Surface Registration.

Rendering issue when switching manifolds in model properties.

Import metadata directly from a 3DF.

Copy/paste.

Recent files list.

Preferences dialog:
    Default syndrome(s)
    Default HPOs(s) and/or metrics

Custom report element placement (text boxes, images etc).


#### Installer
Add shortcut to bin/cliniface in installation directory
Make installer uninstall previous version first - also offer option to remove preferences file.
Windows: warn if trying to install in Program Files directory since theres an issue with running pdflatex from here.


#### Actions
- With 3VID button checked, visualisations should be applied in all viewers (not just current).
- Toggle camera prop in 3D view.
- Note taking.
- Selection tool with dragable boundary / freehand select.


### Geometry
- Interactive rotation about specific axes.
- Flip normals for specific polygons.
- Raise / sink face locally.
- Localised smoothing (use radial select tool)?
- Modify vertex positions by clicking and dragging.


#### Visualisations
- Vector map visualisation for differences
- Visualise difference of curvature maps.


#### File I/O
- Extend 3DF file format to allow other kinds of models (hand, foot).
- Record movie from viewer for mpeg export.
- Save camera position (make preference option to set camera on load).


#### Documentation
- Error logging.


#### Framework
- Require semaphore on actions for locked model.
- Click in a different viewer to select that view of the current model (don't have to click on model itself).
- Highlight component under mouse cursor (thicker boundary?).
- Disable only required type of actions (ChangeEvent based) before an action starts.


#### Metrics
- Extract & visualise FASD:
 - Philtrum
 - Upper lip

- Ear length (if poss)
- Philtrum length (if poss)
- Mouth length (if poss)
- Cornea diameter (if poss)
- Palpebral fissure angle (if poss)
- Ear angle  (if poss)

- Angular measure between points.
- Z-scores for relevent HPO terms.
- Better surface metric for point to point distances.

- Volume difference


#### Landmarks
- Manually specify nose-tip and eye centres (draw triangle?)


#### Menus/Dialogs
- Check/download updates via Updates menu option?
- Offer to send error/output log as email attachment?


#### Preferences
- Different colours for landmarks between faces.
- Edit caliper handle size (fixed size vs scaled size).
- Show line paths as A\* path or geodesics.


#### Data
- Search within cohorts
    - For a particular HPO term or metric
    - Get the N most similar faces to the current from a cohort.

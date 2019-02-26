#### 3.2.1 features:

Allow only certain reports to be generated based on selected model state
    (need a general report for non-face models).

Preferences dialog:
    Per metric source,
    Default syndrome
    Default HPOs
    Default visualised metrics
    Landmark size

Model select from file menu.

Make scaling procrustes and implement coregistration for generating average face.

Windows: show warning if try to install in Program Files directory since
there's an issue with running pdflatex from here.

Check self for updates.

#### 4.0.0 features:
Move to different model selection backend (extensive changes to actions).





#### Actions
- With 3VID button checked, visualisations should be applied in all viewers (not just current).
- Manual orientation setting from current camera view.
- Manually specify nose-tip and eye centres (draw triangle?)
- Interactive rotation about specific axes.
- Undo/redo.
- Show face orientation.
- Toggle camera prop in 3D view.
- Remove component under mouse cursor (context menu).
- Remove all other components (context menu).
- Note taking.
- Model repair - flip normals for specific polygons.
- Move/copy actions on context menu.
- Localised smoothing (use radial select tool)?
- Selection tool with dragable boundary / freehand select.
- Update detection tooltip to advise that detection performed from camera location.
- Record movie of viewer for export to mpeg.
- Visualise difference of curvature maps.

- Raise / sink face locally.

- Extend 3DF file format to allow other kinds of models (hand, foot).


#### Documentation
- Online.
- In-app.
- Error logging.


#### Framework
- Require semaphore on actions for locked model.
- Click in a different viewer to select that view of the current model (don't have to click on model itself).
- Dropdown lists to aid model selection.
- Highlight component under mouse cursor (thicker boundary).
- Disable only required type of actions (ChangeEvent based) before an action starts.
- Static functions for some actions (e.g. view wide backface culling).


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


### Report


#### Landmarks
- Dropdown select name on add landmark?
- Make face landmark detection points non-deletable?


#### Menus/Dialogs
- Model info dialog.
- Landmarks dialog needed?
- HPO metrics dialog.
- Check/download updates via Updates menu option?
- Offer to send error/output log as email attachment?


#### Preferences
- Different colours for landmarks between faces.
- Edit caliper handle size (fixed size vs scaled size).
- Show line paths as A\* path or geodesics.

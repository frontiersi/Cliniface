### 4.0.1
- When model slicing, extra vertices are added resulting in the creation of holes / extra manifolds (use patch.ply).
- ObjModelSurfaceCurveFinder fails sometimes (use patch2.3df).
- ObjModelSurfacePlanePathFinder doesn't work properly.

### 4.0.0
- vtkCubeAxesActor fails to initialise properly on viewers at start.
- Occasional graphical corruption on scalar views when copying/moving between viewers (fix by toggling scalar mapping view).
- When merging view from centre to left/right AND no other views in centre, then remove view to centre and close left/right viewer.
- pdflatex doesn't run if installed in system dir (permissions?)
- Widget bounding box around scalar mapping key appears sometimes.

### 3.1.0 (all to be reconfirmed)
- Copying when radial select visualisation active doesn't apply visualisation correctly.
- Metric caption (bottom left) disappearing when moving between landmarks and metric visualisations.
- If surface curvature visualisation applied, then reapply after model surface changed (e.g. by smooth).


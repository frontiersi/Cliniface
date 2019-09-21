- Weird rendering issue on right/left viewers sometimes (Windows only?).
  Could be a VTK bug - investigate with later versions of VTK when available.

- ObjModelSurfaceCurveFinder fails sometimes (use patch2.3df).
- ObjModelSurfacePlanePathFinder doesn't work properly.

- vtkCubeAxesActor fails to initialise properly on viewers at start (or small text).
- Occasional graphical corruption on scalar views when copying/moving between viewers (fix by toggling scalar mapping view).
- pdflatex doesn't run if installed in system dir (permissions?)
- Widget bounding box around scalar mapping key appears sometimes (confirm).

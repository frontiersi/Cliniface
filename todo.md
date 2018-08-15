### TODO

- Extract & visualise FASD:
 - Palpebral Fissure Length (PFL)
 - Philtrum
 - Upper lip

- Documentation
 - Online documentation.

- Z-scores for relevent HPO terms.

- Asymmetric colour mapping with vertical pencil at nose-tip.
- Show legends toggle (for supported visualisations).

- Click in a different viewer to select that view of the current model (don't have to click on model itself).

- Model transform actions (rotate 90 degrees about three different axis)
- Action to flip model normals.
- Model info dialog.
- Edit caliper handle size (fixed size vs scaled size) (option).
- Different colours for landmarks between faces (option).
- Dropdown select name on add landmark?
- Make face landmark detection points non-deletable?
- Dropdown lists to aid model selection.
- Landmarks dialog?
- Manually specify nose-tip and eye centres.
- HPO metrics dialog.
- Note taking.
- Angular measure between points.
- Better surface metric for point to point distances.
- Show line paths as A\* path or geodesics (option).
- Disable only required type of actions (ChangeEvent based) before an action starts.
- Highlight component under mouse cursor (thicker boundary).
- Action to remove component under mouse cursor (context).
- Action to remove all other components (context).
- Undo/redo.
- Align CPD (Coherent Point Drift).
- Change ICP align to use common landmarks on selected model (and radius?).
- Selection tool with dragable boundary.
- Move/copy actions on context menu.
- Localised smoothing (use radial select tool)?
- Check/download updates via Updates menu option?

- File error logging
 - Offer to send log as email attachment?

### BUGS
1. Hole filling issue on Bella (around neck)?
2. Not entering map surface data sometimes on model load.
3. Fix creation / I/O of orientation data (always calculate from landmarks?)
4. Path caption line visible when path editor enabled and no paths present.
5. Remeshed polygons have alternately incorrect normals (problem seen with backface culling enabled).
7. Camera placement when exporting PDF.
8. Double clicking on freshly detected face when repositioning radial select causes
   centre point to be placed behind the surface.
9. UI not updating after curvature mapping complete.
10. When updating the model with radial selection visible, a purge of the radial selection visualisation
    causes the visualisation elements to be repositioned. Need to move the radial selection into a
    secondary data structure associated with the FaceModel.
    FaceModel should have ownership of a set of hash mapped data structures useful to other algorithms.
11. Bug in ObjModelRegionSelector causing radius to update upon setCentre.
12. After crop, show radial select.
13. Crop should only retain a single component?
14. Show landmarks at load only if present.
15. Fix hole filling so doesn't generate non-flat vertices requiring subsequent clean.
16. Regularize progression of progress bar across actions that use it.
17. Stack interactors.
18. Restore legends if moved outside viewer.

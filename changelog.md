[https://cliniface.org/download/cliniface-tool/](https://cliniface.org/download/cliniface-tool/)

#### 3.0.5.180729
- Bug fix: backface culling (and opacity) now persist between model resets.
- Bug fix: orientation of polygon normals on hole filling and remeshing now matches adjacent polygons.
- Bug fix: hole filling no longer an available action when multiple components showing and hole boundaries on any of them.
- Bug fix: queued actions in FaceActionManager are now executed when previous queued actions aren't entered.
- Bug fix: outline views of holes/boundaries now complete loop (bug in RVTK).

#### 3.0.4.180724
- Fixed viewer rendering upon orientation change action.
- New hole filling algorithm (RFeatures).
- New vertex remeshing algorithm (RFeatures).
- Very large reduction in size of texmfs for PDF reporting.
- Bug fix: remeshing no longer causes texture map to disappear due to model copy.

#### 3.0.3.180718
- Fixed referencing of facetools and pdf generation folders relative to application directory.
- Updated location of GitHub repository to "Cliniface/releases" page.
- Renamed PDF export action and placed button on toolbar.
- Bug fix: Models are now viewed properly after loading from command line.

#### 3.0.2.180717
- Moved face centre to mean of eye centres and nose tip for more central placement.
- Backface culling now applies to all model views in directly selected viewers.
- Modified remesh to silently fail instead of crashing on problem.
- Face profile buttons act over all viewers if "triple camera" selected.
- Offline installer created.
- Bug fix: model filepath not updating on selection change (regression)
- Bug fix: fixed vertex key mapping (RFeatures::ObjModel).

#### 3.0.1.180712
- Camera/model movement interaction switching.
- One and two button mouse camera/model interaction.
- Orthographic projection mode.
- World coordinates visualisation toggle.
- Fixed model transform to standard position.
- Re-worked caliper path measurement tool.
- Caliper path info written to 3DF.
- Landmark add/delete/rename included on context menu.
- Path add/delete/rename included on context menu.
- Added option to synchronise camera movement between viewers.
- Model opacity select and auto semi-opacity on overlap.
- Backface culling toggle.
- Region select visualisation now using RFeatures::ObjModelRegionSelector.
- Left and right face profile view actions.
- Action to align faces across landmarks with same names.
- Installation/updates using Qt based maintenance tool.
- Landmark and path actors mapped for fast (hash) lookup.
- Interaction data change events handled separately from Actions.

#### 3.0.0.180612
- Version provided to Cathryn Poulton for Milan demo.

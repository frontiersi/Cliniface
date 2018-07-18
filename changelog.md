[https://cliniface.org/download/cliniface-tool/](https://cliniface.org/download/cliniface-tool/)

#### 3.0.3.180718
- Fixed referencing of facetools and pdf generation folders relative to application directory.
- Fixed viewing of model after load from command line.
- Updated location of GitHub repository to "Cliniface/releases" page.
- Renamed PDF export action and placed button on toolbar.

#### 3.0.2.180717
- Moved face centre to mean of eye centres and nose tip for more central placement.
- Made backface culling apply to all model views in directly selected viewers.
- Modified remesh to silently fail instead of crashing on problem.
- Bug fix: model filepath not updating on selection change (regression)
- Bug fix: fixed vertex key mapping bug (RFeatures::ObjModel).
- Face profile buttons act over all viewers if "triple camera" selected.
- Offline installer created.

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

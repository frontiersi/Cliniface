- When detecting face and landmarks but when no landmarks selected to move, display a different msg at the end.

- Landmark labels still not disappearing sometimes (e.g. stomion inferius and superius)

- Some undo/redo errors with respect to the below bug.

- If radial selection tool has been used but is now off, and no landmarks, then auto
face extraction still uses the definition given by the radial selection tool.

- Load double clicked files in existing instance of Cliniface if available.

- Fix alignment after making half faces.

- Colour scaling for even number of labels is wrong.

- Landmark labels need to account for individual visibility.

- Help links to calliper measurement not going to that section in metrics.html.

- Rebuild normal vectors after recalculation of curvature.

- Coronal plane too far forward (though not used right now so fine to leave).
- Calliper path/handle setting/drawing inaccuracy issues on some rare edge cases (test/complex_18vtxs.obj)

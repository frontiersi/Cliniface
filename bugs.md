- Fix marquee mode (camera movement to cancel).
- Fix alignment after making half faces.
- Fix docs for FASD plugin, file format.

- Colour scaling for even number of labels is wrong.

- Metric visualisations don't update after undo (because the details aren't stored as
  part of the metric measurements - needs a bit of an architectural fix).

- Landmark labels need to account for individual visibility.

- When following a link to a different section in a different page of the help,
  the page is always shown at the top instead of at the id position.
- Rebuild normal vectors after recalculation of curvature.

- Lighting issues after inverting normals (apparent on models without texture).
- Restoring individual landmark from context menu does not update until dialog closed.
- Coronal plane too far forward (though not used right now so fine to leave).
- Calliper path/handle setting/drawing inaccuracy issues on some rare edge cases (test/complex_18vtxs.obj)
- Test ability to run pdflatex if Cliniface installed in system dir on Windows (permissions?)

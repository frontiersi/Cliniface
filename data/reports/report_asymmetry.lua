report = {
    name = "Asymmetry (All Dimensions)",
    title = "Asymmetry (All Dimensions)",
    twoModels = false,

    isAvailable = function( fm)
        acols = getSelectedViewActiveColoursName()
        return acols == "Asymmetry All Dimensions (mm)"
    end,

    setContent = function( fm)
        x0 = 0.03
        x1 = 0.53
        y = 0.09
        w = 0.46

        addScanInfo( Box.new( x0, y, w, 0.1), fm)
        addFigure(   Box.new( x0, 0.18, w, 0.4), fm)

        addNotes(    Box.new( x1, y, w, 0.07), fm)
        addSelectedColourMapFigure( Box.new( x1, 0.18, w, 0.4))
        addSelectedColourMapLegend( Box.new( x1, 0.59, w, 0.025))  -- legend about 7.5 mm high
        addText( Box.new( x1, 0.59 + 0.03, w, 0.03), "millimetres", true)
    end
}

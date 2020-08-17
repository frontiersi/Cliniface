report = {
    name = "TEST",
    title = "TEST Report",

    isAvailable = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addStartColumn()
        addScanInfo()
        addFigure( 100, 150, "")
        addNotes()
        addEndColumn()

        addStartColumn()

        -- Source references for footnotes
        fnmks = footnoteIndices( {10,8,44})
        addPhenotypicTraits(0, 13)
        addEndColumn()

        addFootnoteSources( {10,8,44})
    end
}

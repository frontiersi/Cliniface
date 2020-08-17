report = {
    name = "Facial Morphology Summary",
    title = "Facial Morphology Report",

    isAvailable = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addStartColumn()
        addScanInfo()
        addEndColumn()

        addStartColumn()
        addNotes()
        addEndColumn()

        addFigure( 130, 140, "")

        addStartColumn()
        hasMoreTraits = addPhenotypicTraits(0, 13)
        addEndColumn()

        if hasMoreTraits then
            addStartColumn()
            addPhenotypicTraits(13, 26)
            addEndColumn()
        end
    end
}

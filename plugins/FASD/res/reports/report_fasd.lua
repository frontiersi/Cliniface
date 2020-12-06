report = {
    name = "Fetal Alcohol Spectrum Disorder (FASD)",
    title = "Fetal Alcohol Spectrum Disorder Report",

    isAvailable = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addStartColumn()
        addScanInfo()
        addFigure( 100, 110, "")
        addNotes()
        addLineBreak()
        addPhenotypicTraits( 0, 13)
        addEndColumn()

        addStartColumn()
        addGrowthCurvesChart( 10, 0, 1)
        addFASDTable()
        addEndColumn()

        addFASDFootnotes()
    end
}

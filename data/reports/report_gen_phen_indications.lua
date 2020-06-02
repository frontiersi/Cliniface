report = {
    name = "Facial Dysmorphology Summary",
    title = "Facial Dysmorphology Summary",

    available = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addScanInfo()
        addFigure( 150, 160, "")
        addPhenotypicVariationsList()
    end
}

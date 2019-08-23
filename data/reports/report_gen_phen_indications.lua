report = {
    name = "Discovered Atypical Phenotypic Variations",
    title = "Discovered Atypical Phenotypic Variations",

    available = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addScanInfo()
        addFigure( 150, 160, "")
        addPhenotypicVariationsList()
    end
}

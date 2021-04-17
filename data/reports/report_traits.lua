report = {
    name = "Noted Phenotypic Traits",
    title = "Noted Phenotypic Traits",
    twoModels = false,

    isAvailable = function( fm0)
        return fm0:hasLandmarks()
    end,

    setContent = function( fm)
        addScanInfo( Box.new( 0.03, 0.09, 0.46, 0.07), fm)
        addNotes(    Box.new( 0.53, 0.09, 0.46, 0.07), fm)
        addFigure(   Box.new( 0.10, 0.17, 0.80, 0.50), fm)

        hasMoreTraits = addPhenotypicTraits( Box.new( 0.03, 0.69, 0.46, 0.3), fm, 0, 13)
        if hasMoreTraits then
            addPhenotypicTraits( Box.new( 0.53, 0.69, 0.46, 0.3), fm, 13, 26)
        end
    end
}

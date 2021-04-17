report = {
    name = "Phenotypic Facial Comparison",
    title = "Phenotypic Facial Comparison",
    twoModels = true,

    isAvailable = function( fm0, fm1)
        return fm0:hasMask() and fm0:maskHash() == fm1:maskHash()
    end,

    setContent = function( fm0, fm1)
        addScanInfo(         Box.new( 0.03, 0.09, 0.46, 0.1), fm0)
        addFigure(           Box.new( 0.03, 0.18, 0.46, 0.4), fm0)
        addPhenotypicTraits( Box.new( 0.03, 0.61, 0.46, 0.3), fm0, 0, 26)

        addScanInfo(         Box.new( 0.53, 0.09, 0.46, 0.1), fm1)
        addFigure(           Box.new( 0.53, 0.18, 0.46, 0.4), fm1)
        addPhenotypicTraits( Box.new( 0.53, 0.61, 0.46, 0.3), fm1, 0, 26)
    end
}

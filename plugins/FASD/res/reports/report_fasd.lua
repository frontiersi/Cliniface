report = {
    name = "Fetal Alcohol Spectrum Disorder (FASD)",
    title = "Fetal Alcohol Spectrum Disorder Sentinel Features",
    twoModels = false,

    isAvailable = function( fm)
        return fm:hasLandmarks()
    end,

    setContent = function( fm)
        -- Left column
        addScanInfo(          Box.new( 0.03, 0.09, 0.46, 0.07), fm)
        addFigure(            Box.new( 0.03, 0.17, 0.46, 0.42), fm)
        addPhenotypicTraits(  Box.new( 0.03, 0.60, 0.46, 0.20), fm, 0, 10)

        -- Right column
        addNotes(             Box.new( 0.53, 0.09, 0.46, 0.07), fm)
        addChart(             Box.new( 0.53, 0.17, 0.46, 0.28), fm, 10, 0, 1)
        addFASDTable(         Box.new( 0.53, 0.53, 0.46, 0.40), fm)

        addFASDFootnotes( Box.new( 0.05, 0.95, 0.9, 0.05), fm)
    end
}

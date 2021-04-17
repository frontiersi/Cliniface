report = {
    name = "3D Model Only",
    title = "3D Model",
    twoModels = false,

    isAvailable = function()
        return true
    end,

    setContent = function( fm)
        addFigure( Box.new( 0.1, 0.15, 0.8, 0.7), fm, "Select profile from `Views' menu")
    end
}

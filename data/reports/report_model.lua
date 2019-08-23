report = {
    name = "PDF Embedded 3D Model",
    title = "3D Image",

    available = function( fm)
        return true
    end,

    addContent = function( fm)
        addFigure( 160, 210, "Select profile from `Views' menu")
    end
}

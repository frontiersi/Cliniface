report = {
    name = "PDF Embedded 3D Model (req. Adobe Reader)",
    title = "3D Image",

    available = function( m)    -- m is FaceModel
        return true
    end,

    content = function( r, m)   -- r is Report, m is FaceModel

        s = r:makeFigure( 160, 210, "Select profile from `Views' menu")

        return s
    end
}

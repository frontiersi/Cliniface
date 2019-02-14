report = {
    name = "Manipulable 3D Model (req. Adobe Reader)",
    title = "Manipulable 3D Image",
    content = function( r, m)   -- r is Report, m is FaceModel

        s = r:makeFigure( 160, 210, "Select profile from `Views' menu")

        return s
    end
}

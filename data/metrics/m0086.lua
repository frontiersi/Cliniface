mc = {
    id = 86,
    name = "Palpebral Fissure Inclination",
    desc = "Angle subtended at endocanthion by a line segment between exocanthion and the opposite endocanthion.",
    regn = "Orbital",
    ndps = 1,
    prms = {
        catg = "Angle",
        norm = "z",
        rmks = "",  -- Remarks about how this metric is defined.
        blat = true,
        pnts = {
                { -- A dimension of the metric
                    -- The number of points expected for each dimension is defined by the category.
                    -- When defining the order of the points for an angle, the right hand rule
                    -- should be respected where the middle point defines where the angle is
                    -- subtended, the first point is the index finger, and the third point the
                    -- second finger. This will define an angle measurement in the positive half
                    -- of the space divided by the plane in which the norm is coincident.
                    -- There is no need to define points for the subject's left lateral.
                    { "Lex"},
                    { "Len"},
                    { "100Lex", "011Len"},    -- Each point can be the average of multiple landmarks.
                }
               }
            }
    }


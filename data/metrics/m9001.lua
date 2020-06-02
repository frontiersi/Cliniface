mc = {
    id = 9001,
    name = "TEST DEPTH",
    desc = "TEST DEPTH",
    regn = "Test",
    ndps = 3,
    prms = {
        catg = "Depth",
        norm = "z",
        rmks = "",
        blat = false,
        pnts = {
                {   -- Dimension 1
                    { "Lal"}, -- endpoint 1 of the line segment
                    { "Ral"}, -- endpoint 2 of the line segment
                    { "Lal", "Ral"},    -- Position along line to measure depth to surface at
                },
            }   -- pnts
        }   -- prms
}


mc = {
    id = 2001,
    name = "Philtral Depth",
    desc = "Depth measured at three spaced points in the midline between the philtral ridges.",
    regn = "Lip",
    ndps = 1,
    prms = {
        catg = "Depth",
        norm = "z",
        rmks = 'Based on <a href="https://doi.org/10.1093/alcalc/agy088">Blanck-Lubarsch M, Dirksen D, Feldmann R, et al. (2019)</a>',
        blat = false,
        pnts = {
                {   -- Dimension 1
                    { "3sn", "ls"},    -- Position along line to measure depth to surface at
                    { "ls"}, -- endpoint 1 of the line segment
                    { "sn"}, -- endpoint 2 of the line segment
                },
                {   -- Dimension 2
                    { "sn", "ls"},  -- Measure from halfway along the line
                    { "ls"}, -- endpoint 1 of the line segment
                    { "sn"}, -- endpoint 2 of the line segment
                },
                {   -- Dimension 3
                    { "sn", "3ls"},
                    { "ls"}, -- endpoint 1 of the line segment
                    { "sn"}, -- endpoint 2 of the line segment
                },
            }   -- pnts
        }   -- prms
}


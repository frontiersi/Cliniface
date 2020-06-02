mc = {
    id = 2002,
    name = "Orbital Protrusion",
    desc = "Degree of the protrusion of the eye (exophthalmos).",
    regn = "Orbital",
    ndps = 1,
    prms = {
        catg = "Depth",
        norm = "z",
        rmks = "Protrusion is normally measured from the bony ridge of the lateral margin of the orbital to the most anterior margin of the cornea. However, most 3D image capture systems do not adequately capture the shape of the eye and so in Cliniface the front of the cornea is estimated using a combination of other landmarks around the orbital. In addition, since the bony ridge of the orbital cannot be palpated for, Cliniface uses a combination of landmarks to define this including the frontotemporale and superciliare lateralis landmarks for lateral position, the superior/inferior position by exocanthian and palpebral inferius, and anteroir/posterior position by palpebral inferius, mid-infraorbital, and palpebral superius. For these reasons, this measurement should be considered less reliable than others produced by Cliniface.",
        blat = true,
        pnts = {
                {   -- Dimension 1
                    { "100Lscl", "100Lft", "010Lex", "011Lpi", "001Lmio", "001Lps"},
                },
            }   -- pnts
        }   -- prms
}


hpo = {
    id = 581,
    name = "Blepharophimosis",
    region = "Orbital",
    synonyms = {"Narrow palpebral fissures", "Decreased width and height of palpebral fissures"},
    criteria = "Objectively determined as both the length and the width (horizontal and vertical distance) of the palpebral fissures being more than 2 standard deviations below their means. Subjectively determined as a fixed reduction in the vertical distance between the upper and lower eyelids together with short palpebral fissures.",
    remarks = "Severely shortened palpebral fissures cannot be widely separated, actively or passively. This is a bundled term and is often associated with Epicanthus Inversus.",
    metrics = {10,87},
    determine = function( msf, msl, msr)
        hlz = msl:get(10):zscore(0)
        hrz = msr:get(10):zscore(0)
        h = (hlz + hrz) < -4.0  -- Short mean horizontal palpebral fissures (below 2 SD)
        vlz = msl:get(87):zscore(0)
        vrz = msr:get(87):zscore(0)
        v = (vlz + vrz) < -4.0  -- Short mean vertical palpebral fissues (below 2 SD)
        return h and v          -- Both factors present
    end
}


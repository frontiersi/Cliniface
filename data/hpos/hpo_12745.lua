hpo = {
    id = 12745,
    name = "Short Palpebral Fissures",
    region = "Orbital",
    synonyms = {"Short opening between the eyelids", "Decreased width of palpebral fissure"},
    criteria = "Objectively evaluated as the distance between the medial and lateral canthi being more than 2 standard deviations below the mean. Subjectively evaluated as an apparently reduced length of the palpebral fissures.",
    remarks = "Severe shortening of the palpebral fissures leads to Blepharophimosis. Decreased palpebral fissure lengths may be accompanied by a decreased palpebral fissure width (vertical distance between the upper and lower eyelids).",
    metrics = {10},
    determine = function( msf, msl, msr)
        lz = msl:get(10):zscore(0)
        rz = msr:get(10):zscore(0)
        return (lz + rz) < -4.0
    end
}

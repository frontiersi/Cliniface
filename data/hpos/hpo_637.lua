hpo = {
    id = 637,
    name = "Long Palpebral Fissures",
    region = "Orbital",
    synonyms = {"Wide palpebral fissures", "Long opening between the eyelids", "Broad opening between the eyelids", "Wide opening between the eyelids"},
    criteria = "Objectively determined as the distance between the medial and lateral canthi being more than 2 standard deviations above the mean. Subjectively evaluated as an apparently increased length of the palpebral fissures.",
    remarks = "The term 'Wide palpebral fissure' is discouraged due to the ambiguity of which dimension it refers to (horizontal length vs vertical height).",
    metrics = {10},
    determine = function( msf, msl, msr)
        lz = msl:get(10):zscore(0)
        rz = msr:get(10):zscore(0)
        v = (lz + rz) > 4.0
        return v
    end
}

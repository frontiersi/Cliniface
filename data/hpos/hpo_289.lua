hpo = {
    id = 289,
    name = "Broad Philtrum",
    region = "Lip",
    synonyms = {"Increased transverse dimension/breadth of philtrum", "Wide philtrum"},
    criteria = "Objectively determined as the distance between the philtral ridges (measured just above the vermilion border) being more than 2 standard deviations above the mean. Subjectively determined as an apparently increased distance between the philtrum ridges.",
    remarks = "Measuring philtral width is much less accurate than measuring length. A broad philtrum may be associated with reduced edge prominences, a shallow groove, or a Smooth Philtrum. It may also be found with a broad nasal septum.",
    metrics = {40},
    determine = function( msf, msl, msr)
        return msf:get(40):zscore(0) > 2.0
    end
}


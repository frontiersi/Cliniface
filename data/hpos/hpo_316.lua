hpo = {
    id = 316,
    name = "Hypertelorism",
    region = "Orbital",
    synonyms = {"Excessive orbital separation", "Widely spaced eyes", "Increased distance between eye sockets", "Increased interpupillary distance", "Wide-set eyes", "Ocular hypertelorism"},
    criteria = "Objectively evaluated as both the interpupillary and intercanthal distances being more than 2 standard deviations above their means. Subjectively evaluated as the appearance of increased interpupillary distance or widely spaced eyes.",
    remarks = "It is important to distinguish between truly increased interpupillary distance, and apparently increased spacing caused by Telecanthus.",
    metrics = {85,8},
    determine = function( age, msf, msl, msr)
        pd = msf:metric(85):zscore(age, 0) > 2.0
        ic = msf:metric(8):zscore(age, 0) > 2.0
        return pd and ic
    end
}


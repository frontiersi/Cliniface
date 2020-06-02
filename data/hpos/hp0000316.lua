hpo = {
    id = 316,
    name = "Hypertelorism",
    region = "Orbital",
    synonyms = {"Excessive orbital separation", "Widely spaced eyes", "Increased distance between eye sockets", "Increased interpupillary distance", "Wide-set eyes", "Ocular hypertelorism"},
    ocrit = "Both the interpupillary and intercanthal distances are more than 2 standard deviations above their means.",
    scrit = "Appearance of increased interpupillary distance or widely spaced eyes.",
    remarks = "It is important to distinguish between truly increased interpupillary distance, and apparently increased spacing caused by Telecanthus.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000316">Full details and related diseases.</a>'},
    metrics = {85,8},
    determine = function( age, msf, msl, msr)
        pd = msf:metric(85):zscore(age, 0) > 2.0
        ic = msf:metric(8):zscore(age, 0) > 2.0
        return pd and ic
    end
}


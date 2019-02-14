hpo = {
    id = 506,
    name = "Telecanthus",
    region = "Orbital",
    synonyms = {"Dystopia canthorum", "Corners of eye widely separated", "Increased distance between medial canthi", "Increased intercanthal distance"},
    criteria = "Objectively evaluated as the inner canthal distance being more than 2 standard deviations above the mean, the interpupillary distance no more than 2 standard deviations from the mean, and the inner canthal distance greater than the subnasal width. Subjectively evaluated as an apparently increased distance between the inner canthi.",
    remarks = "Intercanthal distance varies between ethnic groups. Telecanthus can be present with or without Hypertelorism.",
    metrics = {85;8;46},
    determine = function( msf, msl, msr)
        pd = msf:get(85):zscore(0) <= 2.0    -- Interpupillary distance <= 2SD above mean
        ic = msf:get(8):zscore(0) > 2.0      -- Intercanthal distance > 2SD above mean
        -- IC > subnasal width
        swv = msf:get(46):value(0)
        icv = msf:get(8):value(0)
        return pd and ic and (icv > swv)
    end
}



hpo = {
    id = 506,
    name = "Telecanthus",
    region = "Orbital",
    synonyms = {"Dystopia canthorum", "Corners of eye widely separated", "Increased distance between medial canthi", "Increased intercanthal distance"},
    criteria = "Objectively evaluated as the inner canthal distance being more than 2 standard deviations above the mean, the interpupillary distance no more than 2 standard deviations from the mean, and the inner canthal distance greater than the subnasal width. Subjectively evaluated as an apparently increased distance between the inner canthi.",
    remarks = "Intercanthal distance varies between ethnic groups. Telecanthus can be present with or without Hypertelorism.",
    metrics = {85;8;46},
    determine = function( age, msf, msl, msr)
        pd = msf:metric(85):zscore(age, 0) <= 2.0    -- Interpupillary distance <= 2SD above mean
        ic = msf:metric(8):zscore(age, 0) > 2.0      -- Intercanthal distance > 2SD above mean
        -- IC > subnasal width
        swv = msf:metric(46):value(0)
        icv = msf:metric(8):value(0)
        return pd and ic and (icv > swv)
    end
}



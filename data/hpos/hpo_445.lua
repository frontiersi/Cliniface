hpo = {
    id = 445,
    name = "Wide Nose",
    region = "Nasal",
    synonyms = {"Broad nose", "Increased nasal width/breadth"},
    criteria = "Objectively determined as the interalar distance being more than 2 standard deviations above the mean. Subjectively determined as an apparently increased width of the nasal base and alae.",
    remarks = [[Nasal width is defined as the distance between the most lateral aspects of the alae nasi. 'Wide nose' is also used by many to denote increases in not only the interalar distance, but also the nasal tip, ridge and bridge. As such it is a bundled term. EOM does not favour this more general use of the term and instead, recommend describing the individual components.]],
    metrics = {46;47},
    determine = function( msf, msl, msr)
        return msf:get(47):zscore(0) > 2.0
    end
}

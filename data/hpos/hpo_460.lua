hpo = {
    id = 460,
    name = "Narrow Nose",
    region = "Nasal",
    synonyms = {"Thin nose", "Decreased nasal width/breadth"},
    criteria = "Objectively determined as the interalar distance being more than 2 standard deviations below the mean. Subjectively determined as an apparently decreased width of the nasal base and alae.",
    remarks = [[Nasal width is defined as the distance between the most lateral aspects of the alae nasi. 'Narrow nose' is also used by many to denote decreases in not only the interalar distance, but also the nasal tip, ridge and bridge. As such it is a bundled term. EOM does not favour this more general use of the term and instead, recommend describing the individual components.]],
    metrics = {46;47},
    determine = function( msf, msl, msr)
        return msf:get(47):zscore(0) < -2.0
    end
}

hpo = {
    id = 460,
    name = "Nose, Narrow",
    region = "Nasal",
    synonyms = {"Thin nose", "Decreased nasal width/breadth"},
    ocrit = "Interalar distance is more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased width of the nasal base and alae.",
    remarks = [[Nasal width is defined as the distance between the most lateral aspects of the alae nasi. 'Narrow nose' is also used by many to denote decreases in not only the interalar distance, but also the nasal tip, ridge and bridge. As such it is a bundled term. EOM does not favour this more general use of the term and instead, recommend describing the individual components.]],
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000460">Full details and related diseases.</a>'},
    metrics = {46;47},
    determine = function( age, msf, msl, msr)
        return msf:metric(47):zscore(age, 0) < -2.0
    end
}

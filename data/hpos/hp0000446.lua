hpo = {
    id = 446,
    name = "Nasal Bridge, Narrow",
    region = "Nasal",
    synonyms = {"Pinched nasal bridge"; "Narrow nasal bridge"; "Narrow nasal root"; "Thin nasal bridge"},
    ocrit = "Both the nasal width and the innercanthal width are more than 2 standard deviations below the mean.",
    scrit = "Decreased width of the bony bridge of the nose.",
    remarks = "Nasal bridge may narrow with age.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000446">Full details and related diseases.</a>'},
    metrics = {8;47},
    determine = function( age, msf, msl, msr)
        ic = msf:metric(8):zscore( age, 0)
        nw = msf:metric(47):zscore( age, 0)
        return ic < -2.0 and nw < -2.0
    end
}

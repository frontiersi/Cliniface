hpo = {
    id = 431,
    name = "Nasal Bridge, Wide",
    region = "Nasal",
    synonyms = {"Broadened nasal bridge";"Increased breadth of nasal bridge";"Increased width of bridge of nose";"Broad nasal root";"Widened nasal bridge";"Broad flat nasal bridge";"Broad nasal bridge"},
    ocrit = "Both the nasal width and the innercanthal width are more than 2 standard deviations above the mean.",
    scrit = "Increased breadth of the nasal bridge and with it the nasal root.",
    remarks = "A wide nasal bridge can either be prominent or depressed, and may also narrow with age. A wide nasal bridge should be distinguished from Telecanthus and Hypertelorism.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000431">Full details and related diseases.</a>'},
    metrics = {8;47},
    determine = function( age, msf, msl, msr)
        ic = msf:metric(8):zscore( age, 0)
        nw = msf:metric(47):zscore( age, 0)
        return ic > 2.0 and nw > 2.0
    end
}

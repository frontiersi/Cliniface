hpo = {
    id = 289,
    name = "Philtrum, Broad",
    region = "Lip",
    synonyms = {"Increased transverse dimension/breadth of philtrum", "Wide philtrum"},
    ocrit = "Distance between the philtral ridges (measured just above the vermilion border) is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased distance between the philtrum ridges.",
    remarks = "Measuring philtral width is much less accurate than measuring length. A broad philtrum may be associated with reduced edge prominences, a shallow groove, or a Smooth Philtrum. It may also be found with a broad nasal septum.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000289">Full details and related diseases.</a>'},
    metrics = {40},
    determine = function( age, msf, msl, msr)
        return msf:metric(40):zscore(age, 0) > 2.0
    end
}


hpo = {
    id = 2002,
    name = "Philtrum, Deep",
    region = "Lip",
    synonyms = {"Depressed philtrum"; "Pronounced/Prominent philtrum"; "Increased depth of philtrum"},
    ocrit = "Philtral depth more than 2 standard deviations above the mean at <i>any</i> of the three measured points of depth in the midline of the philtrum.",
    scrit = "Accentuated, prominent philtral ridges giving rise to an exaggerated groove in the midline between the nasal base and upper vermilion border.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0002002">Full details and related diseases.</a>'},
    metrics = {2001},
    determine = function( age, msf, msl, msr)
        return msf:metric(2001):zscore(age, 0) > 2.0 or
               msf:metric(2001):zscore(age, 1) > 2.0 or
               msf:metric(2001):zscore(age, 2) > 2.0
    end
}



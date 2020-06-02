hpo = {
    id = 154,
    name = "Mouth, Wide",
    region = "Lip",
    synonyms = {"Broad mouth", "Large mouth", "Large oral aperture", "Macrostomia"},
    ocrit = "The distance between the commissures of the mouth is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased width of the oral aperture.",
    remarks = "Mouth width varies with facial movement and must be assessed with a relaxed or neutral face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000154">Full details and related diseases.</a>'},
    metrics = {39},
    determine = function( age, msf, msl, msr)
        return msf:metric(39):zscore(age, 0) > 2.0
    end
}


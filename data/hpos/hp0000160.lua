hpo = {
    id = 160,
    name = "Mouth, Narrow",
    region = "Lip",
    synonyms = {"Small oral aperture", "Small mouth", "Microstomia"},
    ocrit = "The distance between the commissures of the mouth is more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased width of the oral aperture.",
    remarks = "Mouth width varies with facial movement and must be assessed with a relaxed or neutral face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000160">Full details and related diseases.</a>'},
    metrics = {39},
    determine = function( age, msf, msl, msr)
        return msf:metric(39):zscore(age, 0) < -2.0
    end
}


hpo = {
    id = 582,
    name = "Palpebral Fissure, Upslanted",
    region = "Orbital",
    synonyms = {"Upward slanting of the opening between the eyelids", "Upward slanting of palpebral fissures", "Mongoloid slant", "Upward slanting palpebral fissures", "Upward slanted palpebral fissures", "Upslanting palpebral fissures", "Upslanted palpebral fissures"},
    ocrit = "The palpebral fissure inclination is more than 2 standard deviations above the mean.",
    scrit = "The inclination of the palpebral fissure is greater than typical for age.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000582">Full details and related diseases.</a>'},
    metrics = {86},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(86):zscore(age, 0)
        rz = msr:metric(86):zscore(age, 0)
        return (lz + rz) > 4.0
    end
}


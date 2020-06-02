hpo = {
    id = 494,
    name = "Palpebral Fissure, Downslanted",
    region = "Orbital",
    synonyms = {"Downslanting palpebral fissures", "Down slanting palpebral fissures", "Downward-slanting palpebral fissures", "Antimongoloid eye slant", "Down-slanting palpebral fissure", "Palpebral fissures down-slanted", "Antimongoloid slanted palpebral fissures", "Downward slanted palpebral fissures", "Downward slanting palpebral fissures", "Antimongoloid slant of palpebral fissures", "Downslanting palpebral fissure", "Downward slanting of the opening between the eyelids", "Down-slanting palpebral fissures", "Down-slanted palpebral fissures"},
    ocrit = "The palpebral fissure inclination is more than 2 standard deviations below the mean.",
    scrit = "",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000494">Full details and related diseases.</a>'},
    metrics = {86},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(86):zscore(age, 0)
        rz = msr:metric(86):zscore(age, 0)
        return (lz + rz) < -4.0
    end
}


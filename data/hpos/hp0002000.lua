hpo = {
    id = 2000,
    name = "Columella, Short",
    region = "Nasal",
    synonyms = {"Decreased length of columella", "Columella, short", "Hypoplasia of columella"},
    ocrit = "The distance between subnasale and columella is more than 2 standard deviations below the mean.",
    scrit = "An apparently reduced distance from the anterior border of the naris to subnasale.",
    remarks = [[Often accompanied by a 'Depressed nasal tip', but this should be assessed separately. The term 'Absent columella' has been deleted because a columella is always thought to be present, except in 'Single naris' and 'Proboscis'.]],
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0002000">Full details and related diseases.</a>'},
    metrics = {49,105},
    determine = function( age, msf, msl, msr)
        return msf:metric(49):zscore( age, 0) < -2.0
    end
}

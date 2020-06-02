hpo = {
    id = 448,
    name = "Nose, Prominent",
    region = "Nasal",
    synonyms = {"Disproportionately large nose", "Hyperplasia of nose", "Big nose", "Increased nasal size", "Hypertrophy of nose", "Pronounced nose"},
    ocrit = "Distance between subnasale and pronasale is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased anterior protrusion of the nasal tip.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000448">Full details and related diseases.</a>'},
    metrics = {105},
    determine = function( age, msf, msl, msr)
        return msf:metric(105):zscore(age, 0) > 2.0
    end
}



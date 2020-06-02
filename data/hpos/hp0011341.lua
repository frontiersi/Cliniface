hpo = {
    id = 11341,
    name = "Upper Lip, Long",
    region = "Lip",
    synonyms = {"Increased vertical length of upper lip", "Increased height of upper lip"},
    ocrit = "Distance between the nasal base and the midpoint of the labial fissure is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased distance between the nasal base and the midpoint of the labial fissure.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0011341">Full details and related diseases.</a>'},
    metrics = {43},
    determine = function( age, msf, msl, msr)
        return msf:metric(43):zscore( age, 0) > 2.0
    end
}

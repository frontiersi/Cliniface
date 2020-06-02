hpo = {
    id = 322,
    name = "Philtrum, Short",
    region = "Lip",
    synonyms = {"Decreased height of philtrum"; "Decreased length of philtrum"; "Vertical hypoplasia of philtrum"; "Decreased vertical dimension of philtrum"},
    ocrit = "Distance between the nasal base and the midline upper vermilion border is more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased distance between the nasal base and the midline upper lip vermilion border.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000322">Full details and related diseases.</a>'},
    metrics = {38},
    determine = function( age, msf, msl, msr)
        return msf:metric(38):zscore(age, 0) < -2.0
    end
}

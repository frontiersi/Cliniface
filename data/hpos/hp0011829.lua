hpo = {
    id = 11829,
    name = "Philtrum, Narrow",
    region = "Lip",
    synonyms = {"Decreased breadth of philtrum"; "Decreased transverse dimension of philtrum"; "Thin philtrum"; "Decreased width of philtrum"},
    ocrit = "Distance between the philtral ridges (measured just above the vermilion border) is more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased distance between the ridges of the philtrum.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0011829">Full details and related diseases.</a>'},
    metrics = {40},
    determine = function( age, msf, msl, msr)
        return msf:metric(40):zscore(age, 0) < -2.0
    end
}

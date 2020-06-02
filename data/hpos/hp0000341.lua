hpo = {
    id = 341,
    name = "Forehead, Narrow",
    region = "Face",
    synonyms = {"Bitemporal narrowing", "Decreased width of forehead"},
    ocrit = "Width of the forehead is more than 2 standard deviations below the mean.",
    scrit = "An apparently narrow intertemporal region",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000341">Full details and related diseases.</a>'},
    metrics = {28},
    determine = function( age, msf, msl, msr)
        return msf:metric(28):zscore(age, 0) < -2.0
    end
}

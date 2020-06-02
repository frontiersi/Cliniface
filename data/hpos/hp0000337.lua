hpo = {
    id = 337,
    name = "Forehead, Broad",
    region = "Face",
    synonyms = {"Wide forehead", "Increased bitemporal width", "Increased width of forehead"},
    ocrit = "Width of the forehead is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased distance between the two sides of the forehead.",
    remarks = "Broad Forehead should not be confused with Prominent Forehead.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000337">Full details and related diseases.</a>'},
    metrics = {28},
    determine = function( age, msf, msl, msr)
        return msf:metric(28):zscore(age, 0) > 2.0
    end
}

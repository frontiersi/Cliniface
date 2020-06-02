hpo = {
    id = 10282,
    name = "Lower Lip Vermilion, Thin",
    region = "Lip",
    synonyms = {"Thin vermilion border of lower lip", "Decreased volume of lower lip vermilion", "Thin lower lip", "Decreased height of lower lip vermilion"},
    ocrit = "The height of the medial part of the lower lip vermilion is more than 2 standard deviations below the mean.",
    scrit = "An apparently reduced height of the lower lip vermilion in the frontal view.",
    remarks = "Measurements are not commonly used, this feature is mostly determined subjectively. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. If the lower lip vermilion is thin, the inferior border of the vermilion is less curved, and in profile view, the lower lip vermilion is less convex than usual.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0010282">Full details and related diseases.</a>'},
    metrics = {37},
    determine = function( age, msf, msl, msr)
        return msf:metric(37):zscore(age, 0) < -2.0
    end
}

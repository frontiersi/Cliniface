hpo = {
    id = 219,
    name = "Upper Lip Vermilion, Thin",
    region = "Lip",
    synonyms = {"Thin vermilion border of upper lip", "Decreased volume of upper lip vermilion", "Thin upper lip", "Decreased height of upper lip vermilion"},
    ocrit = "Height of the vermilion of the upper lip in the midline is more than 2 standard deviations below the mean.",
    scrit = "An apparently reduced height of the vermilion of the upper lip in the frontal view.",
    remarks = "Measurement is not commonly used; this feature is mostly determined subjectively or using the Likert Scale. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. A thin vermilion is less convex than usual in profile view, and may be associated with a Smooth Philtrum. Facial expression can influence the apparent height of the upper lip.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000219">Full details and related diseases.</a>'},
    metrics = {44},
    determine = function( age, msf, msl, msr)
        return msf:metric(44):zscore(age, 0) < -2.0
    end
}

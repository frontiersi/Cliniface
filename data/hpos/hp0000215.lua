hpo = {
    id = 215,
    name = "Upper Lip Vermilion, Thick",
    region = "Lip",
    synonyms = {"Thick upper lip", "Increased volume of upper lip", "Plump upper lip", "Thick vermilion border of upper lip", "Prominent/full upper lip", "Increased height of upper lip vermilion"},
    ocrit = "Height of the vermilion of the upper lip in the midline is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased height of the vermilion of the upper lip in the frontal view.",
    remarks = "Measurement is not commonly used; this feature is mostly determined subjectively or using the Likert Scale. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. A thick vermilion is more convex than usual in profile view. Facial expression can influence the apparent height of the upper lip.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000215">Full details and related diseases.</a>'},
    metrics = {44},
    determine = function( age, msf, msl, msr)
        return msf:metric(44):zscore(age, 0) > 2.0
    end
}

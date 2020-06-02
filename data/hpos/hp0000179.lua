hpo = {
    id = 179,
    name = "Lower Lip Vermilion, Thick",
    region = "Lip",
    synonyms = {"Thick vermilion border of lower lip", "Increased volume of lower lip vermilion", "Thick lower lip", "Increased height of lower lip vermilion", "Full/Prominent lower lip"},
    ocrit = "Height of the medial part of the lower lip vermilion is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased height of the lower lip vermilion in the frontal view.",
    remarks = "Measurements are not commonly used, this feature is mostly determined subjectively. The lower lip is typically thicker than the upper one. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. If the lower lip vermilion is thick, it is more convex and everted than usual in profile view.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000179">Full details and related diseases.</a>'},
    metrics = {37},
    determine = function( age, msf, msl, msr)
        return msf:metric(37):zscore(age, 0) > 2.0
    end
}

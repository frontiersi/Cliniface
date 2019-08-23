hpo = {
    id = 219,
    name = "Thin Upper Lip Vermilion",
    region = "Lip",
    synonyms = {"Thin vermilion border of upper lip", "Decreased volume of upper lip vermilion", "Thin upper lip", "Decreased height of upper lip vermilion"},
    criteria = "Objectively determined as the height of the vermilion of the upper lip in the midline being more than 2 standard deviations below the mean. Subjectively evaluated as an apparently reduced height of the vermilion of the upper lip in the frontal view.",
    remarks = "Measurements are not commonly used, this feature is mostly determined subjectively or using the Likert Scale. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. A thin vermilion is less convex than usual in profile view, and may be associated with a Smooth Philtrum. Facial expression can also influence the apparent height of the upper lip.",
    metrics = {44,1000},
    determine = function( age, msf, msl, msr)
        return msf:metric(44):zscore(age, 0) < -2.0
    end
}

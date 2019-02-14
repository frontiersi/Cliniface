hpo = {
    id = 215,
    name = "Thick Upper Lip Vermilion",
    region = "Lip",
    synonyms = {"Thick upper lip", "Increased volume of upper lip", "Plump upper lip", "Thick vermilion border of upper lip", "Prominent/full upper lip", "Increased height of upper lip vermilion"},
    criteria = "Objectively determined as the height of the vermilion of the upper lip in the midline being more than 2 standard deviations above the mean. Subjectively evaluated as an apparently increased height of the vermilion of the upper lip in the frontal view.",
    remarks = "Measurements are not commonly used, this feature is mostly determined subjectively or using the Likert Scale. Vermilion height varies among ethnic groups and should be compared to a population of same ethnic background. A thick vermilion is more convex than usual in profile view. Facial expression can also influence the apparent height of the upper lip.",
    metrics = {44,1000},
    determine = function( msf, msl, msr)
        return msf:get(44):zscore(0) > 2.0
    end
}

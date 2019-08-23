hpo = {
    id = 11341,
    name = "Long Upper Lip",
    region = "Lip",
    synonyms = {"Increased vertical length of upper lip", "Increased height of upper lip"},
    criteria = "Objectively determined as the distance between the nasal base and the midpoint of the labial fissure being more than 2 standard deviations above the mean. Subjectively determined as an apparently increased distance between nasal base and the midpoint of the labial fissure.",
    remarks = "",
    metrics = {43},
    determine = function( age, msf, msl, msr)
        return msf:metric(43):zscore( age, 0) > 2.0
    end
}

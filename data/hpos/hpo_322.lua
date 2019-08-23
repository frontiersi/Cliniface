hpo = {
    id = 322,
    name = "Short Philtrum",
    region = "Lip",
    synonyms = {"Decreased height of philtrum"; "Decreased length of philtrum"; "Vertical hypoplasia of philtrum"; "Decreased vertical dimension of philtrum"},
    criteria = "Objectively determined as the distance between the nasal base and midline upper vermilion border being more than 2 standard deviations below the mean. Subjectively determined as an apparently decreased distance between nasal base and midline upper lip vermilion border.",
    remarks = "",
    metrics = {38},
    determine = function( age, msf, msl, msr)
        return msf:metric(38):zscore(age, 0) < -2.0
    end
}

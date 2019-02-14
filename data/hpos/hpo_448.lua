hpo = {
    id = 448,
    name = "Prominent Nose",
    region = "Nasal",
    synonyms = {"Disproportionately large nose", "Hyperplasia of nose", "Big nose", "Increased nasal size", "Hypertrophy of nose", "Pronounced nose"},
    criteria = "Objectively determined as the distance between subnasale and pronasale being more than 2 standard deviations above the mean, or subjectively determined as an apparently increased anterior protrusion of the nasal tip.",
    remarks = "",
    metrics = {105},
    determine = function( msf, msl, msr)
        return msf:get(105):zscore(0) > 2.0
    end
}



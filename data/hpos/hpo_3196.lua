hpo = {
    id = 3196,
    name = "Short Nose",
    region = "Nasal",
    synonyms = {"Decreased nasal height", "Decreased nasal length", "Shortened nose", "Small nose", "Nasal hypoplasia"},
    criteria = "Objectively determined as the distance from nasion to subnasale being more than 2 standard deviations below the mean. Subjectively determined as an apparently decreased length from nasal root to nasal tip.",
    remarks = [[Nose length can be well within normal limits, but can still appear small due to a large midfacial length.]],
    metrics = {59;60},
    determine = function( age, msf, msl, msr)
        return msf:metric(60):zscore(age, 0) < -2.0
    end
}

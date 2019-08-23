hpo = {
    id = 3189,
    name = "Long Nose",
    region = "Nasal",
    synonyms = {"Increased nasal height", "Increased nasal length", "Nasal elongation"},
    criteria = "Objectively determined as the distance from nasion to subnasale being more than 2 standard deviations above the mean. Subjectively determined as an apparently increased length from nasal root to nasal base.",
    remarks = [[Some make a clear distinction between nasal height and length, while others use the terms interchangeably. Nose length usually shows an apparent inverse relationship with philtrum length. The nose continues to grow throughout life, contributing to increases in nasal ridge length, but not necessarily nasal length. At time of writing, there are no normal adult standards to determine this. A Long Nose is distinct from a Large Nose.]],
    metrics = {59;60},
    determine = function( age, msf, msl, msr)
        return msf:metric(60):zscore(age, 0) > 2.0
    end
}

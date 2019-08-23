hpo = {
    id = 160,
    name = "Narrow Mouth",
    region = "Lip",
    synonyms = {"Small oral aperture", "Small mouth", "Microstomia"},
    criteria = "Objectively determined as the commissures of the mouth being more than 2 standard deviations below the mean. Subjectively determined as an apparently decreased width of the oral aperture.",
    remarks = "The width of the mouth varies with facial movement and must be assessed with a relaxed or neutral face.",
    metrics = {39},
    determine = function( age, msf, msl, msr)
        return msf:metric(39):zscore(age, 0) < -2.0
    end
}


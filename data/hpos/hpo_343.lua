hpo = {
    id = 343,
    name = "Long Philtrum",
    region = "Lip",
    synonyms = {"Elongated philtrum"; "Increased length of philtrum"; "Increased vertical dimension of philtrum"; "Vertical hyperplasia of philtrum"; "Increased height of philtrum"},
    criteria = "Objectively determined as the distance between the nasal bone/base and midline upper lip vermilion border being more than 2 standard deviations above the mean. Subjectively determined as an apparently increased distance between the nasal base and the midline upper lip vermilion border.",
    remarks = "The philtrum & nose together occupy the central face zone; a change in length of one will usually be accompanied by a compensatory & opposite change in the other. A Short Nose often accompanies a Long Philtrum, but this is not always the case.",
    metrics = {38},
    determine = function( msf, msl, msr)
        return msf:get(38):zscore(0) > 2.0
    end
}


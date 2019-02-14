hpo = {
    id = 11829,
    name = "Narrow Philtrum",
    region = "Lip",
    synonyms = {"Decreased breadth of philtrum"; "Decreased transverse dimension of philtrum"; "Thin philtrum"; "Decreased width of philtrum"},
    criteria = "Objectively determined as the distance between the philtral ridges (measured just above the vermilion border) being more than 2 standard deviations below the mean. Subjectively determined as an apparently decreased distance between the ridges of the philtrum.",
    remarks = "",
    metrics = {40},
    determine = function( msf, msl, msr)
        return msf:get(40):zscore(0) < -2.0
    end
}

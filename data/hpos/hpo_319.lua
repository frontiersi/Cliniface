hpo = {
    id = 319,
    name = "Smooth Philtrum",
    region = "Lip",
    synonyms = {"Flat philtrum"; "Indistinct philtrum"; "Shallow philtrum"; "Decreased depth of philtrum"},
    criteria = "Flat skin surface, with no ridge formation in the central region of the upper lip between the nasal base and upper vermilion border (subjective).",
    remarks = "This is greatly influenced by facial expression. A Smooth Philtrum can be associated with a Long Philtrum.",
    metrics = {1001},
    determine = function( msf, msl, msr)
        return false
    end
}



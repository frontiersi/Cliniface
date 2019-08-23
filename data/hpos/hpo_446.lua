hpo = {
    id = 446,
    name = "Narrow Nasal Bridge",
    region = "Nasal",
    synonyms = {"Pinched nasal bridge"; "Narrow nasal bridge"; "Narrow nasal root"; "Thin nasal bridge"},
    criteria = "Decreased width of the bony bridge of the nose (subjective).",
    remarks = "Nasal bridge may narrow with age.",
    metrics = {8;81},
    determine = function( age, msf, msl, msr)
        return false
    end
}

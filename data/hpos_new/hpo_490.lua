hpo = {
    id = 490,
    name = "Deeply Set Eye",
    region = "Orbital",
    synonyms = {"Ocular depression";"Enophthalmos";"Sunken eyes";"Deep set eye"},
    criteria = "Subjectively evaluated as an eye that is more deeply recessed into the plane of the face than is typical.",
    remarks = "Deeply set eye should be distinguished from a prominent supraorbital ridge or inferior orbital margin. In Deeply Set Eye, the globe is recessed in comparison to the overall prominence of the face. There is no known objective measurement, diagnosis heavily dependent on observer experience.",
    metrics = {117;158;159;160;112;113;88},
    determine = function( msf, msl, msr)
        return false
    end
}



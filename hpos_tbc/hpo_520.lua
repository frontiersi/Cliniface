hpo = {
    id = 520,
    name = "Proptosis",
    region = "Orbital",
    synonyms = {"Exophthalmos", "Ocular proptosis", "Prominent globes", "Anterior bulging of the globe of eye", "Prominent eyes", "Bulging eye", "Protruding eyes"},
    criteria = "Subjectively evaluated as an eye that is protruding anterior to the facial plane to a greater extent than is typical.",
    remarks = "Proptosis should be distinguished from underdevelopment of the supraorbital ridge or maxilla/zygoma. There is no known objective measurement & diagnosing this feature is heavily dependent on the experience of the observer.",
    metrics = {117;158;159;160;112;113;88},
    determine = function( msf, msl, msr)
        return 0.5
    end
}

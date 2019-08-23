hpo = {
    id = 601,
    name = "Hypotelorism",
    region = "Orbital",
    synonyms = {"Decreased interpupillary distance", "Decreased orbital separation", "Ocular hypotelorism", "Decreased distance between eye sockets", "Closely spaced eyes"},
    criteria = "Objectively determined as the Interpupillary distance being less than 2 standard deviations below the mean. Subjectively evaluated as the appearance of decreased distance.",
    remarks = "",
    metrics = {85},
    determine = function( age, msf, msl, msr)
        return msf:metric(85):zscore(age, 0) < -2.0
    end
}


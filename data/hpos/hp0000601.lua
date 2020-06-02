hpo = {
    id = 601,
    name = "Hypotelorism",
    region = "Orbital",
    synonyms = {"Decreased interpupillary distance", "Decreased orbital separation", "Ocular hypotelorism", "Decreased distance between eye sockets", "Closely spaced eyes"},
    ocrit = "Interpupillary distance is less than 2 standard deviations below the mean.",
    scrit = "Interpupillary distance appears lower than normal.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000601">Full details and related diseases.</a>'},
    metrics = {85},
    determine = function( age, msf, msl, msr)
        return msf:metric(85):zscore(age, 0) < -2.0
    end
}


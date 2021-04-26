hpo = {
    id = 426,
    name = "Nasal Bridge, Prominent",
    region = "Nasal",
    synonyms = {"Prominent nasal bridge",
                "Elevated nasal bridge",
                "High nasal bridge",
                "Convex bridge of nose",
                "Protruding nasal bridge",
                "Prominent nasal root",
                "Protruding bridge of nose",
                "Prominent bridge of nose",
                "Convex nasal bridge"
                },
    ocrit = "Nasal root depth is more than 2 standard deviations above the mean.",
    scrit = "Anterior positioning of the nasal root in comparison to the usual positioning for age.",
    remarks = 'A prominent nasal bridge can occur irrespective of the width of the nasal bridge, and the width should be assessed separately. The nasal bridge becomes more prominent with age. Although the nasal root may be anteriorly placed without increasing the space between the eyes, prominence of the nasal bridge may be accompanied by Telecanthus or ocular Hypertelorism. If such findings are present these should be coded separately. Deep-set eyes may lead to the impression of a prominent nasal bridge, but this finding should be coded separately.',
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000426">Full details and related diseases.</a>'},
    metrics = {2003},
    determine = function( age, msf, msl, msr)
        return msf:metric(2003):zscore( age, 0) > 2.0
    end
}

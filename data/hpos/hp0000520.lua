hpo = {
    id = 520,
    name = "Proptosis",
    region = "Orbital",
    synonyms = {"Exophthalmos",
                "Ocular proptosis",
                "Prominent globes",
                "Anterior bulging of the globe of eye",
                "Prominent eyes",
                "Bulging eye",
                "Protruding eyes"},
    ocrit = "Orbital protrusion for both eyes is more than 2 standard deviations above the mean.",
    scrit = "The eye globes appear to protrude anteriorly to a greater extent than is typical.",
    remarks = "Proptosis should be distinguished from underdevelopment of the supraorbital ridge or maxilla/zygoma.",
    refs = {"Gripp, K.; Slavotinek, A.; Hallm, J. Handbook of Physical Measurements; Oxford University Press: Oxford, UK, 2013.", '<a href="https://hpo.jax.org/app/browse/term/HP:0000508">Full details and related diseases.</a>'},
    metrics = {2002},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(2002):zscore( age, 0)
        rz = msr:metric(2002):zscore( age, 0)
        return (lz + rz) > 4.0
    end
}

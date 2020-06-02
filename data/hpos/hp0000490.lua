hpo = {
    id = 490,
    name = "Eyes, Deeply Set",
    region = "Orbital",
    synonyms = {"Ocular depression",
                "Enophthalmos",
                "Sunken eyes",
                "Deep set eyes"},
    ocrit = "Orbital protrusion for both eyes is more than 2 standard deviations below the mean.",
    scrit = "The eye globes appear more deeply recessed than is typical.",
    remarks = "Deeply Set Eye should be distinguished from a prominent supraorbital ridge or inferior orbital margin. In Deeply Set Eye, the globe is recessed in comparison to the overall prominence of the face.",
    refs = {"Gripp, K.; Slavotinek, A.; Hallm, J. Handbook of Physical Measurements; Oxford University Press: Oxford, UK, 2013.", '<a href="https://hpo.jax.org/app/browse/term/HP:0000490">Full details and related diseases.</a>'},
    metrics = {2002},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(2002):zscore( age, 0)
        rz = msr:metric(2002):zscore( age, 0)
        return (lz + rz) < -4.0
    end
}



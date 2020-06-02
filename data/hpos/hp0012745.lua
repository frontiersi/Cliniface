hpo = {
    id = 12745,
    name = "Palpebral Fissures, Short",
    region = "Orbital",
    synonyms = {"Short opening between the eyelids", "Decreased width of palpebral fissure"},
    ocrit = "Distance between the medial and lateral canthi is more than 2 standard deviations below the mean.",
    scrit = "Subjectively evaluated as an apparently reduced length of the palpebral fissures.",
    remarks = "Severe shortening of the palpebral fissures leads to Blepharophimosis. Decreased palpebral fissure lengths may be accompanied by a decreased palpebral fissure width (vertical distance between the upper and lower eyelids).",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0012745">Full details and related diseases.</a>'},
    metrics = {10},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(10):zscore(age, 0)
        rz = msr:metric(10):zscore(age, 0)
        return (lz + rz) < -4.0
    end
}

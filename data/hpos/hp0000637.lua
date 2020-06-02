hpo = {
    id = 637,
    name = "Palpebral Fissures, Long",
    region = "Orbital",
    synonyms = {"Wide palpebral fissures", "Long opening between the eyelids", "Broad opening between the eyelids", "Wide opening between the eyelids"},
    ocrit = "Distance between the medial and lateral canthi is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased length of the palpebral fissures.",
    remarks = "The term 'Wide palpebral fissure' is discouraged due to the ambiguity of which dimension it refers to (horizontal length vs vertical height).",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000637">Full details and related diseases.</a>'},
    metrics = {10},
    determine = function( age, msf, msl, msr)
        lz = msl:metric(10):zscore(age, 0)
        rz = msr:metric(10):zscore(age, 0)
        v = (lz + rz) > 4.0
        return v
    end
}

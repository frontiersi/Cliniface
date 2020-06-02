hpo = {
    id = 3196,
    name = "Nose, Short",
    region = "Nasal",
    synonyms = {"Decreased nasal height", "Decreased nasal length", "Shortened nose", "Small nose", "Nasal hypoplasia"},
    ocrit = "Distance from nasion to subnasale is more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased length from nasal root to nasal tip.",
    remarks = [[Nose length can be well within normal limits, but can still appear small due to a large midfacial length.]],
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0003196">Full details and related diseases.</a>'},
    metrics = {59;60},
    determine = function( age, msf, msl, msr)
        return msf:metric(60):zscore(age, 0) < -2.0
    end
}

hpo = {
    id = 11219,
    name = "Face, Short",
    region = "Face",
    synonyms = {"Vertical shortening of face", "Vertical facial deficiency", "Decreased height of face", "Decreased length of face", "Short facies", "Vertical hypoplasia of face"},
    ocrit = "Facial height (length) is more than 2 standard deviations below the mean, and facial index (100*height/width) is < 85.",
    scrit = "An apparently decreased height of the face.",
    remarks = "Short face is distinct from Broad Face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0011219">Full details and related diseases.</a>'},
    metrics = {16,18},
    determine = function( age, msf, msl, msr)
        findex = 100 * msf:metric(16):value(0) / msf:metric(18):value(0)    -- Facial index
        return findex < 85
           and msf:metric(16):zscore(age, 0) < -2.0
    end
}

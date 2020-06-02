hpo = {
    id = 276,
    name = "Face, Long",
    region = "Face",
    synonyms = {"Vertical hyperplasia of face", "Vertical overgrowth of face", "Increased height of face", "Increased length of face", "Long facies", "Elongation of face"},
    ocrit = "Facial height (length) is more than 2 standard deviations above the mean, and the facial index (100*height/width) is >= 90.",
    scrit = "An apparently increased height of the face.",
    remarks = "Long Face is distinct from Narrow Face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000276">Full details and related diseases.</a>'},
    metrics = {16,18},
    determine = function( age, msf, msl, msr)
        findex = 100 * msf:metric(16):value(0) / msf:metric(18):value(0)    -- Facial index (height / upper width)
        return findex >= 90 -- Tall, narrow face?
           and msf:metric(16):zscore(age, 0) > 2.0
    end
}

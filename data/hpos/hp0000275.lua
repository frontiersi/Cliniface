hpo = {
    id = 275,
    name = "Face, Narrow",
    region = "Face",
    synonyms = {"Narrow facies", "Horizontal deficiency of face", "Thin facies", "Decreased transverse dimension of face", "Decreased width/breadth of face"},
    ocrit = "Bizygomatic (upper face) and bigonial (lower face) widths are both unusually short (more than 2 standard deviations below their means), and the facial index (100*height/width) is >= 90.",
    scrit = "An apparently reduced width of the upper and lower face.",
    remarks = "Narrow Face is distinct from Long Face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000275">Full details and related diseases.</a>'},
    metrics = {16,18,26},
    determine = function( age, msf, msl, msr)
        findex = 100 * msf:metric(16):value(0) / msf:metric(18):value(0)    -- Facial index (height / upper width)
        return msf:metric(18):zscore(age, 0) < -2.0
           and msf:metric(26):zscore(age, 0) < -2.0
           and findex >= 90
    end
}

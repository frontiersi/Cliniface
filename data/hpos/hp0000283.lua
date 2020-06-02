hpo = {
    id = 283,
    name = "Face, Broad",
    region = "Face",
    synonyms = {"Broad facies", "Increased horizontal dimension of face", "Wide face", "Transverse excess/hyperplasia of face", "Increased width/breadth of face"},
    ocrit = "Bizygomatic (upper face) and bigonial (lower face) widths are both unusually long (more than 2 standard deviations above their means), and the facial index (100*height/width) is < 85.",
    scrit = "An apparently increased width of the upper and lower face.",
    remarks = "Broad Face is distinct from Round Face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000283">Full details and related diseases.</a>'},
    metrics = {16,18,26},
    determine = function( age, msf, msl, msr)
        findex = 100 * msf:metric(16):value(0) / msf:metric(18):value(0)    -- Facial index (height / upper width)
        return msf:metric(18):zscore(age, 0) > 2.0  -- Unusually wide upper face
           and msf:metric(26):zscore(age, 0) > 2.0  -- Unusually wide lower face
           and findex < 85  -- Broad face
    end
}

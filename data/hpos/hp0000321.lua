hpo = {
    id = 321,
    name = "Face, Square",
    region = "Face",
    synonyms = {},
    ocrit = "Bizygomatic (upper face) width is greater than average, and the bigonial (lower face) width is unusually long (more than 2 standard deviations above the mean), but the facial index (100*height/width) is of average value in the range [85,90).",
    scrit = "The face as viewed from the front is both broad in the upper face/cranium and in the lower face/mandible creating a square appearance.",
    remarks = "Square face shape is usually related to increased lower facial width. In Broad Jaw, the lower face is wider than the upper face.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000321">Full details and related diseases.</a>'},
    metrics = {16,18,26},
    determine = function( age, msf, msl, msr)
        fheight = msf:metric(16):value(0)
        fupperw = msf:metric(18):value(0)
        findex = 100 * fheight / fupperw -- Facial index (height / upper width)
        return msf:metric(18):zscore(age, 0) > 0.0 -- Wider than average upper face
           and msf:metric(26):zscore(age, 0) > 2.0 -- Unusually wide lower face
           and findex >= 85 and findex < 90 -- Not narrow in the upper face
    end
}

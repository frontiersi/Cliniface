hpo = {
    id = 311,
    name = "Face, Round",
    region = "Face",
    synonyms = {"Round facial appearance", "Round facies", "Circular face", "Round, full face"},
    ocrit = "Bizygomatic (upper face) and bigonial (lower face) widths are not unusually long (both less than 2 standard deviations above their means), and the facial index (100*height/width) is of average value in the range [85,90).",
    scrit = "Facial appearance is more circular than usual as viewed from the front.",
    remarks = "While a Broad Face may have rounded cheeks, a Round Face appears to be as long as it is broad.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000311">Full details and related diseases.</a>'},
    metrics = {16,18,26},
    determine = function( age, msf, msl, msr)
        fheight = msf:metric(16):value(0)
        fupperw = msf:metric(18):value(0)
        findex = 100 * fheight / fupperw -- Facial index (height / upper width)
        return msf:metric(18):zscore(age, 0) < 2.0 -- Not unusually wide upper face
           and msf:metric(26):zscore(age, 0) < 2.0 -- Not unusually wide lower face
           and findex >= 85 and findex < 90  -- Average face (not narrow, nor broad)
    end
}

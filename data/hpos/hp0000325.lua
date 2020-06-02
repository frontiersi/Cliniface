hpo = {
    id = 325,
    name = "Face, Triangular",
    region = "Face",
    synonyms = {"Triangular facies", "Face with broad temples and narrow chin"},
    ocrit = "Bizygomatic (upper face) width is greater than average by 1 standard deviation, and the bigonial (lower face) width is narrower than average by 1 standard deviation, and the mandibular contour is less than 90 degrees.",
    scrit = "The face as viewed from the front is triangular in shape, with breadth at the temples and tapering to a narrow chin.",
    remarks = "Triangular face is distinct from Narrow Jaw, where the width of the midface is unchanged.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000325">Full details and related diseases.</a>'},
    metrics = {18,26,89},
    determine = function( age, msf, msl, msr)
        return msf:metric(18):zscore(age, 0) > 1.0 -- wide upper face
           and msf:metric(26):zscore(age, 0) < -1.0 -- narrow lower face
           and msf:metric(89):value(0) < 90 -- pointed chin
    end
}

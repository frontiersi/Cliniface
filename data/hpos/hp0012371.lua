hpo = {
    id = 12371,
    name = "Hyperplasia of midface",
    region = "Face",
    synonyms = {"Hypertrophy of midface", "Large midface", "Big midface", "Overgrowth of the midface", "Midfacial excess", "Midface hyperplasia"},
    ocrit = "Facial convexity more than 2 standard deviations below the mean.",
    scrit = "Abnormally anterior positioning of the infraorbital and perialar regions, or increased convexity of the face, or increased nasolabial angle. The midface includes the maxilla, the cheeks, the zygomas, and the infraorbital and perialar regions of the face.",
    remarks = 'This term represents increased size of the maxilla (upper jaw) in length (increased midface height) or anterior-posterior projection (midface prominence). In the presence of normal mandible size, maxillary prominence may give the appearance of retrognathia. Facial convexity is <i>increased</i> if measured facial convexity angle is <i>lower</i>. Facial convexity is not sexually dimorphic and does not change significantly with age.',
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0012371">Full details and related diseases.</a>', '<a href="https://doi.org/10.1016/0002-9416(67)90022-X">Burstone (1967)</a>', '<a href="https://doi.org/10.1016/0889-5406(93)70010-L">Arnett and Bergman (1993)</a>', '<a href="https://doi.org/10.1016/S0889-5406(98)70203-3">Bishara et al. (1998)</a>'},
    metrics = {103},
    determine = function( age, msf, msl, msr)
        -- T = 168.7 - 2*4.1   -- Taken from https://doi.org/10.1016/0889-5406(93)70010-L
        return msf:metric(103):zscore( age, 0) < -2.0
    end
}

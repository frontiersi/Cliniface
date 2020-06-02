hpo = {
    id = 11800,
    name = "Midface retrusion",
    region = "Face",
    synonyms = {"Decreased size of midface", "Underdevelopment of midface", "Flat midface", "Midface hypoplasia", "Midface deficiency", "Hypotrophic midface", "Small midface", "Decreased projection of midface", "Midface, flat", "Retrusive midface", "Hypoplasia of midface"},
    ocrit = "Facial convexity more than 2 standard deviations above the mean.",
    scrit = "Posterior positions and/or vertical shortening of the infraorbital and perialar regions, or increased concavity of the face and/or reduced nasolabial angle.",
    remarks = 'Facial convexity is <i>decreased</i> if measured facial convexity angle is <i>higher</i>. Facial convexity is not sexually dimorphic and does not change significantly with age',
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0011800">Full details and related diseases.</a>', '<a href="https://doi.org/10.1016/0002-9416(67)90022-X">Burstone (1967)</a>', '<a href="https://doi.org/10.1016/0889-5406(93)70010-L">Arnett and Bergman (1993)</a>', '<a href="https://doi.org/10.1016/S0889-5406(98)70203-3">Bishara et al. (1998)</a>'},
    metrics = {103},
    determine = function( age, msf, msl, msr)
        --T = 168.7 + 2*4.1   -- Taken from https://doi.org/10.1016/0889-5406(93)70010-L
        return msf:metric(103):zscore( age, 0) > 2.0
    end
}

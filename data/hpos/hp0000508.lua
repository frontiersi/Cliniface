hpo = {
    id = 508,
    name = "Ptosis",
    region = "Orbital",
    synonyms = {"Drooping upper eyelid", "Eyelid ptosis", "Blepharoptosis", "Eye drop"},
    ocrit = "Upper eyelid margin is positioned 3mm or more lower than usual and the palpebral fissure length is within 2 standard deviations of the mean.",
    scrit = "Upper eyelid margin is lower than usual (not fixed) and covers the superior portion of the iris, or obscures at least a part of the pupil.",
    remarks = "Ptosis is the term to be used when the reduction in eyelid opening is not fixed but can be increased actively or passively.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000508">Full details and related diseases.</a>'},
    metrics = {10,87},
    determine = function( age, msf, msl, msr)
        pfllz = msl:metric(10):zscore(age, 0)    -- Left palpebral fissure length Z-score
        pflrz = msr:metric(10):zscore(age, 0)    -- Right palpebral fissure length Z-score
        pflz = 0.5 * (pfllz + pflrz)    -- Mean PFL Z-score
        pflTest = (pflz >= -2.0) and (pflz <= 2.0)   -- PFL within 2SD of mean?

        pfwm = msr:metric(87):mean(age, 0)  -- Distribution mean (can use either eye metric set for this)
        pfwlv = msl:metric(87):value(0)     -- Left palpebral fissure width (vertical)
        pfwrv = msr:metric(87):value(0)     -- Right palpebral fissure width (vertical)
        pfwv = 0.5 * (pfwlv + pfwrv)    -- Mean of both eyes
        pfwTest = pfwv <= (pfwm - 3.0)  -- Mean palpebral fissure width less than 3mm from usual (distribution mean)

        return pflTest and pfwTest
    end
}

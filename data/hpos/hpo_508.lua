hpo = {
    id = 508,
    name = "Ptosis",
    region = "Orbital",
    synonyms = {"Drooping upper eyelid", "Eyelid ptosis", "Blepharoptosis", "Eye drop"},
    criteria = [[Objectively determined as the upper eyelid margin being positioned 3mm or more lower than usual.
Subjectively determined as the upper eyelid margin being 3mm or more lower than usual (not fixed) and covering the superior portion of the iris, or obscuring at least a part of the pupil.]],
    remarks = "Ptosis is the term to be used when the reduction in eyelid opening is not fixed but can be increased actively or passively.",
    metrics = {10,87},
    determine = function( msf, msl, msr)
        pfllz = msl:get(10):zscore(0)    -- Left palpebral fissure length Z-score
        pflrz = msr:get(10):zscore(0)    -- Right palpebral fissure length Z-score
        pflz = 0.5 * (pfllz + pflrz)    -- Mean PFL Z-score
        pflTest = (pflz >= -2.0) and (pflz <= 2.0)   -- PFL within 2SD of mean?

        pfwm = msr:get(87):mean(0)       -- Distribution mean (can use either eye metric set for this)
        pfwlv = msl:get(87):value(0)     -- Left palpebral fissure width (vertical)
        pfwrv = msr:get(87):value(0)     -- Right palpebral fissure width (vertical)
        pfwv = 0.5 * (pfwlv + pfwrv)    -- Mean of both eyes
        pfwTest = pfwv <= (pfwm - 3.0)  -- Mean palpebral fissure width less than 3mm from usual (distribution mean)

        return pflTest and pfwTest
    end
}

hpo = {
    id = 506,
    name = "Telecanthus",
    region = "Orbital",

    synonyms = {"Dystopia canthorum",
                "Corners of eye widely separated",
                "Increased distance between medial canthi",
                "Increased intercanthal distance"},

    ocrit = "The W index where a value of more than 1.95 is abnormal calculated as:\
    W = X + Y + a/b,\
    X = (2a - (0.2119c + 3.909))/c,\
    Y = (2a - (0.2479b + 3.909))/b,\
where 'a' is the inner canthal distance, 'b' is the interpupillary distance, and 'c' is the outer canthal distance.",
    scrit = "An apparently increased distance between the inner canthi while the pupils appear in their normal positions (not widely spaced).",

    remarks = "Intercanthal distance varies between ethnic groups. Telecanthus can be present with or without Hypertelorism.\
Dystopia canthorum is associated with conditions including Waardenburg syndrome which is due to a mutation in genes including the PAX gene.",
    
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000506">Full details and related diseases.</a>'},
    metrics = {85,8,6},
    determine = function( age, msf, msl, msr)
        a = msf:metric(8):value( 0)     -- Intercanthal Width
        b = msf:metric(85):value( 0)    -- Interpupillary
        c = msf:metric(6):value( 0)     -- Outercanthal Width

        x = (2*a - (0.2119*c + 3.909))/c
        y = (2*a - (0.2479*b + 3.909))/b
        w = x + y + a/b

        return w > 1.95
    end

    --[[
    metrics = {85;8;46},

    determine = function( age, msf, msl, msr)
        pd = msf:metric(85):zscore(age, 0) <= 2.0    -- Interpupillary distance <= 2SD above mean
        ic = msf:metric(8):zscore(age, 0) > 2.0      -- Intercanthal distance > 2SD above mean
        -- IC > subnasal width
        swv = msf:metric(46):value(0)
        icv = msf:metric(8):value(0)
        return pd and ic and (icv > swv)
    end
    --]]
}

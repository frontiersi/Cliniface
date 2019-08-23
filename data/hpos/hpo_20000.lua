hpo = {
    id = 20000,
    name = "Dystopia canthorum",
    region = "Orbital",
    synonyms = {"W Index", "Lateral displacement of inner canthi", "Telecanthus"},
    criteria = "Objectively calculated as the W index where a value of more than 1.95 is abnormal. The calculation is as follows:\nW = X + Y + a/b,\nX = (2a - (0.2119c + 3.909))/c,\nY = (2a - (0.2479b + 3.909))/b,\nwhere 'a' is the inner canthal distance, 'b' is the interpupillary distance, and 'c' is the outer canthal distance.",
    remarks = "Dystopia canthorum is associated with conditions including Waardenburg syndrome which is due to a mutation in genes including the PAX gene.",
    metrics = {8,85,6},
    determine = function( age, msf, msl, msr)
        a = msf:metric(8):value( 0)
        b = msf:metric(85):value( 0)
        c = msf:metric(6):value( 0)

        x = (2*a - (0.2119*c + 3.909))/c
        y = (2*a - (0.2479*b + 3.909))/b
        w = x + y + a/b

        return w > 1.95
    end
}

hpo = {
    id = 463,
    name = "Anteverted nares",
    region = "Nasal",
    synonyms = {"Anteverted nose", "Upturned nasal tip", "Upturned nares", "Upturned nasal tips", "Upturned nostrils", "Anteverted nostrils", "Nostrils anteverted", "Nasal tip, upturned", "Upturned nose"},
    ocrit = "The nasolabial angle is more than 2 standard deviations above the mean.",
    scrit = "Anteriorly-facing nostrils viewed with the head in the Frankfurt horizontal and the eyes of the observer level with the eyes of the subject. This gives the appearance of an upturned nose (upturned nasal tip).",
    remarks = "The tip of the nose is upturned and is positioned superiorly to the nasal base, allowing the nares to be easily visualized from the front. With maturation and growth of the nasal ridge and tip, the nares usually become more downwardly directed. Studies of caucasians have found no sexual dimorphism and no significant variance with age.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000463">Full details and related diseases.</a>', '<a href="https://doi.org/10.2319/041513-291.1">Bergman et al. (2014)</a>', '<a href="https://doi.org/10.1016/0889-5406(92)70048-F">Fitzgerald et al. (1992)</a>'},
    metrics = {1006},
    determine = function( age, msf, msl, msr)
        return msf:metric(1006):zscore( age, 0) > 2.0
    end
}



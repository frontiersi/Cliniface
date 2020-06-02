hpo = {
    id = 3189,
    name = "Nose, Long",
    region = "Nasal",
    synonyms = {"Increased nasal height", "Increased nasal length", "Nasal elongation"},
    ocrit = "Distance from nasion to subnasale is more than 2 standard deviations above the mean.",
    scrit = "An apparently increased length from nasal root to nasal base.",
    remarks = [[Some make a clear distinction between nasal height and length, while others use the terms interchangeably. Nose length usually shows an apparent inverse relationship with philtrum length. The nose continues to grow throughout life, contributing to increases in nasal ridge length, but not necessarily nasal length. At time of writing, there are no normal adult standards to determine this. A Long Nose is distinct from a Large Nose.]],
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0003189">Full details and related diseases.</a>'},
    metrics = {59;60},
    determine = function( age, msf, msl, msr)
        return msf:metric(60):zscore(age, 0) > 2.0
    end
}

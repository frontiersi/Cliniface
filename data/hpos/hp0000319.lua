hpo = {
    id = 319,
    name = "Philtrum, Smooth",
    region = "Lip",
    synonyms = {"Flat philtrum"; "Indistinct philtrum"; "Shallow philtrum"; "Decreased depth of philtrum"},
    ocrit = "Philtal depth more than 2 standard deviations below the mean at <i>any</i> of the three measured points of depth in the midline of the philtrum.",
    scrit = "Flat skin surface, with no ridge formation in the central region of the upper lip between the nasal base and upper vermilion border.",
    remarks = "Greatly influenced by facial expression. A Smooth Philtrum can be associated with a Long Philtrum.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0000319">Full details and related diseases.</a>'},
    metrics = {2001},
    determine = function( age, msf, msl, msr)
        return msf:metric(2001):zscore(age, 0) < -2.0 or
               msf:metric(2001):zscore(age, 1) < -2.0 or
               msf:metric(2001):zscore(age, 2) < -2.0 
    end
}



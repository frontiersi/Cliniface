hpo = {
    id = 12802,
    name = "Jaw, Wide",
    region = "Face",
    synonyms = {"Broad mandible", "Broad jaw", "Broad lower face", "Wide jaw", "Wide mandible"},
    ocrit = "Bigonial distance (lower facial width) more than 2 standard deviations above the mean.",
    scrit = "An apparently increased width of the lower jaw (mandible) when viewed from the front.",
    remarks = "The lower jaw is measured from the right gonion to the left gonion using spreading calipers. The gonion is the point at the angle of the bony mandible where the ramus changes direction to become the body of the mandible. It faces inferiorly and medially and is best found by placing a finger over the outward facing angle and rolling the finger downwards and inwards. When a broad jaw is accompanied by a broad upper face the term Square face is used.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0012802">Full details and related diseases.</a>'},
    metrics = {26},
    determine = function( age, msf, msl, msr)
        return msf:metric(26):zscore(age, 0) > 2.0
    end
}

hpo = {
    id = 12801,
    name = "Jaw, Narrow",
    region = "Face",
    synonyms = {"Thin lower face", "Thin lower jaw", "Narrow mandible", "Narrow jaw", "Narrow lower face", "Narrow lower jaw"},
    ocrit = "Bigonial distance (lower facial width) more than 2 standard deviations below the mean.",
    scrit = "An apparently decreased width of the lower jaw (mandible) when viewed from the front.",
    remarks = "The width of the jaw is measured from the right gonion to the left gonion using spreading calipers. The gonion is the point at the angle of the bony mandible where the ramus changes direction to become the body of the mandible. It faces inferiorly and medially and is best found by placing a finger over the outward facing angle and rolling the finger downwards and inwards.",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0012801">Full details and related diseases.</a>'},
    metrics = {26},
    determine = function( age, msf, msl, msr)
        return msf:metric(26):zscore(age, 0) < -2.0
    end
}

hpo = {
    id = 9765,
    name = "Columella, Low hanging",
    region = "Nasal",
    synonyms = {"Columella, low", "Columella, low hanging", "Extension of the columella below the ala nasi", "Prominent columella", "Columella extends below the ala nasi", "Low-hanging columella", "Rounded columella"},
    ocrit = "Nasolabial angle D (columella - subnasal - sublabial) is 90 degrees or less.",
    scrit = "Columella extends inferiorly to the level of the nasal base or lower, when viewed from the side.",
    remarks = "",
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0009765">Full details and related diseases.</a>'},
    metrics = {1008},
    determine = function( age, msf, msl, msr)
        return msf:metric(1008):value( 0) <= 90.0
    end
}

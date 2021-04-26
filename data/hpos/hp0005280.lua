hpo = {
    id = 5280,
    name = "Nasal Bridge, Depressed",
    region = "Nasal",
    synonyms = {"Low nasal bridge",
                "Concave nasal bridge",
                "Low nasal root",
                "Depressed bridge of nose",
                "Flat bridge of nose",
                "Concave bridge of nose",
                "Depressed nasal bridge",
                "Flattened nasal bridge",
                "Depressed nasal root/bridge",
                "Retruded bridge of nose",
                "Flat nasal root",
                "Flat nasal bridge",
                "Retruded nasal bridge",
                "Depressed nasal root"},
    ocrit = "Nasal root depth is more than 2 standard deviations below the mean.",
    scrit = "Posterior positioning of the nasal root in relation to the overall facial profile for age.",
    remarks = 'The adjective "depressed" here does not indicate an active process but a status. A depressed nasal bridge can occur irrespective of the width of the nasal bridge, and the width should be assessed independently. In infancy, the nasal bridge is relatively more posterior than in the older person. The term depressed nasal bridge should only be used when the bridge is more posterior than is typical for age and ethnic background.',
    refs = {'<a href="https://hpo.jax.org/app/browse/term/HP:0005280">Full details and related diseases.</a>'},
    metrics = {2003},
    determine = function( age, msf, msl, msr)
        return msf:metric(2003):zscore( age, 0) < -2.0
    end
}

report = {
    name = "Detected Phenotypic Indications",
    title = "Phenotypic Indications",

    available = function( m)
        return m:hasLandmarks()
    end,

    content = function( r, m)   -- r is Report, m is FaceModel

        s = "\n" .. r:makeScanInfo()
        s = s .. r:makeFigure( 150, 160, "") .. "\n"

        pids = m:phenotypes() -- phenotype ids

        if ( #pids > 0) then
            s = s .. [[
            \centering
            \textbf{General Phenotypic Indications} \\
            \raggedleft
            \begin{itemize}]] .. "\n"

            for i, j in pids:pairs() do
                s = s .. "\\item " .. phenotype(j):name():toStdString() .. "\n"
            end

            s = s .. [[
            \end{itemize}
            ]] .. "\n"
        end

        return s
    end
}

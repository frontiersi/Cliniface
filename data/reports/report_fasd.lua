function round( num, numDecimals)
    local mult = 10^(numDecimals or 0)
    return math.floor( num * mult + 0.5) / mult
end

report = {
    name = "Fetal Alcohol Syndrome Facial Analysis Report",
    title = "FAS Facial Analysis Report",

    available = function( m)
        return m:hasLandmarks()
    end,

    content = function( r, m)   -- r is Report, m is FaceModel

        -- First column
        s = [[\begin{minipage}[t]{.5\linewidth}  % First column]] .. "\n"

        s = s .. "\n" .. r:makeScanInfo()
        s = s .. r:makeFigure( 100, 150, "") .. "\n"

        --s = s .. "\\hfill \\break \n"
        s = s ..  r:showNotes() .. "\n"

        s = s .. [[\end{minipage}]] .. "\n"

        -- Second column

        msf = m:metrics()  -- Front metric set
        msl = m:metricsL() -- Left metric set
        msr = m:metricsR() -- Right metric set

        s = s .. [[\begin{minipage}[t]{.5\linewidth}  % Second column]] .. "\n"
        s = s .. r:makeChart( 10, 0) .. "\n\n\n"

        -- Palpebral fissure
        -- Z-scores
        pflz = msl:get(10):zscore(0) -- left
        pfrz = msr:get(10):zscore(0) -- right
        pfmz = 0.5 * (pflz + pfrz)   -- mean
        pflz = round( pflz, 2)
        pfrz = round( pfrz, 2)
        pfmz = round( pfmz, 2)
        -- Values
        pflv = msl:get(10):value(0) -- left
        pfrv = msr:get(10):value(0) -- right
        pfmv = 0.5 * (pflv + pfrv)  -- mean
        pflv = round( pflv, 2)
        pfrv = round( pfrv, 2)
        pfmv = round( pfmv, 2)

        -- Inner Canthal
        icv = round( msf:get(8):value(0), 2)
        icz = round( msf:get(8):zscore(0), 2)

        -- Philtrum Curvature metrics
        pcv0 = round( msf:get(1001):value(0), 2)
        pcv1 = round( msf:get(1001):value(1), 2)
        pcv2 = round( msf:get(1001):value(2), 2)
        pcv3 = round( msf:get(1001):value(3), 2)

        -- Upper Lip Circularity
        ulc = round( msf:get(1000):value(0), 2)

        -- Upper Vermilion Height
        uvhv = round( msf:get(44):value(0), 2)
        uvhz = round( msf:get(44):zscore(0), 2)

        s = s .. [[
        \begin{table}[H]
            \centering
            \caption*{\textbf{Facial Measurements}}
            \begin{tabular}{|c|c|c|} \hline
                \textbf{Measurement Name} & \textbf{Value} & \textbf{Z-score} \\ \hline ]] .. "\n"
        s = s .. "\tLeft Palpebral Fissure Length \\footnotemark[1] & "  .. pflv .. " mm & "      .. pflz  .. " \\\\ \\hline \n"
        s = s .. "\tRight Palpebral Fissure Length \\footnotemark[1] & " .. pfrv .. " mm & "      .. pfrz  .. " \\\\ \\hline \n"
        s = s .. "\tMean Palpebral Fissure Length \\footnotemark[1] & "  .. pfmv .. " mm & "      .. pfmz  .. " \\\\ \\hline \n"
        s = s .. "\tInner Canthal Distance \\footnotemark[2] & "  ..  icv .. " mm & "      ..  icz  .. " \\\\ \\hline \n"
        s = s .. "\tUpper Vermilion Height \\footnotemark[3] & "  .. uvhv .. " mm & "      .. uvhz  .. " \\\\ \\hline \n"
        s = s .. "\tUpper Lip Circularity & "                ..  ulc .. " $p^2/A$ & " .. "n/a" .. " \\\\ \\hline \n"
        s = s .. "\tPhiltral Curvature ($\\kappa_{max}$) & " .. pcv0 .. "         & " .. "n/a" .. " \\\\ \\hline \n"
        s = s .. "\tPhiltral Curvature ($\\kappa_{min}$) & " .. pcv1 .. "         & " .. "n/a" .. " \\\\ \\hline \n"
        s = s .. "\tPhiltral Curvature ($\\mu$) & "          .. pcv2 .. "         & " .. "n/a" .. " \\\\ \\hline \n"
        s = s .. "\tPhiltral Curvature ($\\Phi$) & "         .. pcv3 .. "         & " .. "n/a" .. " \\\\ \\hline \n"
        s = s .. "\tLip-Philtrum Rank (Subjective) & "       .. "\\multicolumn{2}{|c|}{} \\\\ \\hline \n"
        s = s .. [[
            \end{tabular}
        \end{table}
        ]] .. "\n"

        pids = m:phenotypes() -- phenotype ids
        if ( #pids > 0) then
            s = s .. "\\hfill \\break \n"
            s = s .. [[
\centering
\textbf{General Phenotypic Indications} \\
\raggedleft
\begin{itemize}
]]
            for i, j in pids:pairs() do
                s = s .. "\t\\item " .. phenotype(j):name():toStdString() .. "\n"
            end

            s = s .. [[
\end{itemize}
]]
        end

        s = s .. [[\end{minipage}]] .. "\n"

        pfsrc = msl:get(10):source()
        icsrc = msf:get(8):source()
        vhsrc = msf:get(44):source()
        s = s .. "\\footnotetext[1]{" .. pfsrc .. "} \n"
        s = s .. "\\footnotetext[2]{" .. icsrc .. "} \n"
        s = s .. "\\footnotetext[3]{" .. vhsrc .. "} \n"

        return s
    end
}

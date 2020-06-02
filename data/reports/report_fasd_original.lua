report = {
    name = "Fetal Alcohol Spectrum Disorder (FASD) Analysis",
    title = "Fetal Alcohol Spectrum Disorder Facial Analysis",

    available = function( fm)
        return fm:currentAssessment():hasLandmarks()
    end,

    addContent = function( fm)
        addStartColumn()
        addScanInfo()
        addFigure( 100, 150, "")
        addNotes()
        addEndColumn()

        msf = fm:currentAssessment():metrics()  -- Front metric set
        msl = fm:currentAssessment():metricsL() -- Left metric set
        msr = fm:currentAssessment():metricsR() -- Right metric set

        age = fm:age()

        -- Palpebral fissure
        -- Z-scores
        pflz = msl:metric(10):zscore(age,0) -- left
        pfrz = msr:metric(10):zscore(age,0) -- right
        pfmz = 0.5 * (pflz + pfrz)   -- mean
        pflz = round( pflz, 2)
        pfrz = round( pfrz, 2)
        pfmz = round( pfmz, 2)
        -- Values
        pflv = msl:metric(10):value(0) -- left
        pfrv = msr:metric(10):value(0) -- right
        pfmv = 0.5 * (pflv + pfrv)  -- mean
        pflv = round( pflv, 2)
        pfrv = round( pfrv, 2)
        pfmv = round( pfmv, 2)

        -- Inner Canthal
        icv = round( msf:metric(8):value(0), 2)
        icz = round( msf:metric(8):zscore(age,0), 2)

        -- Philtrum Depth metrics
        pcv0 = round( msf:metric(2001):value(0), 2)
        pcv1 = round( msf:metric(2001):value(1), 2)
        pcv2 = round( msf:metric(2001):value(2), 2)

        -- Upper Lip Circularity
        ulc = round( msf:metric(1000):value(0), 2)

        -- Upper Vermilion Height
        uvhv = round( msf:metric(44):value(0), 2)
        uvhz = round( msf:metric(44):zscore(age,0), 2)

        -- Source references for footnotes
        fnmks = footnoteIndices( {10, 8, 44})

        addStartColumn()
        addGrowthCurvesChart( 10, 0, fnmks[10]) -- Note the use of the footnote mark here.

        addCustomLatex( [[
        \begin{table}[H]
            \centering
            \caption*{\textbf{Facial Measurements}}
            \begin{tabular}{|c|c|c|} \hline
                \textbf{Measurement Name} & \textbf{Value} & \textbf{Z-score} \\ \hline ]])

        addCustomLatex( "\tLeft Palpebral Fissure Length \\footnotemark[" .. fnmks[10] .. "] & "  .. pflv .. " mm & "      .. pflz  .. " \\\\ \\hline")
        addCustomLatex( "\tRight Palpebral Fissure Length \\footnotemark[" .. fnmks[10] .. "] & " .. pfrv .. " mm & "      .. pfrz  .. " \\\\ \\hline")
        addCustomLatex( "\tMean Palpebral Fissure Length \\footnotemark[" .. fnmks[10] .. "] & "  .. pfmv .. " mm & "      .. pfmz  .. " \\\\ \\hline")
        addCustomLatex( "\tInner Canthal Distance \\footnotemark[" .. fnmks[8] .. "] & "  ..  icv .. " mm & "      ..  icz  .. " \\\\ \\hline")
        --addCustomLatex( "\tUpper Vermilion Height \\footnotemark[" .. fnmks[44] .. "] & "  .. uvhv .. " mm & "      .. uvhz  .. " \\\\ \\hline")
        addCustomLatex( "\tUpper Vermilion Height & "           .. uvhv .. " mm & "      .. uvhz  .. " \\\\ \\hline")
        addCustomLatex( "\tUpper Lip Circularity & "            ..  ulc .. " $p^2/A$ & " .. "n/a" .. " \\\\ \\hline")
        addCustomLatex( "\tPhiltral Depth (P1) & "              .. pcv0 .. " mm      & " .. "n/a" .. " \\\\ \\hline")
        addCustomLatex( "\tPhiltral Depth (P2) & "              .. pcv1 .. " mm      & " .. "n/a" .. " \\\\ \\hline")
        addCustomLatex( "\tPhiltral Depth (P3) & "              .. pcv2 .. " mm      & " .. "n/a" .. " \\\\ \\hline")
        addCustomLatex( "\tLip-Philtrum Rank (Subjective) & "   .. "\\multicolumn{2}{|c|}{} \\\\ \\hline")

        addCustomLatex( [[\end{tabular} \end{table}]])

        addPhenotypicVariationsList()
        addEndColumn()

        addFootnoteSources( {10,8,44})
    end
}

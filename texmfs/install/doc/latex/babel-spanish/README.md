Spanish for babel 5.0p
======================

This is the babel style for the Spanish language (5.0p). 

History
-------

5.0a. 2007-02-21
Reimplemented in full, which some parts rewritten from scratch.  Added
the es- mechanism and the mexico option.  Many bug fixes

5.0d. 2008-05-25
Fixed two bugs: misplaced subscripts with lim and the like; problem
with \roman and hyperref

5.0e. 2008-07-06
Fixed '', which gobbled the space following.

[5.0f. Internal.] 

5.0g. 2008-07-19
Fixed '' again, which killed the preceding kerning 

5.0h.  2009-01-02
Added two package options: es-noenumerate, es-noitemize.

5.0i. 2009-05-11
Fixed bug with romanidx.

5.0j. 2010-05-23
Fixed a babel bug for spanish to work with french.
Fixed a bug with ifthen.
Fixed a bug with \... and \.

5.0k. 2011-10-06
Fixed an error when . had mathcode "8000 before loading spanish

5.0l (mistakenly identified as 5.0j). 2013-04-11
Adapted to new babel modifiers

5.0m. 2014-01-29
Fixed (rare) bug in font sizes when superscript o/a where used
Fixed bad spacing in tabular and accents in OT1 encoding
Restored code for ., accidentally deleted in previous version

5.0n. 2014-04-24
New macros \spanishdate, \spanishDate, which format dates
  (\today, \Today are now based on them)
Mainly internal changes, using the new tools in babel 3.9
Fixed error with \foreignlanguage in tabular

5.0o. 2016-01-15
New macro \spanishdatefirst
Fixed a bug when \alph reached tilde-n
Fixed \dotlessi with lua (but not \acute, yet)

5.0p. 2016-03-03
Fixed \acute with lua (it now uses the composite char)

----------
2016-03-03

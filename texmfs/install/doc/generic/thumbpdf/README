README for project thumbpdf 2014/07/15 v3.16

TABLE OF CONTENTS
=================
A. Project thumbpdf
B. Copyright, Disclaimer, License
C. Files
D. Requirements
E. Installation
F. User Interface
G. Author
H. Questions, Suggested Improvements
I. Known Problems
J. History

A. PROJECT THUMBPDF
==================
THUMBPDF adds thumbnails to TeX documents which will be converted
to PDF files. The following routes are supported by this package:
* pdfTeX
* dvips/ps2pdf (with Ghostscript version 6.0)
* dvips/Distiller (tested with versions 3 and 4)
* VTeX in PostScript mode (tested with versions 7.51, 7.53)
Because the support for dvips/ps2pdf, dvips/Distiller, and
VTeX/PS uses pdfmark specials both routes are shortly called
the pdfmark route in the following.

Both plain and LaTeX formats are supported.

Thumbnails are small images, pictograms or reduced images from
the pages of a document. They support the navigation through the
document if the viewer supports it (e. g. AcrobatReader).

The mode of action:
In the following the user TeX file is called `<jobname>.tex'.
0. The user compiles the TeX file to get `<jobname>.pdf'.
1. Now the perl script generates the thumbnails in the form
   of png images by the help of Ghostscript. Versions below
   5.50 are not recommended because of some bugs in the pdf
   support of Ghostscript (see section I. Known Problems).
2. Then pdfTeX compiles the driver file `thumbpdf.tex' which
   looks for these thumbnails and includes them as images.
   `thumbpdf.pdf' will contain on each page a thumbnail.
3. The perl script scans `thumbpdf.pdf' to get the thumbnails
   as pdf objects. They are written in data files:
   `<jobname>.tpt' for pdfTeX and
   `<jobname>.tpm' for the pdfmark route.
4. If the user compiles now his TeX file which loads package
   `thumbpdf.sty' then he gets a pdf file with thumbnails.

B. COPYRIGHT, DISCLAIMER, LICENSE
=================================
Copyright (C) 1999-2014 Heiko Oberdiek.

This work may be distributed and/or modified under the
conditions of the LaTeX Project Public License, either version 1.3
of this license or (at your option) any later version.
The latest version of this license is in
  http://www.latex-project.org/lppl.txt
and version 1.3 or later is part of all distributions of LaTeX
version 2003/12/01 or later.

This work has the LPPL maintenance status "maintained".

This Current Maintainer of this work is Heiko Oberdiek.

C. FILES
========
The project `thumbpdf' consists of four files:
thumbpdf(.pl): This perl script is the main program. The extension
               `.pl' may be omitted.
thumbpdf.tex:  A pdfTeX program to produce the thumbnail pdf
               file (A2.).
thumbpdf.sty:  The package for the user to be included in his TeX
               source (A4.)
README:        Documentation, the file you are reading.

Following temporary files are produced if `thumbpdf' is invoked:
thb*.png:      The thumbnails in PNG format:
               `thb1.png', `thb2.png', ..., `thb10.png', ...
thumbpdf.pdf:  The thumbnail pdf file (A2.).
thumbpdf.log:  The protocol file of the pdfTeX run (A2.).

The result is written to (A3.):
<jobname>.tpt: Thumbnail data file for pdfTeX.
               (tpt = Thumbnail PdfTex)
<jobname>.tpm: Thumbnail data file as pdfmark specials for
               the pdfmark route.
               (tpm = Thumbnail PdfMark)

Additional thumbnails for the `\thisthumb' feature have to be
declared in the file `<jobname>.tno'. This can also be done
with \DeclareThumbs in the user's TeX file. Package
`thumbpdf.sty' then writes the file `<jobmane>.tno'.
(tno = ThumbNail Optional)

D. REQUIREMENTS
===============
* Perl5 (version 5 of the perl interpreter).
* Ghostscript:
  * Thumbnail generation: version 5.50 or better 6.0 (see section I.)
  * Including thumbnails with ps2pdf: version 6.0.
* pdfTeX (also when pdfmark mode is used).

E. INSTALLATION
===============
1. TeX directory structure (TDS, see CTAN:tds/tds.pdf):
   The archive `thumbpdf.tds.zip' contains all the files sorted
   into a TDS tree (texmf tree):
     CTAN:install/support/thumbpdf.tds.zip
   For installation just unpack `thumbpdf.tds.zip' in the texmf tree
   of your choice and update the file data base when your
   distribution supports it (mikTeX, teTeX, TeX Live, ...).
   Check E.2 for installation of the perl script.

   Alternatively you can install the files manually:
   The files
     `readme.txt' (documentation)
     `thumbpdf.tex' (pdftex)
     `thumbpdf.sty' (pdf(e)tex, pdf(e)latex, (e)tex, (e)latex)
   go to
     texmf/doc/generic/thumbpdf/readme.txt
     texmf/tex/generic/thumbpdf/thumbpdf.tex
     texmf/tex/generic/thumbpdf/thumbpdf.sty
   (These now definite places in the TDS tree are a result of
   a discussion with Thomas Esser, who supports them in his
   great teTeX 1.0).

2. Perl script `thumbpdf.pl':

   Unix
   * Your are allowed to rename `thumbpdf.pl' to `thumbpdf':
       mv thumbpdf.pl thumbpdf
   * Ensure that the execute permission is set:
       chmod +x thumbpdf
   * Move the file to a directory where the shell can find it
     (environment variable PATH, e.g. /usr/local/bin/).

   Dos/Windows/(OS2)
   * Running the perl interpreter directly with the perl script:
       perl thumbpdf.pl ...
   * Methods for calling by typing the script name without extension
     and perl interpreter:
     a) If your perl distribution provides a pl2exe program,
        use it to generate `thumbpdf.exe'.
        Advantage: I/O redirection works.
     b) A good method is a dos program of John Dallman:
          #!perl.exe (versions below 4)
          hbperl.exe (version 4)
        http://www.perl.com/CPAN/authors/id/JDALLMAN/hbp_403.zip
        Move thumbpdf.pl in a PERLLIB directory and copy the
        exe program to `thumbpdf.exe'. Then the program looks
        for the perl interpreter, the script and calls them.
        Advantage: I/O redirection works.
     c) Windows NT 4.0 users can use associated file types:
          SET PATHEXT=.pl;%PATHEXT%
        See perl win32 faq "How do I associate Perl scripts with perl?":
        http://www.activestate.com/support/faqs/win32/perlwin32faq4.html
        Disadvantage: I/O redirection does not work.
     d) 4DOS: SET .PL=c:/bin/perl.exe
        See perl win32 faq "How can I get Perl to run a Perl script at
        the 4DOS command line by typing the name of the script without
        the extension or "perl", just like a regular exe file?":
        http://www.activestate.com/support/faqs/win32/perlwin32faq1.html
     e) Convert the perl script to a batch file `thumbpdf.bat', if your
        distribution provides `pl2bat.bat'.
        Disadvantage: I/O redirection does not work.
     Many of this methods are listed in the perl win32 faq
     "What's the equivalent of the shebang ("#!") syntax for Win32?":
     http://www.activestate.com/support/faqs/win32/perlwin32faq4.html
   * TeXLive 7+/fpTeX contain a compiled version "thumbpdf.exe".

   Dos
   * I do not know a precompiled Ghostscript interpreter for
     version 5.5 or 6.0. Version 5.10 misses the png devices.
     So you have to compile Ghostscript for dos or you
     can use another program to generate the thumbnails
     (names: `thb1.png', `thb2.png', ...).
   * If Ghostscript cannot be started by perl because
     of serious problems (segmentation faults, ...),
     option `printgscmd' can be used: Thumbpdf prints the command line
     arguments for the ghostscript call in the file `thumbpdf.gs'
     and finishes. Then Ghostscript can manually be invoked to
     create the png files, e.g.:
       gs386/gswin32c @thumbpdf.gs (DOS/Win3.1)
     You need a Ghostscript executable that contains the png devices.
     For example version 5.50 can be run under Win3.1/Win32s by making
     a program icon with the correct working directory and
     the command `PATH/gswin32(c) @thumbpdf.gs', e.g:
       c:\gstools\gs5.50\gswin32 -Ic:\gstools\gs5.50 @thumbpdf.gs
     In the next call of thumbpdf the Ghostscript step is omitted by
     option `nomakepng'.
     Example command sequence:
       thumbpdf --printgscmd test
       gswin32c @thumbpdf.gs
       thumbpdf --nomakepng test

   Windows 3.1x with win32s
   * The png files can be produced with gswin32(c), but the means
     of the operating system to start windows programs with command
     line arguments are very poor. I found no way to invoke Ghostscript
     via perl, so the thumbnails must be generated manually (see above).

F. USER INTERFACE
=================
* The perl script `thumbpdf.pl' can be invoked in two ways:
  a) thumbpdf --help
     prints a help screen with the options and some default values.
  b) thumbpdf [options] <jobname>[.pdf|.ps]
     `<jobname>' is the file name of user's TeX file without extension.
     The extension `.pdf' can be omitted. If option `--useps' is
     active, thumbpdf will look for `<jobname>.ps'.

  Action options (see section C.):
  --makepng:  Call of Ghostscript to generate the thumbnails (A1.).
  --makepdf:  Call of pdfTeX to produce a thumbnail pdf file (A2.).
  --makedata: Parsing of this pdf file and writing the bject
              represenation to `<jobname>.tpt' or `<jobname>.tpm' (A3.).
   --clean:   Removing of the temporary files.

  Configuration options for --makepng:
  --useps:      Use the PostScript file `<jobname>.ps' instead of the
                pdf file `<jobname>.pdf' for generating the thumbnails.
  --gscmd <name>: Name of the ghostscript command.
  --device|png [png]<dev>:     (dev = mono, gray, 16, 256, 16m)
                Ghostscript device for producing the thumbnails, e.g.:
                --device=mono, --device=pnggray, --png=256
  --resolution: Without the option thumbpdf checks the /MediaBox
                and calculates the resolution, that neither height
                nor width exceed the recommended 106 pixels.
                For a4 or letter paper the resolution is about 9 DPI.
                With this option, the default behaviour can be
                overwritten.
  --compress:   The compress level of the thumbnail images. There
                is no reason not to use the maximal value, except
                for debugging or drafts.
                * Note for the pdfTeX route:
                  Value 10 has a special meaning: streams are compressed
                  like value 9, but unnecessary white spaces are removed
                  in the dictionaries
                * Note for Distiller:
                  The Distiller does not recognize compressed streams
                  and uses its own settings to compress the streams.
                  Because previously compressed streams are not
                  detected and /Filter entries are overwritten, the
                  double compressed streams will be invalid. Therefore
                  the PostScript code in `<jobname>.tpm' uncompresses
                  the streams, if Distiller is detected.
                  Although the compression of thumbpdf is quite useless
                  with Distiller, compression can be turned on for a
                  possible reuse with ps2pdf or to keep the file
                  `<jobname>.tpm' small.
  --password:   Password for an encrypted pdf file.
                The following patch is required for ghostscript:
                http://www.ozemail.com.au/~geoffk/pdfencrypt/pdf_sec.ps
  --antialias:  Set anti-aliasing parameters for ghostscript.
                It expects numbers with values 0, 1, 2, or 4.
                The meaning of 0 and 1 is the same and disables
                anti-aliasing.
                  If two numbers are given, the first sets
                anti-aliasing for text (TextAlphaBits), the second
                for graphics (GraphicsAlphaBits). If only one number
                is present, then it is taken for both text and graphics.
                Examples:
                --antialias=1   disable anti-aliasing
                --antialias=4   max. anti-aliasing
                --antialias=14  -dTextAlphaBits=1 -dGraphicsAlphaBits=4
  --greek:      The text is printed in Greek style, the characters are
                replaced by gray boxes. Three limitations:
                * Only fonts in Type 1 format are supported.
                * The `show' operator is redefined in `userdict'.
                  So it will not work, if the characters are displayed
                  by other methods.
                * The color is lightened, assuming a white background.
                Status: experimental.

  Configuration option for --makedata:
  --modes:      Select the modes for which you want that thumbpdf
                generates the thumbnail data files:
                * pdftex: <jobname>.tpt
                * pdfmark|dvips|ps2pdf|vtexpdfmark: <jobname>.tpm
                * all: synonym for the list `pdftex,pdfmark'
  --level2:     `<jobname>.tpm' is written with PostScript level 2
                features to reduce the size of this file.
                (This decreases the size of the final PostScript file,
                but does not affect the size of the final pdf file.)

  Options for message control:
  --quiet       Messages except warnings are suppressed.
  --verbose     Informations about the actions are printed.
  --debug       Additional messages for debugging purposes.
                Also option `--clean' is disabled.

  Environment variable `THUMBPDF':
  Before scanning the command line for options the perl script
  `thumbpdf.pl' looks for the environment variable `THUMBPDF'.
  Example (bash syntax):
    export THUMBPDF="--modes=pdfmark --level2"
  To avoid syntax errors the equal sign can be replaced by a space,
  for example (DOS syntax):
    SET THUMBPDF=--modes pdfmark --level2
  The reason for this environment variable is, that users of ps2pdf
  can omit the option 'modes', if they have set `THUMBPDF':
    SET THUMBDPF=--modes pdfmark
  Or if both data files should be generated:
    SET THUMBPDF=--modes all

* Package `thumbpdf.sty'
  * LaTeX format: \usepackage{thumbpdf}
    Options: pdftex, pdfmark, dvips, ps2pdf
    (the last three options are synonyms).
  * Plain format:   \input thumbpdf.sty
    Define \ThumbpdfDriver to inform `thumbpdf.sty' about the driver:
      \def\ThumbpdfDriver{ps2pdf}\input thumbpdf.sty
  * If you want to use option `pdfmark' in all cases exept pdfTeX in
    pdf mode, you can use a configuration file `thumbpdf.cfg':
      \ifx\ThumbpdfDriver\UnDeFiNeD
        \def\ThumbpdfDriver{pdfmark}%
      \fi
      \endinput
    This works, because `\ThumbpdfDriver' is ignored, if pdfTeX in
    pdf mode is detected. A more politically correct example of
    the configuration file `thumbpdf.cfg':
      \expandafter\ifx\csname ThumbpdfDriver\endcsname\relax
        \def\ThumbpdfDriver{pdfmark}%
        \ifx\pdfoutput\UnDeFiNeD
        \else
          \ifx\pdfoutput\relax
          \else
            \ifnum\pdfoutput>0
              \def\ThumbpdfDriver{pdftex}%
            \fi
          \fi
        \fi
      \fi
      \endinput
  * Because pdfTeX can be detected at run time, it is not necessary
    to specify driver `pdftex'; even the other drivers are ignored.
  * \thisthumb{name} uses the thumbnail mentioned in
    `<jobname>.tno' for this page. (This file can be written by
    the \DeclareThumbs feature, see below.)
  * \DeclareThumbs contains the content of the file `<jobname>.tno',
    see below.

* Additional thumbnails: `<jobname>.tno'
  If `<jobname>.tno' is found, the thumbnails are automatically
  included by the program `thumbpdf.tex'.
  Format of this file:
  \thumb{filename without `.png' extension}% or
  \thumb[name]{filename with/without extension}
  Examples:
    \thumb{one}%      --> \thisthumb{one}, recommended
    \thumb{one.png}%  --> \thisthumb{one.png}
    \thumb[two]{one}% --> \thisthumb{two}
    \thumb[one]{./extrathumb/one.png}% --> thisthumb{one}
  The first example shows that the `.png' extension
  is supported and recommended. But `.jpg' or `.tif'
  files also work.
  This data can be included in the main tex file that
  includes `thumbpdf.sty' with the \DeclareThumbs feature:
    \usepackage{thumbpdf}% or \input thumbpdf.sty
    \DeclareThumbs{
      \thumb{one}
      \thumb[two]{./extra/two.jpg}
    }
  Then the file `\jobname.tno' is written with the thumbnail
  declarations. (LaTeX's \nofiles is respected.)
* Program `thumbpdf.tex'
  Execept for debugging there is no reason for an user to call it
  manually. But if so the number of the files to be looked for can
  be reduced, because it takes some time to test 1000 thumbnails if
  they exist. A maximum number can be specified before:
    pdftex \def\thumbmax{20}\input thumbpdf
  The highest thumbnail is now `thumb020.png'.
  `<jobname>.tno' can only be found, if `thumbpdf.tex' knows about
  this `<jobname>':
    \pdftex \def\thumbjob{<jobname>}\input thumbpdf
  If `\thumbjob' is undefined, `thumbpdf.tex' looks for
  `thumbopt.tex'.

Example: Addition of thumbnails for `example.tex':
  pdfelatex example      % or pdf(e)(la)tex, whatever example needs.
  thumbpdf  example
  pdfelatex example      % example.tex contains \usepackage{thumbpdf}
or for the dvips/ps2pdf route:
  latex example
  dvips example
  ps2pdf example.ps example.pdf
  thumbpdf --modes=dvips example
  latex example       % example.tex contains \usepackage[dvips]{thumbpdf}
  dvips example
  ps2pdf example.ps example.pdf

* Option `clean':
  It clears the temporary files of the current thumbpdf run:
  * thumb*.png (by `makepng')
  * thumbpdf.pdf, thumbpdf.log (by `makepdf')
  Previously generated files are untouched. This option will
  be automatically disabled, if options `debug', `nomakepdf',
  or `nomakedef' are active.

G. AUTHOR
=========
Heiko Oberdiek
Email: heiko.oberdiek at googlemail.com

H. QUESTIONS, SUGGESTED IMPROVEMENTS
====================================
If you have questions, problems with `thumbpdf', error reports,
if you have improvements or want to have additional features,
please send them to the author.

Other ps->pdf routes:
* pstill: I did not succeed. It seems that the used pdfmark specials
  are not supported.
* Adobe Distiller: Because I do not have access to it, I am curious
  to hear whether it works. Theoretically it should.

I. KNOWN PROBLEMS
=================
* Compressed PDF objects (version PDF-1.5) are not supported,
  thus the PDF file should be generated with \pdfobjcompresslevel=0,
  before running thumbpdf.
* The most bug reports I get are perl script error messages regarding
  Ghostscript. Especially versions below 5.50 often have problems
  with the pdf support. Sometimes it can be necessary to update
  to version 6.0.
  Before writing a bug report, please check, if your pdf file
  is processible by your Ghostscript version. If upgrading do not
  help, you have two possibilities:
  a) Use of another program to generate the thumbnails. But
     an other program that uses Ghostscript to do the job will not
     help of course.
     The thumbnails should be named `thb1.png', `thb2.png', ...
     Then run `thumbpdf' and omit the Ghostscript step by the
     option `--nomakepng'.
  b) Generate a PostScript version of your TeX file and run
     `thumbpdf' with option `--useps'. Getting the PostScript
     file by Acrobat's conversion feature often fails. So it is
     better to generate the PostScript file from the TeX source
     with TeX/dvips, for example.
* Incompatibility with option `pdfpagetransition' of package `hyperref'
  versions below 6.64. This option overwrites the previous contents
  of `\pdfpageattr', used by `thumbpdf.sty'.
* If there is a package that overloads `\shipout' to rearrange pages
  (two pages to one, ...), then `thumbpdf.sty' should be loaded first.
* Some builds of the perl port of ActiveState, version 5.6.0
  for Windows do not work correctly: for example, after
  a successful call of system commands with the backtick
  operator (``), $? ($CHILD_ERROR) returns -1 instead of 0.
  Summary of informations contributed by users:
  - These builds for Windows do not work: 612, 613, 616
  ? Windows port: 618 (one user suceeds, one user fails)
  + The following Windows builds work: 522
  + The Linux versions work.
  Workaround: Use another version or port,
  see `http://www.cpan.org/'.

J. HISTORY
==========
1999/02/14 v1.0 first release
1999/02/23 v1.1
  * Bug in thumbpdf.sty removed (#2->#1).
  * New option in thumbpdf.pl: --resolution.
  * Automatic calculation of the resolution for Ghostscript
    (max. size 106x106).
1999/03/01 v1.2
  * Optimizition: indirect objects for length values removed.
1999/03/12 v1.3
  * Copyright: LPPL
1999/05/05 v1.4
  * Detecting of cygwin-perl.
  * Save memory by sharing `RGB' objects (devices: png16, png256).
1999/06/13 v1.5
  * Gs detection extended (thumbpdf.pl).
  * Installation part of readme.txt rewritten and table of contents
    added.
1999/07/27 v1.6
  * Bug in thumbpdf.sty fixed (handling of \pdfpageattr).
  * Warning if there are missing thumbs (thumbpdf.sty).
  * Added section `Known Problems' in `readme.txt'.
1999/08/08 v1.7
  * Bug in thumbpdf.sty fixed. This bug was introduced in
    version 1.6, therefore this version should not be used.
  * Adaptation for pdfTeX version 0.14a.
  * LPPL 1.1
1999/09/09 v1.8
  * Bug in thumbpdf.sty fixed. \PackageWarning cannot be used
    in \shipout, because \protect has been set to \noexpand
    (mysterious LaTeX behaviour?).
1999/09/16 v1.9
  * The perl script now detects direct /Length objects,
    that pdfTeX uses including JPEG images.
2000/01/11 v1.10
  * Bug in /Length detection removed.
  * Direct /Length in RGB objects supported.
  * \immediate before \pdfximage removed.
  * thumbpdf.sty revised.
2000/01/19 v1.11
  * Small fix for perl versions that cannot handle "my"
    in "for" loops.
  * LPPL 1.2
2000/02/11 v1.12 (not released)
  * Option `clean' added.
  * `<jobname>.tnd' replaces `thumbpdf.tex', if thumbpdf is called:
     thumbpdf [options] <jobname>[.pdf]
  * `<jobname>.tno' can be used instead of `thumbopt.tex'.
  * \DeclareThumbs added.
2000/02/22 v2.0
  * Support for the dvips/ps2pdf route by pdfmark specials.
  * Temporary files are removed.
  * Renaming of files:
    thumb%03d.png (thumb001.png) ==> thb%d.png (thb1.png)
    thumbdta.tex ==> <jobname>.tpt, <jobname>.tpm added
    thumbopt.tex ==> <jobname>.tno
  * Syntax change: <jobname> is now required.
2000/02/28 v2.1
  * Environment variable `THUMBPDF' supported.
  * `thumbpdf.cfg' supported.
  * TDS place for readme.txt: texmf/doc/generic/thumbpdf/readme.txt
  * Errors in `readme.txt' corrected.
2000/03/07 v2.2
  * Support for Distiller 3 and 4, the streams are uncompressed.
  * Call of gs is changed in order to show the currently processed
    page number of the pdf file.
  * Option --printgscmd creates the command line file `thumbpdf.gs'
    for the Ghostscript call.
2000/03/22 v2.3
  * Bug fix: --useps now works.
2000/04/10 v2.4
  * Fix for ActiveState Perl 5.6.0: uc line changed, fork removed.
    (Thanks to Andreas Buehmann <andreas.buehmann@gmx.de>.)
  * Test of `thumbpdf.tex' version.
2000/07/29 v2.5
  * Save trick in call of ghostscript.
  * Undocumented option --gspages added.
  * Problem with ActiveState perl 5.6.0 (win32) added
    to section "Known Problems".
2000/09/27 v2.6
  * Information about ActiveState perl 5.6.0 updated.
2000/10/27 v2.7
  * -dFIXEDMEDIA=0 added in gs call.
2001/01/12 v2.8:
  * Bug fix in dvips mode and active option `level2':
    pack parameter corrected for little-endian machines.
  * /Rotate in pdf pages:
    ghostscript versions around 6.01 have added a hack
    in /pdfshowpage_setpage, that ignores the /Rotate entry.
    A patch is added to disable the hack.
  * Ghostscript uses the MediaBox for calculating the
    page size. For version 6.50 a patch is added to use
    the /CropBox instead.
2001/03/29 v2.9:
  * Option --password added.
2001/04/02 v2.10:
  * Small corrections in readme.txt, contributed by Michael Wiedmann.
2001/04/26 v2.11:
  * Option --antialias added (suggestion of Juergen Bausa).
  * This version is not released.
2002/01/11 v3.0:
  * Syntax of option --antialias changed. The option is enabled
    as default, so that the ghostscript step is now slower.
  * Support for VTeX's PS mode added.
  * Greek mode added (experimental).
  * Signal handlers added for cleanup.
2002/05/26 v3.1:
  * SIG_HUP unkown in Windows.
  * Bug fix: The signal function for __DIE__ "cleanup" aborts
    before the error message of "die" is printed.
    Replaced by "clean" that does not contain "exit 1".
  * Small bug fix in mode detection and mode "vtex"
    removed from list. "vtexpdfmark" was detected,
    "vtex" did not work and perhaps it will be used
    later for VTeX in PDF mode.
2002/05/26 v3.2:
  * Fix: "MacOS/X: darwin" is now not interpreted as
    Windows.
2003/03/19 v3.3:
  * Fix for gs 8.00 in mode dvips:
    THB_DistillerPatch also applied to ghostscript >= 8.00.
2003/06/06 v3.4:
  * Bug fix, two forgotten "pop"s added for Distiller case.
2004/10/24 v3.5:
  * Revert Cygwin detection: is unix (request by Jan Nieuwenhuizen).
  * LPPL 1.3.
2004/11/19 v3.6:
  * Bug fix for dvips mode and gs < 8.00 (/stackunderflow in pop).
2004/11/19 v3.7:
  * For easier debugging, the special thumbpdf objects are now
    valid PDF objects that are referenced in the Catalog.
  * Remove of extra '\n' before "endstream" that is added
    by pdfTeX 1.20a.
2005/07/06 v3.8:
  * Fix because of pdfTeX 1.30.
2007/11/07 v3.9:
  * Deprecation warning of perl 5.8.8 fixed.
2008/04/16 v3.10:
  * TDS archive `thumbpdf.tds.zip' provided.
2010/07/07 v3.11:
  * \pdfcompresslevel=0 and \pdfminorversion=4 added for thumbpdf.tex.
  * \input is used with file name extension for "thumbpdf.tex".
  * Warning if \pdfobjcompresslevel>0.
2011/08/09 v3.12:
  * Support for LuaTeX added.
2011/08/10 v3.13:
  * Use gswin64c in Windows with 64 bits.
  * Fix of version date of thumbpdf.sty.
2012/04/09 v3.14:
  * Fix for plain TeX compatibility (offending \RequirePackage).
2012/04/18 v3.15:
  * Option --version added.
  * File `readme.txt' renamed to `README'.
2014/07/15 v3.16
  * Patch for `thumbpdf.pl' by Norbert Preining because of   
    pdfTeX 1.40.15 (TeX Live 2014).

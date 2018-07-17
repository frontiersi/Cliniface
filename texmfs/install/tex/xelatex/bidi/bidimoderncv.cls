%%
%% This is file `bidimoderncv.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% bidi.dtx  (with options: `table,bidimoderncv.cls')
%% 
%%   __________________________________________________
%%   Copyright (c) 2009--2017  Vafa Khalighi <persian-tex@tug.org>
%% 
%%   It may be distributed and/or modified under the LaTeX Project Public License,
%%   version 1.3c or higher (your choice). The latest version of
%%   this license is at: http://www.latex-project.org/lppl.txt
%% 
%%   This work is “author-maintained” (as per LPPL maintenance status)
%%   by Vafa Khalighi.
%% 
%% 
%% \CheckSum{48021}
%% \CharacterTable
%%  {Upper-case    \A\B\C\D\E\F\G\H\I\J\K\L\M\N\O\P\Q\R\S\T\U\V\W\X\Y\Z
%%   Lower-case    \a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z
%%   Digits        \0\1\2\3\4\5\6\7\8\9
%%   Exclamation   \!     Double quote  \"     Hash (number) \#
%%   Dollar        \$     Percent       \%     Ampersand     \&
%%   Acute accent  \'     Left paren    \(     Right paren   \)
%%   Asterisk      \*     Plus          \+     Comma         \,
%%   Minus         \-     Point         \.     Solidus       \/
%%   Colon         \:     Semicolon     \;     Less than     \<
%%   Equals        \=     Greater than  \>     Question mark \?
%%   Commercial at \@     Left bracket  \[     Backslash     \\
%%   Right bracket \]     Circumflex    \^     Underscore    \_
%%   Grave accent  \`     Left brace    \{     Vertical bar  \|
%%   Right brace   \}     Tilde         \~}
%%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{bidimoderncv}
\newif\if@DEBUG\@DEBUGfalse
\DeclareOption{a4paper}{
  \setlength\paperheight{297mm}
  \setlength\paperwidth{210mm}}
\DeclareOption{a5paper}{
  \setlength\paperheight{210mm}
  \setlength\paperwidth{148mm}}
\DeclareOption{b5paper}{
  \setlength\paperheight{250mm}
  \setlength\paperwidth{176mm}}
\DeclareOption{letterpaper}{
  \setlength\paperheight{11in}
  \setlength\paperwidth{8.5in}}
\DeclareOption{legalpaper}{
  \setlength\paperheight{14in}
  \setlength\paperwidth{8.5in}}
\DeclareOption{executivepaper}{
  \setlength\paperheight{10.5in}
  \setlength\paperwidth{7.25in}}
\DeclareOption{landscape}{
  \setlength\@tempdima{\paperheight}
  \setlength\paperheight{\paperwidth}
  \setlength\paperwidth{\@tempdima}}
\newcommand\@ptsize{}
\DeclareOption{10pt}{\renewcommand\@ptsize{0}}
\DeclareOption{11pt}{\renewcommand\@ptsize{1}}
\DeclareOption{12pt}{\renewcommand\@ptsize{2}}
\DeclareOption{draft}{\setlength\overfullrule{5pt}}
\DeclareOption{final}{\setlength\overfullrule{0pt}}
\newif\if@colour\@colourtrue
\DeclareOption{nocolour}{\@colourfalse}
\ExecuteOptions{a4paper,11pt,colour,final}
\ProcessOptions\relax
\input{size1\@ptsize.clo}
\RequirePackage{color}
\RequirePackage{graphicx}
\RequirePackage{ifthen}
\RequirePackage{marvosym}
\addtolength{\oddsidemargin}{-54pt}
\addtolength{\textwidth}{109pt}
\addtolength{\topmargin}{-70pt}
\addtolength{\textheight}{122pt}
\addtolength{\marginparsep}{-5pt}
\addtolength{\marginparwidth}{-10pt}
\RequirePackage{url}
\RequirePackage{hyperref}
\hypersetup{%
  breaklinks,
  baseurl       = http://,%
  pdfborder     = 0 0 0,%
  pdfpagemode   = UseNone,%
  pdfstartpage  = 1}
\AtEndOfClass{%
  \AtBeginDocument{%
    \hypersetup{%
      pdfauthor     = \@firstname~\@familyname,%
      pdftitle      = \@title,%
      pdfsubject    = \@firstname~\@familyname,%
      pdfkeywords   = \@resumename~\@firstname~\@familyname}}}
\urlstyle{tt}
\RequirePackage{fancyhdr}
\fancypagestyle{plain}{%
  \renewcommand{\headrulewidth}{0pt}
  \renewcommand{\footrulewidth}{0pt}
  \fancyhf{}}
\pagestyle{plain}
\setlength\lineskip{1\p@}
\setlength\normallineskip{1\p@}
\renewcommand\baselinestretch{}
\setlength{\parindent}{0pt}
\setlength{\parskip}{0pt}
\setlength\columnsep{10\p@}
\setlength\columnseprule{0\p@}
\pagestyle{empty}
\pagenumbering{arabic}
\raggedbottom
\onecolumn
\providecommand*{\listitemsymbol}{\textbullet}
\providecommand*{\addresssymbol}{}
\providecommand*{\mobilesymbol}{\Mobilefone}
\providecommand*{\phonesymbol}{\Telefon}
\providecommand*{\faxsymbol}{\FAX}
\providecommand*{\emailsymbol}{\Letter}
\definecolor{firstnamecolor}{rgb}{0,0,0}
\def\firstnamecolour#1{\gdef\@firstnamecolour{\addfontfeature{Color=#1}}}
\def\familynamecolour#1{\gdef\@familynamecolour{\addfontfeature{Color=#1}}}
\def\titlecolour#1{\gdef\@titlecolour{\addfontfeature{Color=#1}}}
\def\addresscolour#1{\gdef\@addresscolour{\addfontfeature{Color=#1}}}
\def\quotecolour#1{\gdef\@quotecolour{\addfontfeature{Color=#1}}}
\def\sectiontitlecolour#1{\gdef\@sectiontitlecolour{\addfontfeature{Color=#1}}}
\definecolor{sectionrectanglecolor}{rgb}{0,0,0}
\def\subsectioncolour#1{\gdef\@subsectioncolour{\addfontfeature{Color=#1}}}
\def\hintcolour#1{\gdef\@hintcolour{\addfontfeature{Color=#1}}}
\providecommand*{\firstnamefont}{}
\providecommand*{\familynamefont}{}
\providecommand*{\titlefont}{}
\providecommand*{\addressfont}{}
\providecommand*{\quotefont}{}
\providecommand*{\sectionfont}{}
\providecommand*{\subsectionfont}{}
\providecommand*{\hintfont}{\small}
\providecommand*{\firstnamestyle}[1]{{\firstnamefont\@firstnamecolour#1}}
\providecommand*{\familynamestyle}[1]{{\familynamefont\@familynamecolour#1}}
\providecommand*{\titlestyle}[1]{{\titlefont\@titlecolour#1}}
\providecommand*{\addresstyle}[1]{{\addressfont\@addresscolour#1}}
\providecommand*{\quotestyle}[1]{{\quotefont\@quotecolour#1}}
\providecommand*{\sectionstyle}[1]{{\sectionfont\@sectiontitlecolour#1}}
\providecommand*{\subsectionstyle}[1]{{\subsectionfont\@subsectioncolour#1}}
\providecommand*{\hintstyle}[1]{{\hintfont\@hintcolour#1}}
\newcommand*{\cvtheme}[2][]{
  \def\@cvtheme{#2}
  \def\@cvthemeoptions{#1}}
\cvtheme{bidi-casual}
\AtBeginDocument{\RequirePackage[\@cvthemeoptions]{cvtheme\@cvtheme}}
\newcommand*{\resumename}[1]{\def\@resumename{#1}}
\newcommand*{\firstname}[1]{\def\@firstname{#1}}
\newcommand*{\familyname}[1]{\def\@familyname{#1}}
\renewcommand*{\title}[1]{\def\@title{#1}}
\newcommand*{\address}[1]{\def\@address{#1}}
\newcommand*{\mobile}[1]{\def\@mobile{#1}}
\newcommand*{\phone}[1]{\def\@phone{#1}}
\renewcommand*{\fax}[1]{\def\@fax{#1}}
\newcommand*{\email}[1]{\def\@email{#1}}
\newcommand*{\extrainfo}[1]{\def\@extrainfo{#1}}
\def\@photowidth{0pt}
\newcommand*{\photo}[2][64pt]{\def\@photowidth{#1}\def\@photo{#2}}
\newcommand*{\quote}[1]{\def\@quote{#1}}
\newlength{\quotewidth}
\newlength{\hintscolumnwidth}
\newlength{\separatorcolumnwidth}
\setlength{\separatorcolumnwidth}{0.025\textwidth}
\newlength{\maincolumnwidth}
\newlength{\doubleitemmaincolumnwidth}
\newlength{\listitemsymbolwidth}
\settowidth{\listitemsymbolwidth}{\listitemsymbol{}~}
\newlength{\listitemmaincolumnwidth}
\newlength{\listdoubleitemmaincolumnwidth}
\newcommand*{\recomputethemelengths}{}
\newcommand*{\recomputelengths}{%
\setlength{\quotewidth}{0.65\textwidth}%
\setlength{\maincolumnwidth}{\textwidth}%
\addtolength{\maincolumnwidth}{-\separatorcolumnwidth}%
\addtolength{\maincolumnwidth}{-\hintscolumnwidth}%
\setlength{\listitemmaincolumnwidth}{\maincolumnwidth}%
\addtolength{\listitemmaincolumnwidth}{-\listitemsymbolwidth}%
\setlength{\doubleitemmaincolumnwidth}{\maincolumnwidth}%
\addtolength{\doubleitemmaincolumnwidth}{-\hintscolumnwidth}%
\addtolength{\doubleitemmaincolumnwidth}{-\separatorcolumnwidth}%
\setlength{\doubleitemmaincolumnwidth}{0.5\doubleitemmaincolumnwidth}%
\setlength{\listdoubleitemmaincolumnwidth}{\maincolumnwidth}%
\addtolength{\listdoubleitemmaincolumnwidth}{-\listitemsymbolwidth}%
\setlength{\listdoubleitemmaincolumnwidth}{0.475\listdoubleitemmaincolumnwidth}%
\renewcommand{\headwidth}{\textwidth}%
\recomputethemelengths}
\setlength{\hintscolumnwidth}{0.15\textwidth}
\recomputelengths
\title{}
\renewcommand*{\maketitle}{}
\newcommand*{\section}[1]{%
\vspace*{2.5ex \@plus 1ex \@minus .2ex}%
  \phantomsection{}%
  \addcontentsline{toc}{part}{#1}%
  \parbox[m]{\hintscolumnwidth}{\bidi@raggedleft\hintfont{\leavevmode\color{sectionrectanglecolor}\rule{\hintscolumnwidth}{1ex}}}%
  \hspace{\separatorcolumnwidth}%
  \parbox[m]{\maincolumnwidth}{\sectionstyle{#1}}\\[1ex]}
\newcommand*{\subsection}[1]{%
  \cvline[0.45em]{}{\subsectionstyle{#1}}}
\newcommand*{\cvline}[3][.25em]{%
  \begin{tabular}{@{}p{\hintscolumnwidth}@{\hspace{\separatorcolumnwidth}}p{\maincolumnwidth}@{}}%
    \bidi@raggedleft\hintfont{#2} &{#3}%
  \end{tabular}\\[#1]}
\newcommand*{\cvdoubleitem}[4]{%
 \cvline{#1}{\begin{minipage}[t]{\doubleitemmaincolumnwidth}#2\end{minipage}%
 \hfill%
 \begin{minipage}[t]{\hintscolumnwidth}\bidi@raggedleft\hintfont{#3}\end{minipage}\hspace*{\separatorcolumnwidth}\begin{minipage}[t]{\doubleitemmaincolumnwidth}#4\end{minipage}}}
\newcommand*{\cvlistitem}[2][\listitemsymbol{}]{%
  \cvline[0pt]{}{#1~\begin{minipage}[t]{\listitemmaincolumnwidth}#2\end{minipage}}}
\newcommand*{\cvlistdoubleitem}[3][\listitemsymbol{}]{%
  \cvline[0pt]{}{#1~\begin{minipage}[t]{\listdoubleitemmaincolumnwidth}#2\end{minipage}%
  \hfill%
  \ifthenelse{\equal{#3}{}}%
    {}%
    {#1~\begin{minipage}[t]{\listdoubleitemmaincolumnwidth}#3\end{minipage}}}}
\newcommand*{\cventry}[6]{%
  \cvline{#1}{%
    {\bfseries#2}%
    \ifx#3\else{, {\slshape#3}}\fi%
    \ifx#4\else{, #4}\fi%
    \ifx#5\else{, #5}\fi%
    .%
    \ifx#6\else{\newline{}\begin{minipage}[t]{\linewidth}\small#6\end{minipage}}\fi
    }}%
\newcommand*{\cvlanguage}[3]{%
  \cvline{#1}{\begin{minipage}[t]{.225\maincolumnwidth}\textbf{#2}\end{minipage}\hfill\begin{minipage}[t]{0.725\maincolumnwidth}\bidi@raggedleft\footnotesize\itshape #3\end{minipage}}}
\newcommand*{\cvcomputer}[4]{%
  \cvdoubleitem{#1}{\small#2}{#3}{\small#4}}
\newcommand*{\link}[2][]{%
  \ifthenelse{\equal{#1}{}}%
    {\href{#2}{#2}}%
    {\href{#2}{#1}}}
\newcommand*{\httplink}[2][]{%
  \ifthenelse{\equal{#1}{}}%
    {\href{http://#2}{#2}}%
    {\href{http://#2}{#1}}}
\newcommand*{\emaillink}[2][]{%
  \ifthenelse{\equal{#1}{}}%
    {\href{mailto:#2}{#2}}%
    {\href{mailto:#2}{#1}}}
\newif\if@displaypagenumbers\@displaypagenumberstrue
\newcommand*{\nopagenumbers}{\@displaypagenumbersfalse}
\AtBeginDocument{%
  \if@displaypagenumbers%
    \@ifundefined{r@lastpage}{}{%
      \ifthenelse{\pageref{lastpage} > 1}{%
      \fancypagestyle{plain}{%
        \fancyfoot[r]{\beginL\addressfont\@quotecolour\footnotesize\thepage\beginL/\endL\pageref{lastpage}\endL}}%
      \pagestyle{plain}}{}}%
  \AtEndDocument{\label{lastpage}}%
  \fi}
\newlength{\bibindent}
\setlength{\bibindent}{1.5em}
\newcommand*{\bibliographyitemlabel}{\includegraphics{bibitem.pdf}}
\newenvironment{thebibliography}[1]%
  {%
    \section{\refname}%
    \small%
    \if@RTL\if@LTRbibitems\@RTLfalse\else\fi\else\if@RTLbibitems\@RTLtrue\else\fi\fi
    \begin{list}{\bibliographyitemlabel}%
      {%
        \setlength{\topsep}{0pt}%
        \setlength{\labelwidth}{\hintscolumnwidth}%
        \setlength{\labelsep}{\separatorcolumnwidth}%
        \leftmargin\labelwidth%
        \advance\leftmargin\labelsep%
        \@openbib@code%
        \usecounter{enumiv}%
        \let\p@enumiv\@empty%
        \renewcommand\theenumiv{\@arabic\c@enumiv}}%
        \sloppy\clubpenalty4000\widowpenalty4000%
  }%
  {%
    \def\@noitemerr{\@latex@warning{Empty `thebibliography' environment}}%
    \end{list}%
  }
\newcommand\newblock{\hskip .11em\@plus.33em\@minus.07em}
\let\@openbib@code\@empty
\endinput
%%
%% End of file `bidimoderncv.cls'.

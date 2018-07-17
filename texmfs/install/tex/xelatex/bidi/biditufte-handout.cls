%%
%% This is file `biditufte-handout.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% bidi.dtx  (with options: `table,biditufte-handout.cls')
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
\NeedsTeXFormat{LaTeX2e}[1994/06/01]
\ProvidesClass{biditufte-handout}[2011/06/18 v0.1 biditufte-handout class]
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{bidituftefloat}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{bidituftetoc}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{bidituftehyperref}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{bidituftetitle}}
\ProcessOptions\relax
\LoadClass{article}
\RequirePackage{bidituftefloat}
\RequirePackage{bidituftesidenote}
\RequirePackage{bidituftetoc}
\RequirePackage{bidituftegeneralstructure}
\RequirePackage{bidituftehyperref}
\RequirePackage{bidituftetitle}
\renewenvironment{abstract}
  {\begin{quotation}}
  {\end{quotation}}
\AtBeginDocument{%
\IfFileExists{biditufte-handout.cfg}
  {\input{biditufte-handout.cfg}
   \ClassInfo{biditufte-handout}{Loading biditufte-handout configuration file \@gobble}}
  {}%
}
\endinput
%%
%% End of file `biditufte-handout.cls'.

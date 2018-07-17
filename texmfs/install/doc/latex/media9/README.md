# The `media9` LaTeX Package

© 2012--`\today` Alexander Grahn

## Description

This package provides an interface to embed interactive Flash (SWF) and 3D objects (Adobe U3D & PRC) as well as video and sound files or streams in the popular MP4, FLV, MP3 formats into PDF documents with Acrobat-9/X compatibility. Playback of multimedia files relies on Adobe Flash Player which supports the efficient H.264 codec for video compression.

This package is based on the RichMedia Annotation, an Adobe addition to the PDF specification. It replaces the now obsolete `movie15` package.

>Note that Adobe Reader for Linux has dropped Flash support since version 9.4.2.

Supported PDF making workflows are:
* pdfLaTeX, LuaLaTeX
* (Lua)LaTeX &rArr; dvips &rArr; ps2pdf/Distiller
* (Xe)LaTeX &rArr; (x)dvipdfmx

The package was written in the new LaTeX3 syntax.

## Requirements

* l3kernel (LaTeX package), version >=2012/07/16
* l3packages (LaTeX package), version >=2012/07/16
* pdfTeX, version >=1.30
* Ghostscript, version >=8.31 or Adobe Distiller for PS to PDF conversion
* dvipdfmx, version >=20120420 for DVI to PDF conversion
* Adobe Reader, version >=9

## Installation

MiKTeX and TeXLive users should run the package manager for installation.

Otherwise, a manual install into the local TeX-Directory-Structure (TDS) root directory can be done along the following steps:

1. Download the TDS compliant package file [`media9.tds.zip`](http://mirror.ctan.org/install/macros/latex/contrib/media9.tds.zip)

2. Find the local TDS root directory by running `kpsewhich -var-value TEXMFLOCAL` on the command line.

3. Unzip media9.tds.zip into the local TDS root directory previously found. Depending on the location of this directory, you may need to be logged in as Root/Administrator.

4. After installation, update the filename database by running `texhash` on the command line. Again, Root/Administrator privileges may be required.

For updating the package, repeat the steps given above.

## License

This material is subject to the [LaTeX Project Public License](http://mirrors.ctan.org/macros/latex/base/lppl.txt).

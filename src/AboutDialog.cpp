/************************************************************************
 * Copyright (C) 2017 Richard Palmer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <AboutDialog.h>
#include <ui_AboutDialog.h>
using Cliniface::AboutDialog;
#include <Cliniface_Config.h>


void AboutDialog::insertHeader()
{
    QStringList cnt;
    cnt << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
        << "<html><head><meta name=\"qrichtext\" content=\"1\" />"
        << "<style type=\"text/css\">"
        << "p, li { white-space: pre-wrap; }"
        << "</style></head>"
        << "<body style=\"font-family:'Noto Sans'; font-size:9pt; font-weight:400; font-style:normal;\">"
        << "<p style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
        << "<span align=\"center\" style=\"font-size:11pt; font-weight:600;\">"
        << "<a name=\"title\" href=\"" << APP_WEBSITE << "\">"
        << "<img src=\":/logos/LARGE_LOGO\"/>"
        << "</a>"
        << "</span>"
        << "<br/>"
        << "</p>";
    ui->textBrowser->insertHtml( cnt.join('\n'));
}   // end insertHeader


namespace {
QString packString( QString label, QString hlink)
{
    return "  - " + label + " (<a href=\"" + hlink + "\">" + hlink + "</a>)<br/>";
}   // end packString

}   // end namespace


void AboutDialog::finishContent()
{
    QStringList appn;
    appn << APP_NAME << QString::fromWCharArray(L"\u00A9 2017,2018") << APP_AUTHOR_NAME
         << QString("(<a href=\"mailto:%0?Subject=%1 (BUG REPORT) version %2\">email developer</a>)").arg( APP_AUTHOR_EMAIL, APP_NAME, APP_VERSION_STRING);

    QStringList cnt;
    cnt << "<br/>"
        << "<p align=\"left\" style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
        << appn.join(' ') << "<br/>"
        << "Version " << APP_VERSION_STRING << "<br/>"
        << APP_NAME << " is open source under the"
        << " <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">GNU General Public License v3.0</a>" << "<br/>"
        << " Source code can be downloaded from <a href=\"" << APP_SOURCE << "\">" << APP_SOURCE << "</a><br/>"
        << " The developer is grateful to the authors of the following works used herein:" << "<br/>"
        << packString( "FaceTools", "https://github.com/richeytastic/FaceTools")
        << packString( "rFeatures", "https://github.com/richeytastic/rFeatures")
        << packString( "rModelIO", "https://github.com/richeytastic/rModelIO")
        << packString( "libbuild", "https://github.com/richeytastic/libbuild")
        << packString( "QTools", "https://github.com/richeytastic/QTools")
        << packString( "rVTK", "https://github.com/richeytastic/rVTK")
        << packString( "rlib", "https://github.com/richeytastic/rlib")
        << packString( "Dlib", "http://dlib.net")
        << packString( "Qt v5.10.1", "https://www.qt.io")
        << packString( "VTK v8.1.1", "https://www.vtk.org")
        << packString( "OpenCV", "https://opencv.org")
        << packString( "CGAL", "https://www.cgal.org")
        << packString( "Boost v1.64", "http://www.boost.org")
        << packString( "Eigen3", "http://eigen.tuxfamily.org")
        << packString( "LibICP", "https://github.com/symao/libicp")
        << packString( "QuaZip", "https://github.com/stachenov/quazip")
        << packString( "MiKTeX Portable", "https://miktex.org/portable")
        << packString( "IDTF to U3D converter", "http://www2.iaas.msu.ru/tmp/u3d")
        << packString( "Open Asset Import Library v3.3.1", "http://assimp.sourceforge.net")
        << packString( "Icons8", "https://icons8.com/")
        << "</p>"
        << "</body>"
        << "</html>";
    ui->textBrowser->insertHtml( cnt.join('\n'));
}   // end finishContent


void AboutDialog::appendPara( const QString& para)
{
    QStringList cnt;
    cnt << "<p style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
        << "<br/>"
        << para
        << "<br/>"
        << "</p>";
    ui->textBrowser->insertHtml( cnt.join('\n'));
}   // end appendPara


void AboutDialog::insertContent()
{
    insertHeader();

    QStringList p1, p2, p3;
    p1 << APP_NAME << "allows users to visualise and work with 3D images of faces stored"
       << "in a wide variety of file formats. The program supports concurrent viewing of different face"
       << "models in three adjacent view panes. Different face models can be superimposed upon one another"
       << "and different visualisations selected to inform the user of salient aspects of facial morphology."
       << "Automatic face detection and surface correspondence tools, along with data cleaning functions"
       << "such as cropping, smoothing, and resampling can assist in standardising for comparison data"
       << "coming from different sources. Characteristic landmarks are automatically mapped and data can"
       << "be saved out into a single standard format that archives together both the model data and"
       << "the meta-data extracted by " << APP_NAME << ".";

    appendPara( p1.join(' '));

    finishContent();
    ui->textBrowser->scrollToAnchor("title");
}   // end insertContent


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    connect( ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    ui->textBrowser->setOpenExternalLinks(true);
    insertContent();
    //setWindowFlags( Qt::WindowCloseButtonHint);
    layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
}   // end ctor


AboutDialog::~AboutDialog()
{
    delete ui;
}   // end dtor

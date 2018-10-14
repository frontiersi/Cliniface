/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
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
#include <MiscFunctions.h>  // FaceTools
#include <Cliniface_Config.h>


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    connect( ui->closeButton, &QPushButton::clicked, this, &AboutDialog::close);

    QStringList cnt;
    cnt << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
        << "<html><head><meta name=\"qrichtext\" content=\"1\" />"
        << "<style type=\"text/css\">"
        << "p, li { white-space: pre-wrap; }"
        << "</style></head>"
        << "<body style=\"font-family:'Noto Sans'; font-size:9pt; font-weight:400; font-style:normal;\">";
    ui->textBrowser->insertHtml( cnt.join('\n'));

    ui->textBrowser->insertHtml( QString("<a href=\"") + APP_WEBSITE + "\"><img src=\":/logos/LARGE_LOGO\"/></a><br><br>");

    ui->textBrowser->insertHtml( QString( APP_NAME) + " " + APP_VERSION_STRING + "<br>");
    ui->textBrowser->insertHtml( QString("Copyright 2018 ") + APP_ORGANISATION + "<br>");
    ui->textBrowser->insertHtml( QString("Developed by ") + APP_AUTHOR_NAME + "<br>");
    ui->textBrowser->insertHtml( QString("<a href=\"") + APP_WEBSITE + "\">" + APP_WEBSITE + "</a><br>");
    ui->textBrowser->insertHtml( QString("<a href=\"mailto:") + APP_CONTACT_EMAIL + "?Subject="
                                        + APP_NAME + "%20" + APP_VERSION_STRING + "\">" + APP_CONTACT_EMAIL + "</a><br><br>");

    ui->textBrowser->insertHtml( FaceTools::loadTextFromFile( ":/html/about.html").c_str());

    ui->textBrowser->insertHtml( "</body></html>");

    ui->textBrowser->scrollToAnchor("title");

    ui->textBrowser->setOpenExternalLinks(true);

    //setWindowFlags( Qt::WindowCloseButtonHint);
    layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
}   // end ctor


AboutDialog::~AboutDialog()
{
    delete ui;
}   // end dtor

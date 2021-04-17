/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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
#include <FaceTools/MiscFunctions.h>
#include <QString>
#include <Cliniface_Config.h>


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);
    setModal(true);
    connect( _ui->closeButton, &QPushButton::clicked, this, &AboutDialog::close);

    static const QString appName = APP_NAME;
    static const QString appVersion = APP_VERSION_STRING;
    static const QString appOrg = APP_ORGANISATION;
    static const QString appAuthor = APP_AUTHOR_NAME;
    static const QString appEmail = APP_CONTACT_EMAIL;
    static const QString website = APP_WEBSITE;
    static const QString cryear = APP_CR_YEARS;

    QString cnt;
    cnt  = R"~(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">)~";
    cnt += R"~(<html>
               <head>)~";
    cnt +=    "<title>" + appName + " | About</title>";
    cnt += R"~(<style type="text/css">
                    p, li { white-space: pre-wrap; }
               </style>
               </head>
               <body style="font-family:Helvetica; font-size:10pt;">)~";
    cnt += "<center>";
    cnt += R"~(<a href=")~" + website + R"~("><img src=":/logos/LARGE_LOGO"></a>)~";
    cnt += "<h3>" + appName + " version " + appVersion + " <a href=\"" + website + "\">" + website + "</a></h3>";
    cnt += "</center>";
    cnt += "<center>";
    cnt += "<p>&copy; " + cryear + " " + appOrg + " &amp; " + appAuthor + "<br>";
    cnt += "Developed by <a href=\"mailto:" + appEmail + "?Subject=" + appName + "%20version%20" + appVersion + "\">" + appAuthor + "</a>";
    cnt += "</p>";
    cnt += "</center><br>";

    _ui->textBrowser->insertHtml( cnt);
    _ui->textBrowser->insertHtml( FaceTools::loadTextFromFile( ":/data/ABOUT"));
    _ui->textBrowser->insertHtml( "</body></html>");

    _ui->textBrowser->scrollToAnchor("title");
    _ui->textBrowser->setOpenExternalLinks(true);

    setFixedSize( geometry().width(), geometry().height());
}   // end ctor


AboutDialog::~AboutDialog()
{
    delete _ui;
}   // end dtor

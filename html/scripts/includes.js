function includeHeader( title)
{
    document.write("<center><a href=\"https://cliniface.org\"><img src=\"images/logo_357x110.png\" alt=\"Cliniface Logo\"></a></center>");
    document.write("<center><h1>" + title + "</h1></center>");
    document.write("<hr>");
}   // end includeHeader


function includeFooter()
{
    document.write("<br>");
    document.write("<hr>");
    document.write("<center>");
    document.write("<a href=\"#_top\">Back to top</a>");
    document.write("<strong><p>&copy; 2018-2020 SIS Research Ltd &amp; Richard Palmer</p></strong>");
    document.write("</center>");
}   // end includeFooter


function showUnderConstruction()
{
    document.write("<br>")
    document.write("<center><img src=\"images/under_construction.png\"/></center>");
    includeFooter()
}   // end showUnderConstruction




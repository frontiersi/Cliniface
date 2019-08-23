function includeHeader( title)
{
    document.write("<center><a href=\"https://cliniface.org\"><img src=\"images/cface.svg\" alt=\"Cliniface Logo\" style=\"width:27%;\"/></a></center>");
    document.write("<center><h1>" + title + "</h1></center>");
    document.write("<center><a href=\"index.html\">Back to contents</a></center>");
    document.write("<hr>");
}   // end includeHeader


function includeFooter()
{
    document.write("<center>");
    document.write("<br>");
    document.write("<a href=\"#_top\">Back to top</a><br>");
    document.write("<a href=\"index.html\">Back to contents</a>");
    document.write("<strong><p>&copy; 2018/2019 Spatial Information Systems Research Limited &amp; Richard Palmer</p></strong>");
    document.write("</center>");
}   // end includeFooter


function showUnderConstruction()
{
    document.write("<br>")
    document.write("<center><img src=\"images/under_construction.png\"/></center>");
    includeFooter()
}   // end showUnderConstruction




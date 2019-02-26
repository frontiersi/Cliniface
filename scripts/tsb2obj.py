#!/usr/bin/env python
import sys
import os
import re
from pywinauto.application import Application
from pywinauto.timings import Timings


def loadTSB( w, tsbfilepath):
    w.menu_select("File -> Load")
    app.Load.Edit.set_text( tsbfilepath)
    app.Load.Open.click()


def exportOBJ( w, objfilepath):
    w.menu_select("File -> Export -> Surface")
    app.Export.Edit.set_text( objfilepath)
    app.Export.Save.click()
    if app.Export.Yes.exists():
        app.Export.Yes.click()  # May ask for confirmation if overwriting


def closeModel( w, modelname):
    w.menu_select("Edit -> Remove Selected Surface")
    app.window(title=modelname).Yes.click()


def addWalkedFiles( dpath, ext, lns):
    "Find and set in lns all files ending with .ext within directory dpath."
    uext = ".*\." + ext.upper()
    lext = ".*\." + ext.lower()
    for root, dirs, files in os.walk( dpath):
        for fn in files:
            if re.search( lext, fn) is not None or re.search( uext, fn) is not None:
                lns.append( os.path.join(root, fn))


def testFileOkay( ext, fname):
    "Return true iff the file exists and has the correct extension."
    if not os.path.exists(fname):
        return False
    froot, fext = fname.split(os.path.sep)[-1].split(".")
    if fext.lower() != ext:
        return False
    return True


def getFilePaths( ext, args):
    files = []
    for fname in args:
        if not os.path.exists(fname):
            print "Couldn't find", fname, "; skipping"
            continue

        # If a directory name is given, walk it to get all valid files within it
        if os.path.isdir(fname):
            addWalkedFiles(fname, ext, files)
            continue

        # A file that can either be an .ext itself or a text file containing a list of .ext files.
        froot, fext = fname.split(os.path.sep)[-1].split(".")
        if fext.lower() == ext:
            files.append(fname)
        elif fext.lower() == "txt":
            for tf in [x.strip() for x in open(fname).readlines()]:
                if testFileOkay( ext, tf):
                    files.append(tf)
                else:
                    print "%s is not a valid .%s file, or can't be found." % (tf, ext)
    return files


def printUsage( pname):
        print "Usage: %s exepath input [input2, input3, ...]" % pname
        print
        print " Export one or several .tsb files to .obj format automatically using 3dMDpatient.exe."
        print
        print " Several input arguments can be given, each of which can take the form of a path to a"
        print " single .tsb file, a path to a directory that will be walked to find .tsb files within"
        print " its hierarchy, or a text file (.txt extension) with rows taken as paths to .tsb files."
        print " Each exported .obj file will be written to the same directory as its input .tsb file."
        print
        print " NB preexisting .obj files are automatically overwritten without warning so use with care!"
        print
        print "Arguments:"
        print " exepath     Path to the 3dMDpatient.exe program."
        print " input       Either a path to a single .tsb, a directory to be walked to find .tsb files,"
        print "             or a text file with paths to .tsb files given on separate lines."


if __name__ == "__main__":
    if len(sys.argv) < 3:
        printUsage( sys.argv[0])
        sys.exit(1)

    exepath = sys.argv[1]
    if not exepath.endswith("3dMDpatient.exe"):
        print "Must point to the 3dMDpatient.exe program!"
        sys.exit(1)

    if not os.path.exists(exepath):
        print "Couldn't find 3dMDpatient.exe at '%s'" % exepath
        sys.exit(1)

    infiles = getFilePaths( "tsb", sys.argv[2:])
    if len(infiles) == 0:
        print "No valid input files specified!"
        sys.exit(1)

    n = len(infiles)
    outfiles = [x[0:-3] + "obj" for x in infiles]

    Timings.Slow()

    # Open the application
    app = Application().start(exepath)
    w = app.window(title="3dMDpatient")

    for i in range(0,n):
        tsbfilepath = infiles[i]
        objfilepath = outfiles[i]
        modelname = objfilepath.split(os.path.sep)[-1].split(".")[0]

        print "Exporting '%s' to .obj" % tsbfilepath
        
        loadTSB( w, tsbfilepath)
        exportOBJ( w, objfilepath)
        closeModel( w, modelname)

    # Finished - close the 3dMDpatient software.
    w.menu_select("File -> Exit")

    sys.exit(0)


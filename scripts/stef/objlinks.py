#!/usr/bin/env python
from win32com.client import Dispatch
import sys
import os


def getCSVHPOs( hpoParentDir):
    """Grabs a list of tuples of TXT files with the names of the containing folders (without suffix parentheticals)."""
    csvhpo = []
    hpodirs = os.listdir( hpoParentDir)
    for hpodir in hpodirs:
        hdir = os.path.join( hpoParentDir, hpodir)
        print hdir
        csvfiles = [x for x in os.listdir(hdir) if os.path.splitext(x)[1].lower().endswith('txt')]
        if len(csvfiles) == 0:
            print "Didn't find a TXT file in", hpodir, "! Skipping directory."
            continue
        if len(csvfiles) > 1:
            print "Found more than one TXT file in", hpodir, "! Using first..."
        csvfile = os.path.join( hpoParentDir, hpodir, csvfiles[0])
        csvhpo.append((csvfile,hdir))
    return csvhpo


def createShortcut( fsrc, fdst):
    """Creates a shortcut to fsrc at location fdst."""
    print 'Creating shortcut', fdst, ' --> ', fsrc
    shell = Dispatch('WScript.Shell')
    shortcut = shell.CreateShortCut(fdst)
    shortcut.Targetpath = fsrc
    shortcut.WorkingDirectory = ''
    shortcut.save()


def createOBJShortcuts( objdir, csvhdir):
    """Reads rows from a TXT and creates shortcuts where OK for HPO analysis."""
    nvalid = 0
    hlines = open( csvhdir[0]).readlines()
    for i in xrange(1,len(hlines)):
        hline = hlines[i].split('\t')
        if hline[15].lower() == 'y':
            nvalid += 1
            scanid = str(int(float(hline[11]))).zfill(12)    # Padded with zeros for length 12
            synid = hline[12]
            objbase = synid + '_' + scanid
            objfile = objbase + '.obj'
            objlnk = objbase + '.lnk'
            objsrc = os.path.abspath( os.path.join( objdir, objfile)) # Location of source file
            objdst = os.path.abspath( os.path.join( csvhdir[1], objlnk))  # Location of destination
            createShortcut( objsrc, objdst)
    print 'Created', nvalid, 'object file links for', csvhdir[1]



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "Provide the obj directory (FACIAL FILES - OBJ), and the directory with the HPO directories (HPO-SCANS)."
        sys.exit(1)

    objdir = sys.argv[1] # C:\Users\Stefa\Dropbox\3D-FAST-HPO\FACIAL FILES - OBJ
    hpodir = sys.argv[2] # C:\Users\Stefa\Dropbox\3D-FAST-HPO\HPO-SCANS

    if not objdir.endswith("FACIAL FILES - OBJ"):
        print objdir, " does not end with 'FACIAL FILES - OBJ'!"
        sys.exit(2)

    if not hpodir.endswith("HPO-SCANS"):
        print objdir, " does not end with 'HPO-SCANS'!"
        sys.exit(3)

    if not os.path.isdir( objdir):
        print objdir, "is not a valid directory!"
        sys.exit(4)

    if not os.path.isdir( hpodir):
        print hpodir, "is not a valid directory!"
        sys.exit(5)

    csvhdirs = getCSVHPOs( hpodir)
    for csvhdir in csvhdirs:
        createOBJShortcuts( objdir, csvhdir)

    sys.exit(0)

#!/usr/bin/env python
import sys
import os
import re


def printNonConverted( lns):
    h = ""
    for i in range(0,len(lns)):
        m = re.search( ".*\.obj", lns[i])
        t = re.search( ".*\.tsb", lns[i])
        if m is not None:
            h = lns[i].split(".obj")[0] # Return the filename minus extension
        elif t is not None:
            if lns[i].split(".tsb")[0] != h:
                print lns[i].strip()


def walkFiles( dpath):
    lns = []
    for root, dirs, files in os.walk( dpath):
        for fn in files:
            mobj = re.search( ".*\.obj", fn)
            mtsb = re.search( ".*\.tsb", fn)
            if mtsb is not None or mobj is not None:
                lns.append( os.path.join(root, fn))
    return lns


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Please supply a directory to walk, or a file listing with sorted .obj and .tsb filenames."
        sys.exit(1)
    elif os.path.isdir( sys.argv[1]):
        lns = walkFiles( sys.argv[1])
    elif os.path.exists( sys.argv[1]):
        lns = open( sys.argv[1]).readlines()
    else:
        print "The supplied file or directory does not exist!"
        sys.exit(1)

    printNonConverted( lns)
    sys.exit(0)

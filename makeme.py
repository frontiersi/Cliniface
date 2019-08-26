#!/usr/bin/env python3

#************************************************************************
# * Copyright (C) 2019 Richard Palmer
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#************************************************************************

from CMakeBuild import CMakeBuilder
from CMakeBuild import EnvDirs
from CMakeBuild import hasFileOnPath
import zipfile
import shutil
import sys
import os


def requiredExes():
    cmakeexe = "cmake"
    ninjaexe = "ninja"
    if sys.platform == "win32":
        cmakeexe += ".exe"
        ninjaexe += ".exe"
    cmakepath = hasFileOnPath(cmakeexe)
    ninjapath = hasFileOnPath(ninjaexe)
    return cmakepath, ninjapath


def printUsage():
    #print( "Usage: {} (release | debug | package | archive)".format( sys.argv[0]))
    print( "Usage: {} (release | debug | package)".format( sys.argv[0]))
    print()
    print( " This script builds Cliniface within the directory from which it is run, potentially clobbering")
    print( " whatever else is present within the directory with the exception of a directory called 'plinks'")
    print( " which should contain symlinks to the plugin source directories that you wish to build.")
    print()
    print( " Pass 'release' to build a release version of Cliniface, 'debug' for a debug build, or 'package'")
    print( " to build an installation package from the release build (which will be build if it doesn't exist).")
    #print( " Pass 'archive' to build the release version, package it, and then create a zip from the package")
    #print( " that can be unpacked and copied to a USB drive for running directly.")
    print()
    print( " After building has finished, Cliniface can be run directly as './${CMAKE_BUILD_TYPE}/bin/cliniface'")
    print( " where ${CMAKE_BUILD_TYPE} is one of {Release,Debug}.")
    print()
    print( " CMake and Ninja must be on the path for this script to work.")
    print()

    cmakepath, ninjapath = requiredExes()
    if len(cmakepath) > 0:
        print( " {0} found at {1}".format( "cmake", cmakepath))
    else:
        print( " cmake was not found on PATH!")
    if len(ninjapath) > 0:
        print( " {0} found at {1}".format( "ninja", ninjapath))
    else:
        print( " ninja was not found on PATH!")


def printNoArgs( devDir, bldDir, pBldDir, pInsDir, packDir):
    printUsage()
    print()
    print( " Cliniface's source files are at '{}'".format(devDir))
    print()
    print( " Run '{} release' to build in the following directories:".format(sys.argv[0]))
    print( " build                '{}'".format(bldDir))
    print( " plugins build        '{}'".format(pBldDir))
    print( " plugins install      '{}'".format(pInsDir))
    print( " installation package '{}'".format(packDir))


def parseArg( args):
    cmakepath, ninjaexe = requiredExes()
    if len(cmakepath) == 0 or len(ninjaexe) == 0:
        printUsage()
        sys.exit(-1)

    doDebug   = False
    doRelease = False
    doPackage = False
    doArchive = False

    if len(args) > 0:
        arg = args[0].lower()
        if arg == "debug":
            doDebug = True
        elif arg == "release":
            doRelease = True
        elif arg == "package":
            doRelease = True
            doPackage = True
        #elif arg == "archive":
        #    doRelease = True
        #    doPackage = True
        #    doArchive = True
        else:
            print( "Exiting - unrecognised option '{}'".format(arg))
            printUsage()
            sys.exit(-1)

    return doRelease, doDebug, doPackage, doArchive


def buildPlugin( pname, pdev, doDebug, pBldDir, pInsDir):
    if not os.path.exists( os.path.join( pdev, "CMakeLists.txt")):
        print( "No CMakeLists.txt in '{}'".format(pdev))
        return False
    pBldDir = os.path.join( pBldDir, pname) # E.g. ./Release/plugins/Curvature
    mb = CMakeBuilder( pdev, doDebug)
    if mb.cmake( pBldDir, pInsDir):
        if mb.build():
            return mb.install()
    return False


if __name__ == "__main__":
    doRelease, doDebug, doPackage, doArchive = parseArg( sys.argv[1:])
    runDir = os.path.realpath( os.path.curdir)                  # Directory from where this script is run (.)
    devDir = os.path.dirname( os.path.realpath( sys.argv[0]))   # Dev directory is where this script is located

    mb = CMakeBuilder( devDir, doDebug)
    bldDir = os.path.join( runDir, mb.buildType())              # E.g. ./Release
    packDir = os.path.join( runDir, "Package")                  # E.g. ./Package
    archDir = os.path.join( runDir, "Archive")                  # E.g. ./Archive
    pBldDir = os.path.join( bldDir, "plugins")                  # E.g. ./Release/plugins
    pInsDir = os.path.join( runDir, "plugins", mb.buildType())  # E.g. ./plugins/Release

    if not doRelease and not doDebug:
        printNoArgs( devDir, bldDir, pBldDir, pInsDir, packDir)
        sys.exit(0)

    # Build and install the plugins in ./plinks
    plinks = os.path.join( runDir, "plinks")
    if not os.path.exists(plinks):
        print( "Error - plinks directory not present!\n")
        printUsage()
        sys.exit(-2)

    for pname in os.listdir( plinks):
        pdev = os.path.join( plinks, pname)
        if not buildPlugin( pname, pdev, doDebug, pBldDir, pInsDir):
            print( "Exiting - failed to build plugin '{}'".format(pname))
            sys.exit(-3)

    # Build the main Cliniface app
    if mb.cmake( bldDir, packDir):
        if mb.build():
            if doPackage:
                mb.install()

    sys.exit(0)

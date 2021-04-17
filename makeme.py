#!/usr/bin/env python3

#************************************************************************
# * Copyright (C) 2020 Richard Palmer
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
from pathlib import Path
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
    print( "Usage: {} (release | debug | package)".format( sys.argv[0]))
    print()
    print( " This script builds Cliniface within the directory from which it is run, potentially clobbering")
    print( " whatever else is present within the directory with the exception of a directory called 'other_plugins'")
    print( " which should contain symlinks to any extra plugin source directories to build.")
    print( " Note that the extra plugins are not copied into the the plugins directory (bin/plugins).")
    print()
    print( " Pass 'release' to build a release version of Cliniface, 'debug' for a debug build, or 'package'")
    print( " to build an installation package from the release build (which will be build if it doesn't exist).")
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


def printNoArgs( devDir, bpsDir, bldDir, pBldDir, pInsDir, packDir):
    printUsage()
    print()
    print( " Cliniface's source files are at '{}'".format(devDir))
    print( " Cliniface's core plugins source files are at '{}'".format(bpsDir))
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


def buildPluginsDir( pDir, doDebug, pBldDir, pInsDir):
    """Given each of the plugins given as separate source folders within directory pDir."""
    for pName in os.listdir( pDir):
        pDev = os.path.join( pDir, pName)
        if not buildPlugin( pName, pDev, doDebug, pBldDir, pInsDir):
            print( "Exiting - failed to build plugin '{}'".format(pName))
            sys.exit(-2)


if __name__ == "__main__":
    doRelease, doDebug, doPackage, doArchive = parseArg( sys.argv[1:])
    runDir = os.path.realpath( os.path.curdir)                  # Directory from where this script is run (.)
    devDir = os.path.dirname( os.path.realpath( sys.argv[0]))   # Dev directory is where this script is located
    bpsDir = os.path.join( devDir, "plugins")                   # Dev directory for base platform plugins

    mb = CMakeBuilder( devDir, doDebug)
    bldDir = os.path.join( runDir, mb.buildType())              # E.g. ./Release (build type directory)
    packDir = os.path.join( runDir, "Package")                  # E.g. ./Package (packaging directory)
    pBldDir = os.path.join( bldDir, "plugins")                  # E.g. ./Release/plugins (where plugins are built)
    pInsDir = os.path.join( bldDir, "bin", "plugins")           # E.g. ./Release/bin/plugins (where plugins are installed)

    if not doRelease and not doDebug:
        printNoArgs( devDir, bpsDir, bldDir, pBldDir, pInsDir, packDir)
        sys.exit(0)

    # There's a bug in CMake and Windows 10 which prevents proper resolution of symlinks.
    # As a workaround, we set a temporary environment variable here which is removed after.
    os.environ['CLINIFACE_DEPLOY_DIR'] = str(Path(__file__).resolve().parent.parent.joinpath('deployed'))

    rval = 1
    # Build the main Cliniface app. Do first since plugins depend upon the presence of Cliniface_Config.h.
    if mb.cmake( bldDir, packDir):
        if mb.build():
            # Build the core plugins and install them into Cliniface's core plugins directory
            buildPluginsDir( bpsDir, doDebug, pBldDir, pInsDir)

            # Build the plugins in ./other_plugins if given (these plugins are not installed).
            pextra = os.path.join( runDir, "other_plugins")
            if os.path.exists(pextra):
                print( "Building custom plugins - which will be installed here.")
                buildPluginsDir( pextra, doDebug, pBldDir, pInsDir)
            rval = 0
    else:
        print( "Exiting - failed to build Cliniface")

    if rval == 0 and doPackage:
        os.environ['ARCH'] = 'x86_64'
        mb.install()
        print( "Finished packaging")

    os.environ.pop('CLINIFACE_DEPLOY_DIR')
    sys.exit(rval)

#!/usr/bin/env python
import sys
import os
import shutil
import subprocess32

def printUsage( pname):
    print "Usage:"
    print pname + " installer_name [online]"
    print "By default, the full offline installer is created."
    print "Specify 'online' to create a repository and online installer."


if __name__ == "__main__":
    if len(sys.argv) == 1:
        printUsage( sys.argv[0])
        sys.exit(1)

    installerName = "ClinifaceInstaller"
    exitPostClean = sys.argv[1].lower() == "clean"
    if not exitPostClean:
        installerName = sys.argv[1]
    makeOnline = len(sys.argv) > 2 and sys.argv[2].lower() == "online"

    # Get the directory in which this script resides and the expected location of the release directory
    installerDir = os.path.dirname( os.path.realpath(__file__))
    releaseDir = os.path.dirname( installerDir)

    # Check that Cliniface.exe resides in the expected location
    clinifaceexe = os.path.join( os.path.join( releaseDir, "bin"), "Cliniface.exe")
    if not os.path.exists( clinifaceexe):
        print "Cliniface.exe does not exist in {0}".format( clinifaceexe)
        print "Only run this script if it resides within the release build directory!"
        sys.exit(2)

    # Set and check the location of the packaegs directory
    packagesDir = os.path.join( installerDir, "packages")
    if not os.path.isdir( packagesDir):
        print "packages directory could not be found!"
        sys.exit(3)

    # Set and check the location of config.xml
    configXML = os.path.join( os.path.join( installerDir, "config"), "config.xml")
    if not os.path.exists( configXML):
        print "config.xml could not be found!"
        sys.exit(4)

    # Get the location of the Qt Installer Framework tools
    if not os.environ.has_key('QT5_CMAKE_PATH'):
        print "Env. var. QT5_CMAKE_PATH must exist and specify the location of Qt."
        sys.exit(5)

    qtvar = os.environ.get('QT5_CMAKE_PATH')
    qtinf = os.path.sep.join( qtvar.split( os.path.sep)[:-5])
    qtinf = os.path.join( qtinf, os.path.sep.join( ['Tools', 'QtInstallerFramework', '3.0', 'bin']))
    if not os.path.isdir( qtinf):
        print "Invalid path to QtInstallerFramework tools!"
        print "Tried: {0}".format( qtinf)
        sys.exit(6)

    qtbincreator = os.path.join( qtinf, "binarycreator.exe")
    qtrepogen = os.path.join( qtinf, "repogen.exe")
    if not os.path.exists( qtbincreator) or not os.path.exists( qtrepogen):
        print "Cannot find Qt Installer Framework tools!"
        print "Check that environment variable QT5_CMAKE_PATH is set properly."
        sys.exit(7)

    repositoryDir = os.path.join( installerDir, "repository")

    print "INSTALLER_DIR:  {0}".format( installerDir)
    print "RELEASE_DIR:    {0}".format( releaseDir)
    print "PACKAGES_DIR:   {0}".format( packagesDir)
    print "REPOSITORY_DIR: {0}".format( repositoryDir)
    print "CONFIG_XML:     {0}".format( configXML)
    print "QTBINCREATOR:   {0}".format( qtbincreator)
    print "QTREPOGEN:      {0}".format( qtrepogen)
    print "INSTALLER_NAME: {0}".format( installerName)
    print "ONLINE:         {0}".format( makeOnline)

    exitVal = 0
    flags = "-v"
    if makeOnline:
        print "Creating repository content..."
        if os.path.isdir( repositoryDir):
            shutil.rmtree( repositoryDir)   # Remove any existing repository directory
        os.mkdir( repositoryDir)
        cmd = "{0} -v -p {1} {2}".format(qtrepogen, packagesDir, repositoryDir)
        print cmd
        if subprocess32.call( cmd) != 0:
            print " *** Repository generation failed! ***" 
            exitVal = -1
        else:
            print "Created repository inside {0}".format( repositoryDir)
            print "Creating online installer..."
            flags += " -n"
    else:
        flags += " --offline-only"
        print "Creating offline installer..."

    if exitVal == 0:
        cmd = "{0} -c {1} -p {2} {3} {4}".format(qtbincreator, configXML, packagesDir, flags, installerName)
        print cmd
        if subprocess32.call( cmd) != 0:
            print " *** Installer creation failed! ***"
            exitVal = -2

    sys.exit(exitVal)

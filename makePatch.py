#!/usr/bin/env python3

#************************************************************************
# * Copyright (C) 2022 Richard Palmer
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

from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED
import tempfile
import sys
import os
import hashlib
import xml.etree.ElementTree as ET


def fileAsBytes( file):
    with file:
        return file.read()

def hashByteStrIter( bytesiter, hasher, ashexstr=False):
    for block in bytesiter:
        hasher.update(block)
    return hasher.hexdigest() if ashexstr else hasher.digest()


def fileAsBlockIter( afile, blocksize=65536):
    with afile:
        block = afile.read(blocksize)
        while len(block) > 0:
            yield block
            block = afile.read(blocksize)


def parseFilesInBaseDir( baseDir) -> dict:
    baseDirPath = Path(baseDir)
    allf = []
    allf.extend( baseDirPath.joinpath('bin').rglob('*'))
    allf.extend( baseDirPath.joinpath('examples').rglob('*'))
    allf.extend( baseDirPath.joinpath('html').rglob('*'))
    if os.name == 'posix':  # Linux
        allf.extend( baseDirPath.joinpath('lib').rglob('*'))
        allf.extend( baseDirPath.joinpath('plugins').rglob('*'))
        allf.extend( baseDirPath.joinpath('share').rglob('*'))
    elif os.name == 'nt':   # Windows
        allf.extend( baseDirPath.joinpath('scripts').rglob('*'))
    fd = {}
    for f in allf:
        if f.is_file():
            nf = str(f).replace(str(baseDir) + os.sep,'')
            fd[nf] = hashByteStrIter( fileAsBlockIter( open(str(f), 'rb')), hashlib.md5(), False)
    return fd


def readLatestChanges() -> (str, list):
    """Read the latest changes from changes.md returning a tuple containing
    the version string e.g. '5.3.0' and the content for the Description XML tag."""
    lns = None
    changesFile = Path(__file__).resolve().parent.joinpath('changes.md')
    assert( changesFile.exists())
    with open( str(changesFile), 'r') as f:
        lns = f.readlines()
    vstr = None
    desc = []
    for ln in lns:
        ln = ln.strip()
        if len(ln) == 0:
            if not vstr:
                continue
            else:
                break
        elif not vstr:
            vstr = ln.split('#')[-1].strip()
        else:
            desc.append(ln)
    return vstr, desc


def appendFilesToXMLElement( elem, fls):
    elem.clear()  # Remove what was previously there
    if fls is not None:
        for f in fls:
            felem = elem.makeelement('File', {})
            felem.text = f
            elem.append(felem)


def updateManifest( umxml, vstr, desc, zipfile, mlist, rlist):
    assert( umxml.exists())
    tree = ET.parse(str(umxml))
    patchlist = tree.getroot()
    patches = patchlist.find('Patches')

    # Get the most recent patch version from the XML file
    # (which could have the same version number)
    patch = patches[0]
    attr = patch.attrib
    recentPatchVersion = '{}.{}.{}'.format( attr['major'], attr['minor'], attr['patch'])
    if recentPatchVersion != vstr:
        patch = ET.fromstring( ET.tostring(patch))  # Make a copy to change
        vnums = vstr.split('.')
        patch.attrib['major'] = vnums[0]
        patch.attrib['minor'] = vnums[1]
        patch.attrib['patch'] = vnums[2]
        patches.insert( 0, patch)
    baseurl = patch.find('BaseURL')
    baseurl.text = 'https://github.com/frontiersi/Cliniface/releases/download/v{}'.format(vstr)
    # Set the patch description
    desc.insert(0, '#### Version {}'.format(vstr))
    patch.find('Description').text = '\n'.join(desc)
    # Get the correct platform
    platforms = patch.find('Platforms')
    platform = platforms[0]
    if os.name == 'nt' and platform.attrib['name'] != 'Windows':
        platform = platforms[1]
    # Set the archive zipfile name
    platform.find('Archive').text = zipfile
    # Set the files
    appendFilesToXMLElement( platform.find('Modify'), mlist)
    appendFilesToXMLElement( platform.find('Remove'), rlist)
    tree.write( str(umxml))


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print( "Usage:")
        print( "{} old_base_dir new_base_dir new_version_number".format( sys.argv[0]))
        print( "Example usage:")
        params = "/tmp/.mount_ClinifHs9Soa/usr ~/lb/Cliniface/Package/AppDir/usr"
        if os.name == 'nt':
            params = "~\\AppData\\Local\\Programs\\Cliniface ~\\lb\\Cliniface\\Release"
        print( "{} {} 5.3.0".format( sys.argv[0], params))
        sys.exit(1)

    oldBaseDir = Path( sys.argv[1]).expanduser().resolve()
    newBaseDir = Path( sys.argv[2]).expanduser().resolve()
    vstr, desc = readLatestChanges()
    majorVer, minorVer, patchVer = [int(x) for x in vstr.split('.')]

    # Get the patch zip name and path, creating deployment directories as needed
    platform = 'win32-x64' if os.name == 'nt' else 'linux-x86_64'
    zipfname = 'patch-{}-{}.zip'.format(sys.argv[3], platform)
    deployedDir = Path(__file__).resolve().parent.parent.joinpath('deployed')
    if not deployedDir.exists():
        deployedDir.mkdir(parents=True)
    continuousDir = deployedDir.joinpath('continuous')
    if not continuousDir.exists():
        continuousDir.mkdir()
    versionDir = deployedDir.joinpath('v{}.{}'.format(majorVer, minorVer))
    if not versionDir.exists():
        versionDir.mkdir()
    zipfpath = str(versionDir.joinpath(zipfname))

    print( "Parsing old directory '{}'...".format(oldBaseDir))
    oldFiles = parseFilesInBaseDir( oldBaseDir)
    print( "Parsing new directory '{}'...".format(newBaseDir))
    newFiles = parseFilesInBaseDir( newBaseDir)

    newf = []   # Changed or added files
    for fn in newFiles:
        if fn not in oldFiles:
            newf.append(fn)
            print( "New file:     '{}'".format(fn))
        elif newFiles[fn] != oldFiles[fn]:
            oldFiles.pop( fn)
            newf.append(fn)
            print( "Changed file: '{}'".format(fn))
    newf.sort()
    print( "--- TOTAL of {} changed or added files ---".format(len(newf)))

    remf = []   # Removed files
    for fn in oldFiles:
        if fn not in newFiles:
            remf.append(fn)
            print( "Removed file: '{}'".format(fn))
    remf.sort()
    print( "--- TOTAL of {} files for removal ---".format(len(remf)))

    print( "Writing patch to '{}'...".format(zipfpath))
    z = ZipFile( zipfpath, mode='w', compression=ZIP_DEFLATED)
    for fn in newf:
        p = Path(newBaseDir).joinpath(fn)
        z.write( str(p), arcname=fn)
    z.close()

    umname = 'update-manifest.xml'
    umzip = continuousDir.joinpath(umname).with_suffix('.zip')
    tmpdir = tempfile.gettempdir()
    umxml = Path(tmpdir).joinpath(umname)
    z = ZipFile( str(umzip), mode='r')
    z.extract(umname, path=tmpdir)

    print( "Updating '{}'...".format(str(umzip)))
    updateManifest( umxml, vstr, desc, zipfname, newf, remf)
    #updateManifest( umxml, vstr, desc, zipfname, newf, None)
    z = ZipFile( str(umzip), mode='w', compression=ZIP_DEFLATED)
    z.write( str(umxml), arcname=umname)
    z.close()

    print()
    print( "  Now finish uploading all files to GitHub:")
    print( "  1) Create a new release with version tag 'v{}'".format(vstr))
    print( "  2) Copy over the version description from 'changes.md'")
    print( "  3) Upload the files in the respective 'app/deployed/v{}.{}' directory and publish".format(majorVer, minorVer))
    print( "  4) Edit the 'Continuous' release to have the latest files in 'app/deployed/v{}.{}'".format(majorVer, minorVer))
    print( "  NB Always ensure step 4 is the last performed!")
    #print( "  REMOVE FILES CURRENTLY DISABLED IN PATCH!")

    sys.exit(0)

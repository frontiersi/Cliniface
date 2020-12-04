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

from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED
import sys
import os
import hashlib


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
    allf = []
    allf.extend( Path(baseDir).joinpath('bin').rglob('*'))
    allf.extend( Path(baseDir).joinpath('examples').rglob('*'))
    allf.extend( Path(baseDir).joinpath('html').rglob('*'))
    if os.name == 'posix':
        allf.extend( Path(baseDir).joinpath('lib').rglob('*'))
    fd = {}
    for f in allf:
        if f.is_file():
            nf = str(f).replace(str(baseDir) + '/','')
            fd[nf] = hashByteStrIter( fileAsBlockIter( open(str(f), 'rb')), hashlib.md5(), False)
    return fd


def printUpdateManifestPlatformXML( mlist, rlist, zipfile):
    platform = 'Windows' if os.name == 'nt' else 'Linux'
    print('<Platform name="{}">'.format(platform))
    print('    <Archive>{}</Archive'.format(zipfile))
    print('    <Modify>')
    for f in mlist:
        print('        <File>{}</File>'.format(f))
    print('    </Modify>')
    print('    <Remove>')
    for f in rlist:
        print('        <File>{}</File>'.format(f))
    print('    </Remove>')
    print('</Platform>')


if __name__ == "__main__":
    if len(sys.argv) != 4:
        aname = sys.argv[0]
        print( "Usage:")
        print( "{} old_base_dir new_base_dir new_version_number".format( aname))
        print( "For example:")
        if os.name == 'nt':
            print( "{} ~\\AppData\\Local\\Programs\\Cliniface ~\lb\Cliniface\Release 5.3.0".format( aname))
        else:
            print( "{} /tmp/.mount_ClinifHs9Soa/usr ~/lb/Cliniface/Package/AppDir/usr 5.3.0".format( aname))
        sys.exit(1)

    oldBaseDir = Path( sys.argv[1]).expanduser()
    newBaseDir = Path( sys.argv[2]).expanduser()
    platform = 'win32-x64' if os.name == 'nt' else 'linux-x86_64'
    zipFileName = 'patch-{}-{}.zip'.format(sys.argv[3], platform)

    oldFiles = parseFilesInBaseDir( oldBaseDir)
    newFiles = parseFilesInBaseDir( newBaseDir)

    newf = []   # Changed or added files
    for fn in newFiles:
        if fn not in oldFiles:
            newf.append(fn)
        elif newFiles[fn] != oldFiles[fn]:
            oldFiles.pop( fn)
            newf.append(fn)

    remf = []   # Removed files
    for fn in oldFiles:
        if fn not in newFiles:
            remf.append(fn)

    newf.sort()
    remf.sort()

    z = ZipFile( zipFileName, mode='w', compression=ZIP_DEFLATED)
    for fn in newf:
        p = Path(newBaseDir).joinpath(fn)
        z.write( str(p), arcname=fn)
    z.close()

    printUpdateManifestPlatformXML( newf, remf, zipFileName)

    sys.exit(0)

/************************************************************************
 * Copyright (C) 2022 SIS Research Ltd & Richard Palmer
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFileInfo>
#include <QDir>
#include <Cliniface_Config.h>
#include <ClinifaceMain.h>
#include <FaceTools.h>

namespace Cliniface {

class ClinifaceApp
{
public:
    ClinifaceApp();
    ~ClinifaceApp();

    int start( int argc, char **argv);

    inline bool doCopyMeasures() const { return _parser.isSet( _copyUserMeasures);}
    inline bool doCropFace() const { return _parser.isSet( _cutopt);}
    inline bool doMap() const { return _parser.isSet( _mapopt);}
    inline bool doExportMask() const { return _parser.isSet( _mskopt);}
    inline bool forceWrite() const { return _parser.isSet( _forceopt);}

private:
    QCoreApplication *_app;
    FaceTools::FM *_fm;
    ClinifaceMain *_mainWin;
    QCommandLineOption _copyUserMeasures;
    QCommandLineOption _cutopt;
    QCommandLineOption _mapopt;
    QCommandLineOption _mskopt;
    QCommandLineOption _repopt;
    QCommandLineOption _forceopt;
    QCommandLineParser _parser;
    QFileInfo _inpath, _outpath;
    QDir _tgtDir;
    bool _saveMeta;
    int _reportId;

    bool _hasInputFile() const;
    bool _isCommandLineOnly() const;
    int _openGUI();
    int _runCommandLine();
    bool _setOutPath();
    int _setReport();
    QFileInfo _getFileInfo( int) const;

    ClinifaceApp( const ClinifaceApp&) = delete;
    void operator=( const ClinifaceApp&) = delete;
};  // end class

}   // end namespace

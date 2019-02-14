/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
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

#ifndef Cliniface_Preferences_h
#define Cliniface_Preferences_h

#include <QString>
#include <memory>
#include <sol.hpp>

namespace Cliniface {

class Preferences
{
public:
    using Ptr = std::shared_ptr<Preferences>;

    // Load preferences returning true on success.
    static bool load( const QString& luascript);

    // Returns the previously loaded preferences or null if none loaded.
    static Ptr get() { return _singleton;}

    // Return the absolute paths to pdflatex and idtfconverter.
    const QString pdflatex() const { return _pdflatex;}
    const QString idtfconv() const { return _idtfconv;}

    // Return the absolute paths to the haar cascades models and the face shape landmarks file for detection.
    const QString haarModels() const { return _haarmodl;}
    const QString faceShape() const { return _faceshap;}

    const QString pdfReader() const { return _pdfreader;}
    bool openPDFOnSave() const { return _openPDFOnSave;}

    // Return the maximum number of models to be loaded.
    int maxload() const { return _maxload;}

private:
    static Ptr _singleton;
    int _maxload;
    QString _pdflatex, _idtfconv;
    QString _haarmodl, _faceshap;
    QString _pdfreader, _inkscape;
    bool _openPDFOnSave;
    sol::state _lua;

    bool read();
    Preferences();
    Preferences( const Preferences&) = delete;
    void operator=( const Preferences&) = delete;
};  // end class

}   // end namespace

#endif

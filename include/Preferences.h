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

#ifndef Cliniface_Preferences_h
#define Cliniface_Preferences_h

#include "Options.h"
#include <memory>
#include <sol.hpp>

namespace Cliniface {

class Preferences
{
public:
    static bool init(); // Initialise preferences

    // Refresh(update) the application with currently set options
    // and make the currently set options the applied options.
    static void apply();

    // Set/get the currently set options (may not be applied yet!)
    static void setOptions( const Options&);
    static const Options& options();

    // Return the applied options which will be equal to the set options
    // iff setOptions has not been called after the last call to apply.
    static const Options& appliedOptions();

    // Reset the currently set options to their defaults.
    static void reset();

    // Returns true iff the currently set options are the same as the applied ones.
    static bool isApplied();

    // Returns full path to the persistent configuration file being used.
    static const QString& configPath();

    // Write the given options to file or the APPLIED options if null.
    static bool writeConfig( const Options *opts=nullptr);

private:
    using Ptr = std::shared_ptr<Preferences>;
    static Ptr _singleton;
    bool _allspec;
    Options _opts;
    Options _aopts;
    sol::state _lua;
    QString _configfile;

    static Ptr _get();

    // Returns true iff the last read in of parameters specified all of
    // them without any default being set. If this function returns false,
    // it is necessary to update the configuration file with new parameters.
    static bool _allSpecified();

    // Load preferences returning true on success.
    static bool _read( const QString& luascript);

    bool _read();
    QString _readFilePath( const char*);
    QString _readDirPath( const char*);
    QString _readString( const char*, const QString&);
    QSize _readSize( const char*, const QSize&);
    bool _readBool( const char*, bool);
    int _readInt( const char*, int);
    double _readDouble( const char*, double);

    Preferences();
    Preferences( const Preferences&) = delete;
    void operator=( const Preferences&) = delete;
};  // end class

}   // end namespace

#endif

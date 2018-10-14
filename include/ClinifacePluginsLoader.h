/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef Cliniface_CLINIFACE_PLUGINS_LOADER_H
#define Cliniface_CLINIFACE_PLUGINS_LOADER_H

#include <PluginsDialog.h>      // QTools
#include <FaceModelManager.h>   // FaceTools::Action
using FAM = FaceTools::Action::FaceActionManager;

namespace Cliniface {

class ClinifacePluginsLoader : public QObject
{ Q_OBJECT
public:
    ClinifacePluginsLoader( FAM*, QWidget* parent=nullptr);
    ~ClinifacePluginsLoader() override;

    void loadPlugins( const QString& dllsDir);
    QDialog* dialog() { return _pdialog;}   // Dialog showing loaded plugins.

private slots:
    void addPlugin( QTools::PluginInterface*);

private:
    QTools::PluginsDialog *_pdialog;
    FAM *_fam;

    ClinifacePluginsLoader( const ClinifacePluginsLoader&) = delete;
    void operator=( const ClinifacePluginsLoader&) = delete;
};  // end class

}   // end namespace

#endif

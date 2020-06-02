/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#include <ClinifacePluginsLoader.h>
#include <FaceTools/Action/FaceActionManager.h>
#include <Cliniface_Config.h>
using Cliniface::ClinifacePluginsLoader;
using FaceTools::Action::FaceAction;
using FaceTools::Action::FAM;


ClinifacePluginsLoader::ClinifacePluginsLoader( QWidget* parent)
    : QObject(), _pdialog( new QTools::PluginsDialog( parent)) {}


ClinifacePluginsLoader::~ClinifacePluginsLoader() { delete _pdialog;}


void ClinifacePluginsLoader::loadPlugins( const QString& dllsDir)
{
    QTools::PluginsLoader ploader( dllsDir.toStdString());
    connect( &ploader, &QTools::PluginsLoader::loadedPlugin, this, &ClinifacePluginsLoader::_addPlugin);
    std::cerr << "Loading plugins from " << ploader.pluginsDir().absolutePath().toStdString() << std::endl;
    ploader.loadPlugins( APP_PLUGIN_TOKEN);  // Actually load with _addPlugin called for each
    _pdialog->addPlugins( ploader); // Add plugins to the dialog
}   // end loadPlugins


void ClinifacePluginsLoader::_addPlugin( QTools::PluginInterface* plugin, const QString& pluginpath)
{
    if ( !plugin)
    {
#ifndef NDEBUG
        std::cerr << "Failed to load plugin from: " << pluginpath.toStdString() << std::endl;
        std::cerr << " >>> Might there be undefined functions on the class?" << std::endl;
#endif
        return;
    }   // end if

#ifndef NDEBUG
    std::cerr << "Loaded plugin: " << plugin->displayName().toStdString() << std::endl;
#endif

    for ( const QString& iid : plugin->interfaceIds())
    {
        FaceAction *act = qobject_cast<FaceAction*>( plugin->iface(iid));
        if ( act)
        {
            FAM::registerAction( act);
            if ( !act->attachToMenu().isEmpty())
                emit onAttachToMenu( act);
        }   // end if
    }   // end for
}   // end _addPlugin


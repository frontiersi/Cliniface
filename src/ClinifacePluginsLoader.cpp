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

#include <ClinifacePluginsLoader.h>
#include <MetricCalculatorTypeRegistry.h>
#include <FaceActionManager.h>
#include <FaceActionGroup.h>
using Cliniface::ClinifacePluginsLoader;



ClinifacePluginsLoader::ClinifacePluginsLoader( FAM* fam, QWidget* parent)
    : QObject(), _pdialog( new QTools::PluginsDialog( parent)), _fam(fam)
{
}   // end ctor


// public
ClinifacePluginsLoader::~ClinifacePluginsLoader()
{
    delete _pdialog;
}   // end dtor


// public
void ClinifacePluginsLoader::loadPlugins( const QString& dllsDir)
{
    QTools::PluginsLoader ploader( dllsDir.toStdString());
    std::cerr << "Plugins directory: " << ploader.getPluginsDir().absolutePath().toStdString() << std::endl;
    connect( &ploader, &QTools::PluginsLoader::loadedPlugin, this, &ClinifacePluginsLoader::addPlugin);
    ploader.loadPlugins();
    _pdialog->addPlugins( ploader);
}   // end loadPlugins


// private slot
void ClinifacePluginsLoader::addPlugin( QTools::PluginInterface* plugin)
{
    using namespace FaceTools::Action;
    FaceAction* faction = qobject_cast<FaceAction*>(plugin);
    FaceActionGroup* grp = qobject_cast<FaceActionGroup*>(plugin);

    using MCTR = FaceTools::Metric::MetricCalculatorTypeRegistry;
    using MCT = FaceTools::Metric::MetricCalculatorTypeInterface;
    MCT* mct = qobject_cast<MCT*>(plugin);

    if ( faction)
        _fam->addAction( faction);
    else if ( mct)
        MCTR::addMCT( mct);
    else if ( grp)
    {
        for ( const QString& iid : grp->getInterfaceIds())
            _fam->addAction( qobject_cast<FaceAction*>( grp->getInterface(iid)));
    }   // end if
}   // end addPlugin


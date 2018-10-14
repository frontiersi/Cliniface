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

#include <VisualisationsOrganiser.h>
#include <SurfaceVisualisation.h>
using Cliniface::VisualisationsOrganiser;
using FaceTools::Action::FaceActionManager;
using FaceTools::Action::ActionVisualise;
using FaceTools::Action::FaceAction;
using FaceTools::Vis::SurfaceVisualisation;


namespace {

QList<QAction*> concat( const QList<QAction*>& lstA, const QList<QAction*>& lstB)
{
    QList<QAction*> lst;
    lst.append(lstA);
    if ( !lstB.isEmpty())
    {
        QAction* separator = new QAction;
        separator->setSeparator(true);
        lst.append(separator);
        lst.append(lstB);
    }   // end if
    return lst;
}   // end concat


QList<QAction*> menuActions( const std::vector<ActionVisualise*>& vs)
{
    //float lst9;   // WEIRD! Declaration of lst[0-9] breaks MSVC version 19.00.24225.1 for x64
    QList<QAction*> lstA;
    QList<QAction*> lstB;
    for ( ActionVisualise* a : vs)
    {
        if ( a->isExclusive())
            lstA << a->qaction();
        else
            lstB << a->qaction();
    }   // end for

    return concat(lstA,lstB);
}   // end menuActions


QList<QAction*> toolbarActions( const std::vector<ActionVisualise*>& vs)
{
    QList<QAction*> lstA;
    QList<QAction*> lstB;
    for ( ActionVisualise* a : vs)
    {
        if ( !a->allowOnToolbar())
            continue;

        if ( a->isExclusive())
            lstA << a->qaction();
        else
            lstB << a->qaction();
    }   // end for

    return concat(lstA,lstB);
}   // end toolbarActions

}   // end namespace


VisualisationsOrganiser::VisualisationsOrganiser( FaceActionManager* fam)
{
    connect( fam, &FaceActionManager::addedAction, this, &VisualisationsOrganiser::doOnAddedAction);
}   // end ctor


// private slot
void VisualisationsOrganiser::doOnAddedAction( FaceAction* a)
{
    ActionVisualise* av = qobject_cast<ActionVisualise*>(a);
    if ( av && av->isVisible())
    {
        if ( qobject_cast<SurfaceVisualisation*>(av->visualisation()))
            _smappings.push_back(av);
        else
            _ordered.push_back(av);
    }   // end if
}   // end add


QList<QAction*> VisualisationsOrganiser::surfaceMappingMenu() const { return menuActions(_smappings);}
QList<QAction*> VisualisationsOrganiser::surfaceMappingToolbar() const { return toolbarActions(_smappings);}
QList<QAction*> VisualisationsOrganiser::otherMenu() const { return menuActions(_ordered);}
QList<QAction*> VisualisationsOrganiser::otherToolbar() const { return toolbarActions(_ordered);}

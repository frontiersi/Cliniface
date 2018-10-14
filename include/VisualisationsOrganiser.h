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

#ifndef Cliniface_VISUALISATIONS_ORGANISER_H
#define Cliniface_VISUALISATIONS_ORGANISER_H

#include <FaceActionManager.h>
#include <ActionVisualise.h>
#include <vector>
#include <QList>
#include <QAction>

namespace Cliniface {

class VisualisationsOrganiser : public QObject
{ Q_OBJECT
public:
    explicit VisualisationsOrganiser( FaceTools::Action::FaceActionManager*);

    QList<QAction*> surfaceMappingMenu() const;
    QList<QAction*> surfaceMappingToolbar() const;

    QList<QAction*> otherMenu() const;
    QList<QAction*> otherToolbar() const;

private slots:
    void doOnAddedAction( FaceTools::Action::FaceAction*);

private:
    std::vector<FaceTools::Action::ActionVisualise*> _smappings;    // Surface mapping visualisations
    std::vector<FaceTools::Action::ActionVisualise*> _ordered;      // All other visualisations

    VisualisationsOrganiser( const VisualisationsOrganiser&) = delete;
    void operator=( const VisualisationsOrganiser&) = delete;
};  // end class

}   // end namespace

#endif

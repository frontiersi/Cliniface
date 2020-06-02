/************************************************************************
 * Copyright (C) 2019 SIS Research Ltd & Richard Palmer
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

#include <QTools/PluginsDialog.h>
#include <FaceTools/FileIO/FaceModelManager.h>

namespace Cliniface {

class ClinifacePluginsLoader : public QObject
{ Q_OBJECT
public:
    explicit ClinifacePluginsLoader( QWidget* parent=nullptr);
    ~ClinifacePluginsLoader() override;

    void loadPlugins( const QString& pluginsDir);   // Directory with shared objects (Windows DLLs)
    QDialog* dialog() { return _pdialog;}   // Dialog showing loaded plugins.

signals:
    void onAttachToMenu( FaceTools::Action::FaceAction*);

private slots:
    void _addPlugin( QTools::PluginInterface*, const QString&);

private:
    QTools::PluginsDialog *_pdialog;

    ClinifacePluginsLoader( const ClinifacePluginsLoader&) = delete;
    void operator=( const ClinifacePluginsLoader&) = delete;
};  // end class

}   // end namespace

#endif

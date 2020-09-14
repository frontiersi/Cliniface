/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#ifndef Cliniface_UpdatesDialog_h
#define Cliniface_UpdatesDialog_h

#include <FaceTools/Action/ActionNewerVersionFinder.h>
#include <QDialog>

using namespace FaceTools::Action;

namespace Ui { class UpdatesDialog;}
namespace Cliniface {

class UpdatesDialog : public QDialog
{ Q_OBJECT
public:
    explicit UpdatesDialog(QWidget *parent = nullptr);
    ~UpdatesDialog();

    FaceAction *action() { return _finder;}

private slots:
    void _doOnFoundNewerVersion( bool);
    void _doOnUpdateButtonPushed();
    void _doOnDonateButtonPushed();

private:
    Ui::UpdatesDialog *_ui;
    ActionNewerVersionFinder *_finder;
};  // end class

}   // end namespace

#endif

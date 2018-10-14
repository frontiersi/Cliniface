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

#ifndef Cliniface_HelpDialog_h
#define Cliniface_HelpDialog_h

#include <QDialog>

namespace Ui { class HelpDialog;}
namespace Cliniface {

class HelpDialog : public QDialog
{ Q_OBJECT
public:
    explicit HelpDialog( QWidget *parent = nullptr);
    ~HelpDialog() override;

private:
    Ui::HelpDialog *ui;
    HelpDialog( const HelpDialog&) = delete;
    void operator=( const HelpDialog&) = delete;
};  // end class

}   // end namespace

#endif

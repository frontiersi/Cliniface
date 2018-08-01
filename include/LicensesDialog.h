/************************************************************************
 * Copyright (C) 2017 Richard Palmer
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

#ifndef Cliniface_LicensesDialog_h
#define Cliniface_LicensesDialog_h

#include <QDialog>

namespace Ui { class LicensesDialog;}
namespace Cliniface {

class LicensesDialog : public QDialog
{ Q_OBJECT
public:
    explicit LicensesDialog( QWidget *parent = nullptr);
    ~LicensesDialog() override;

private:
    Ui::LicensesDialog *ui;
    LicensesDialog( const LicensesDialog&) = delete;
    void operator=( const LicensesDialog&) = delete;
};  // end class

}   // end namespace

#endif

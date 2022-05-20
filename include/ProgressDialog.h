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

#ifndef Cliniface_PROGRESSDIALOG_H
#define Cliniface_PROGRESSDIALOG_H

#include <QDialog>

namespace Ui { class ProgressDialog;}
namespace Cliniface {

class ProgressDialog : public QDialog
{ Q_OBJECT
public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

    void reset( size_t n);

signals:
    void onCancelled();

public slots:
    void doOnPercentProgress( float);

private:
    Ui::ProgressDialog *_ui;
    size_t _nfiles;
};  // end class

}   // end namespace

#endif

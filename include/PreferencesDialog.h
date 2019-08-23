/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
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

#ifndef Cliniface_PreferencesDialog_h
#define Cliniface_PreferencesDialog_h

#include <QDialog>
#include <QLineEdit>
#include <QTimer>
#include "Preferences.h"

namespace Ui { class PreferencesDialog;}
namespace Cliniface {

class PreferencesDialog : public QDialog
{ Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    PreferencesDialog( const PreferencesDialog&) = delete;
    void operator=( const PreferencesDialog&) = delete;
    ~PreferencesDialog() override;

signals:
    void onUpdated();
    void showHelp();

public slots:
    void accept() override;
    void reject() override;

private slots:
    void updateMemoryBar();

private:
    Ui::PreferencesDialog *_ui;
    QTimer _timer;

    void chooseExe( QLineEdit*);
    void chooseModel( QLineEdit*);
};  // end class

}   // end namespace

#endif

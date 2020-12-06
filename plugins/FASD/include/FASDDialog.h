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

#ifndef Cliniface_Plugin_FASDDialog_H
#define Cliniface_Plugin_FASDDialog_H

#include "Cliniface_Plugin_FASD_Export.h"
#include <unordered_map>
#include <vector>
#include <QDialog>
#include <QFileDialog>

namespace Ui { class FASDDialog;}

namespace Cliniface {

class Cliniface_Plugin_FASD_EXPORT FASDDialog : public QDialog
{ Q_OBJECT
public:
    explicit FASDDialog( QWidget *parent=nullptr);
    ~FASDDialog() override;

    static QString REPORT_NAME;

public slots:
    void refresh();

signals:
    void onShowHelp();

private slots:
    void _doOnGuideChanged();
    void _doOnUpdateOverallRank();
    void _doOnGenerateReport();

private:
    Ui::FASDDialog *_ui;
    QFileDialog *_sdialog;

    // Ethnicity code keyed lip curvature guides
    std::unordered_map<int, std::vector<double> > _lcguides;

    void _addReportTable();
    void _addReportFootnotes();
    void _createGuides();
    void _updateMeanPFL( double, double);
};  // end class

}   // end namespace

#endif


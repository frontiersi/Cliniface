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

#include <PreferencesDialog.h>
#include <ui_PreferencesDialog.h>
using Cliniface::PreferencesDialog;
#include <FaceModelManager.h>
#include <QFileDialog>
#include <QToolButton>
#include <QFile>
#include <Cliniface_Config.h>
#include <iostream>
#include <Memory.h> // rlib
using FaceTools::FileIO::FMM;

namespace {

bool isValidPath( const QString& fpath) { return QFileInfo(fpath).exists();}

bool testValidExe( QLineEdit* ledit)
{
    bool valid = false;
    ledit->setStyleSheet("color: red;");
    const QFileInfo finfo( ledit->text());
    const QString pname = ledit->property("program_name").toString();
    if ( finfo.exists() && (pname.isEmpty() || finfo.fileName() == pname))
    {
        ledit->setText(finfo.filePath());   // Sets slashes correctly
        ledit->setStyleSheet("color: black;");
        valid = true;
    }   // end if
    return valid;
}   // end testValidExe


bool testValidModelPath( QLineEdit *ledit)
{
    bool valid = false;
    ledit->setStyleSheet("color: red;");
    const QFileInfo finfo( ledit->text());
    if ( FMM::canRead( finfo.filePath().toStdString()))
    {
        ledit->setText(finfo.filePath());   // Sets slashes correctly
        ledit->setStyleSheet("color: black;");
        valid = true;
    }   // end if
    return valid;
}   // end testValidModelPath

}   // end namespace



PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::PreferencesDialog), _timer(this)
{
    _ui->setupUi(this);

    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape");
#ifdef _WIN32
    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex.exe");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape.exe");
#endif

    connect( _ui->latexExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->latexExeLineEdit);});
    connect( _ui->inkscapeExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->inkscapeExeLineEdit);});
    connect( _ui->maskPathLineEdit, &QLineEdit::textChanged, [this](){ testValidModelPath( _ui->maskPathLineEdit);});

    connect( _ui->latexExeButton, &QToolButton::clicked, [this](){ chooseExe( _ui->latexExeLineEdit);});
    connect( _ui->inkscapeExeButton, &QToolButton::clicked, [this](){ chooseExe( _ui->inkscapeExeLineEdit);});
    connect( _ui->maskPathButton, &QToolButton::clicked, [this](){ chooseModel( _ui->maskPathLineEdit);});

    connect( _ui->helpButton, &QToolButton::clicked, this, &PreferencesDialog::showHelp);

    reject();   // Initialise values

    const size_t memBytes = rlib::getMemorySize();
    const int kbs = int(double(memBytes) / 1024);
    _ui->memoryBar->setRange( 0, kbs);
    connect( &_timer, &QTimer::timeout, this, &PreferencesDialog::updateMemoryBar);
    _timer.start(2000); // Update once every two seconds
}   // end ctor


PreferencesDialog::~PreferencesDialog()
{
    delete _ui;
}   // end dtor


void PreferencesDialog::chooseExe( QLineEdit* ledit)
{
    QString* sfilter = nullptr;
    const QString ptype = ledit->property("program_type").toString();
    QString pname = ledit->property("program_name").toString();
    QString caption = tr(QString("Locate the %1 program to use.").arg(ptype).toStdString().c_str());
    if ( !pname.isEmpty() && pname != ptype)
    {
        caption = tr(QString("Locate executable '%1'.").arg(pname).toStdString().c_str());
        sfilter = &pname;
    }   // end if

    QString fileTypes = "Executable files (*)";
#ifdef _WIN32
    fileTypes = "Exe files (*.exe)";
#endif

    QFileDialog dlg( this, caption);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setOption( QFileDialog::DontUseNativeDialog);
    dlg.setNameFilter( sfilter ? *sfilter : fileTypes);

    if ( dlg.exec())
    {
        if ( !dlg.selectedFiles().empty())
        {
            const QString fname = dlg.selectedFiles().first();
            ledit->setText(fname);
            testValidExe(ledit);
        }   // end if
    }   // end if
}   // end chooseExe


void PreferencesDialog::chooseModel( QLineEdit* ledit)
{
    QString caption = tr( "Specify the model to use.");
    QFileDialog dlg( this, caption);

    QStringList filters = FMM::fileFormats().createImportFilters().split(";;");
    filters.prepend( "Any file (*)");
    dlg.setNameFilters(filters);
    dlg.setViewMode(QFileDialog::Detail);
    dlg.setFileMode(QFileDialog::ExistingFiles);
    dlg.setOption(QFileDialog::DontUseNativeDialog);
    dlg.setOption(QFileDialog::DontUseCustomDirectoryIcons);

    if ( dlg.exec())
    {
        if ( !dlg.selectedFiles().empty())
        {
            QString fname = dlg.selectedFiles().first();
            ledit->setText(fname);
            testValidModelPath(ledit);
        }   // end if
    }   // end if
}   // end chooseModel


void PreferencesDialog::accept()
{
    Preferences::Ptr prefs = Preferences::get();

    if ( isValidPath( _ui->latexExeLineEdit->text()))
        prefs->setPdfLatex( _ui->latexExeLineEdit->text());

    if ( isValidPath( _ui->inkscapeExeLineEdit->text()))
        prefs->setInkscape( _ui->inkscapeExeLineEdit->text());

    prefs->setUnits( _ui->unitsLineEdit->text());
    prefs->setOverlapOpacity( _ui->overlapOpacitySpinBox->value());
    prefs->setMetricOpacity( _ui->metricOpacitySpinBox->value());
    prefs->setMaxLoad( _ui->maxModelsSpinBox->value());
    prefs->setMaxMan( _ui->maxManifoldsSpinBox->value());
    prefs->setOpenPDFOnSave( _ui->viewReportsCheckBox->isChecked());
    prefs->setAutoFocus( _ui->autoFocusCheckBox->isChecked());
    prefs->setWhiteBG( _ui->whiteBGCheckBox->isChecked());
    prefs->setAntiAlias( _ui->antiAliasCheckBox->isChecked());

    if ( isValidPath( _ui->maskPathLineEdit->text()))
        prefs->setMaskPath( _ui->maskPathLineEdit->text());

    prefs->setKnnReg( _ui->knnSpinBox->value());

    prefs->write(); // Also updates application wide settings
    QDialog::accept();
    emit onUpdated();
}   // end accept


void PreferencesDialog::reject()
{
    Preferences::Ptr prefs = Preferences::get();
    _ui->latexExeLineEdit->setText( prefs->pdflatex());
    _ui->inkscapeExeLineEdit->setText( prefs->inkScape());
    _ui->unitsLineEdit->setText( prefs->units());
    _ui->overlapOpacitySpinBox->setValue( prefs->overlapOpacity());
    _ui->metricOpacitySpinBox->setValue( prefs->metricOpacity());
    _ui->maxModelsSpinBox->setValue( prefs->maxLoad());
    _ui->maxManifoldsSpinBox->setValue( prefs->maxMan());
    _ui->viewReportsCheckBox->setChecked( prefs->openPDFOnSave());
    _ui->autoFocusCheckBox->setChecked( prefs->autoFocus());
    _ui->whiteBGCheckBox->setChecked( prefs->whiteBG());
    _ui->antiAliasCheckBox->setChecked( prefs->antiAlias());
    _ui->maskPathLineEdit->setText( prefs->maskPath());
    _ui->knnSpinBox->setValue( prefs->knnReg());
    QDialog::reject();
}   // end reject


void PreferencesDialog::updateMemoryBar()
{
    const int nb = int(rlib::getCurrentRSS());
    const int kbs = int(double(nb) / 1024);
    _ui->memoryBar->setValue(kbs);
}   // end updateMemoryBar

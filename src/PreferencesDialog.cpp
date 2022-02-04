/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#include <FaceTools/FileIO/FaceModelManager.h>

#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <Cliniface_Config.h>
#include <iostream>
using FMM = FaceTools::FileIO::FaceModelManager;
using QMB = QMessageBox;

namespace {

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

/*
bool testValidModelPath( QLineEdit *ledit)
{
    bool valid = false;
    ledit->setStyleSheet("color: red;");
    const QFileInfo finfo( ledit->text());
    if ( FMM::canRead( finfo.filePath()))
    {
        ledit->setText(finfo.filePath());   // Sets slashes correctly
        ledit->setStyleSheet("color: black;");
        valid = true;
    }   // end if
    return valid;
}   // end testValidModelPath
*/


bool testValidDir( QLineEdit *ledit)
{
    bool valid = false;
    ledit->setStyleSheet("color: red;");
    const QFileInfo finfo( ledit->text());
    if ( finfo.exists() && finfo.isDir() && finfo.isReadable())
    {
        ledit->setText(finfo.filePath());   // Sets slashes correctly
        ledit->setStyleSheet("color: black;");
        valid = true;
    }   // end if
    return valid;
}   // end testValidDir

}   // end namespace


PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::PreferencesDialog), _exeDialog(nullptr), _imgDialog(nullptr)
{
    _ui->setupUi(this);

    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape");
#ifdef _WIN32
    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex.exe");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape.exe");
#endif

    connect( _ui->latexExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->latexExeLineEdit); _testAndSetButtons();});
    connect( _ui->inkscapeExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->inkscapeExeLineEdit); _testAndSetButtons();});
    connect( _ui->imagesPathLineEdit, &QLineEdit::textChanged, [this](){ testValidDir( _ui->imagesPathLineEdit); _testAndSetButtons();});

    connect( _ui->latexExeButton, &QToolButton::clicked, [this](){ _chooseExe( _ui->latexExeLineEdit); _testAndSetButtons();});
    connect( _ui->inkscapeExeButton, &QToolButton::clicked, [this](){ _chooseExe( _ui->inkscapeExeLineEdit); _testAndSetButtons();});
    connect( _ui->imagesPathButton, &QToolButton::clicked, [this](){ _chooseDir( _ui->imagesPathLineEdit); _testAndSetButtons();});

    connect( _ui->viewReportsCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->maxManifoldsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->showBoxesCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->whiteBGCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->antiAliasCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->checkUpdateCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->smoothLightingCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->interpolatedShadingCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->parallelProjectionMetricsCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->viewAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->cropRadiusSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &PreferencesDialog::_reset);
    connect( _ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PreferencesDialog::_apply);

    _refresh(); // Init values

    _exeDialog = new QFileDialog( this, tr( "Specify the program to use."));
    _exeDialog->setFileMode( QFileDialog::ExistingFile);

    _imgDialog = new QFileDialog( this, tr( "Specify the directory where images are stored."));
    _imgDialog->setFileMode( QFileDialog::Directory);
    _imgDialog->setOption( QFileDialog::ShowDirsOnly, true);
    /*
    QStringSet exts;
    exts.insert( FMM::fileFormats().preferredExt());
    QStringList filters = FMM::fileFormats().createImportFilters( exts);
    _modDialog->setNameFilters(filters);
    */
    _imgDialog->setViewMode(QFileDialog::Detail);

    this->adjustSize();
    setFixedSize( geometry().width(), geometry().height());
}   // end ctor


PreferencesDialog::~PreferencesDialog()
{
    delete _ui;
}   // end dtor


void PreferencesDialog::_chooseExe( QLineEdit* ledit)
{
    QString* sfilter = nullptr;
    const QString ptype = ledit->property("program_type").toString();
    QString pname = ledit->property("program_name").toString();
    //QString caption = tr(QString("Locate the %1 program to use.").arg(ptype).toStdString().c_str());
    if ( !pname.isEmpty() && pname != ptype)
    {
        //caption = tr(QString("Locate executable '%1'.").arg(pname).toStdString().c_str());
        sfilter = &pname;
    }   // end if

    QString fileTypes = "Executable files (*)";
#ifdef _WIN32
    fileTypes = "Exe files (*.exe)";
#endif

    _exeDialog->setNameFilter( sfilter ? *sfilter : fileTypes);
    if ( _exeDialog->exec())
    {
        if ( !_exeDialog->selectedFiles().empty())
        {
            const QString fname = _exeDialog->selectedFiles().first();
            ledit->setText(fname);
            testValidExe(ledit);
        }   // end if
    }   // end if
}   // end _chooseExe


void PreferencesDialog::_chooseDir( QLineEdit* ledit)
{
    if ( _imgDialog->exec())
    {
        if ( !_imgDialog->selectedFiles().empty())
        {
            QString fname = _imgDialog->selectedFiles().first();
            ledit->setText(fname);
            testValidDir(ledit);
        }   // end if
    }   // end if
}   // end _chooseDir


bool PreferencesDialog::_hasValidEntries() const
{
    if ( !_ui->latexExeLineEdit->text().isEmpty() && !testValidExe( _ui->latexExeLineEdit))
        return false;
    if ( !_ui->inkscapeExeLineEdit->text().isEmpty() && !testValidExe( _ui->inkscapeExeLineEdit))
        return false;
    if ( !_ui->imagesPathLineEdit->text().isEmpty() && !testValidDir( _ui->imagesPathLineEdit))
        return false;
    return true;
}   // end _hasValidEntries


void PreferencesDialog::_testAndSetButtons()
{
    static const Options defaultOptions;
    Options guiOptions = _toOptions();
    const bool isCurrent = Preferences::appliedOptions() == guiOptions;
    const bool isValid = _hasValidEntries();
    _ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled( guiOptions != defaultOptions);
    _ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled( !isCurrent && isValid);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( isValid);
}   // end _testAndSetButtons


void PreferencesDialog::_apply()
{
    const Options guiOptions = _toOptions();
    Preferences::setOptions( guiOptions);
    Preferences::apply();
    emit onUpdated();
    if ( !Preferences::writeConfig())
    {
        static const QString msg = tr("Unable to update preferences configuration file!");
        QMB::critical( this, tr("Preferences Write Error!"), QString("<p align='center'>%1</p>").arg(msg));
    }   // end if
    _testAndSetButtons();
}   // end _apply


void PreferencesDialog::accept()
{
    const bool isCurrent = Preferences::appliedOptions() == _toOptions();
    if ( !isCurrent && _hasValidEntries())
        _apply();
    QDialog::accept();
}   // end accept


void PreferencesDialog::_reset()
{
    Options opts;   // Defaults
    Preferences::setOptions( opts);
    _fromOptions( opts);
    _testAndSetButtons();
}   // end _reset


void PreferencesDialog::_refresh()
{
    const Options &opts = Preferences::appliedOptions();
    Preferences::setOptions( opts);
    _fromOptions( opts);
    _testAndSetButtons();
}   // end _refresh


Cliniface::Options PreferencesDialog::_toOptions() const
{
    Options opts = Preferences::options();
    //opts.setNrMaskPath( _ui->maskPathLineEdit->text());
    opts.setUserImagesPath( _ui->imagesPathLineEdit->text());
    opts.setPdfLatex( _ui->latexExeLineEdit->text());
    opts.setInkscape( _ui->inkscapeExeLineEdit->text());
    opts.setOpenPDFOnSave( _ui->viewReportsCheckBox->isChecked());
    opts.setMaxMan( _ui->maxManifoldsSpinBox->value());
    opts.setShowBoxes( _ui->showBoxesCheckBox->isChecked());
    opts.setWhiteBG( _ui->whiteBGCheckBox->isChecked());
    opts.setAntiAlias( _ui->antiAliasCheckBox->isChecked());
    opts.setCheckUpdate( _ui->checkUpdateCheckBox->isChecked());
    opts.setSmoothLighting( _ui->smoothLightingCheckBox->isChecked());
    opts.setInterpolatedShading( _ui->interpolatedShadingCheckBox->isChecked());
    opts.setParallelProjectionMetrics( _ui->parallelProjectionMetricsCheckBox->isChecked());
    opts.setViewAngle( _ui->viewAngleSpinBox->value());
    opts.setCropRadius( _ui->cropRadiusSpinBox->value());

    return opts;
}   // end _toOptions


void PreferencesDialog::_fromOptions( const Cliniface::Options &opts)
{
    //_ui->maskPathLineEdit->setText( opts.nrMaskPath());
    _ui->imagesPathLineEdit->setText( opts.userImagesPath());
    _ui->latexExeLineEdit->setText( opts.pdflatex());
    _ui->inkscapeExeLineEdit->setText( opts.inkscape());
    _ui->viewReportsCheckBox->setChecked( opts.openPDFOnSave());
    _ui->maxManifoldsSpinBox->setValue( opts.maxMan());
    _ui->showBoxesCheckBox->setChecked( opts.showBoxes());
    _ui->whiteBGCheckBox->setChecked( opts.whiteBG());
    _ui->antiAliasCheckBox->setChecked( opts.antiAlias());
    _ui->checkUpdateCheckBox->setChecked( opts.checkUpdate());
    _ui->smoothLightingCheckBox->setChecked( opts.smoothLighting());
    _ui->interpolatedShadingCheckBox->setChecked( opts.interpolatedShading());
    _ui->parallelProjectionMetricsCheckBox->setChecked( opts.parallelProjectionMetrics());
    _ui->viewAngleSpinBox->setValue( opts.viewAngle());
    _ui->cropRadiusSpinBox->setValue( opts.cropRadius());
}   // end _fromOptions


void PreferencesDialog::reject()
{
    _refresh();
    QDialog::reject();
}   // end reject


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
#include <rlib/Memory.h>
using FMM = FaceTools::FileIO::FaceModelManager;

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

}   // end namespace


PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::PreferencesDialog), _exeDialog(nullptr), _modDialog(nullptr), _timer(this)
{
    _ui->setupUi(this);

    setWindowModality( Qt::NonModal);

    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape");
#ifdef _WIN32
    _ui->latexExeLineEdit->setProperty("program_name", "pdflatex.exe");
    _ui->inkscapeExeLineEdit->setProperty("program_name", "inkscape.exe");
#endif

    connect( _ui->latexExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->latexExeLineEdit); _testAndSetButtons();});
    connect( _ui->inkscapeExeLineEdit, &QLineEdit::textChanged, [this](){ testValidExe( _ui->inkscapeExeLineEdit); _testAndSetButtons();});
    connect( _ui->maskPathLineEdit, &QLineEdit::textChanged, [this](){ testValidModelPath( _ui->maskPathLineEdit); _testAndSetButtons();});

    connect( _ui->latexExeButton, &QToolButton::clicked, [this](){ _chooseExe( _ui->latexExeLineEdit); _testAndSetButtons();});
    connect( _ui->inkscapeExeButton, &QToolButton::clicked, [this](){ _chooseExe( _ui->inkscapeExeLineEdit); _testAndSetButtons();});
    connect( _ui->maskPathButton, &QToolButton::clicked, [this](){ _chooseModel( _ui->maskPathLineEdit); _testAndSetButtons();});

    connect( _ui->viewReportsCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->maxModelsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->maxManifoldsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->autoFocusCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->showBoxesCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->whiteBGCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->antiAliasCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->smoothLightingCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->interpolatedShadingCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->parallelProjectionMetricsCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});
    connect( _ui->viewAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->overlapOpacitySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->maxCurvatureSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->cropRadiusSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    _ui->useCurveDistCheckBox->setEnabled(false);
    connect( _ui->useCurveDistCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->nrTotalItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->nrKnnSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrFlagThreshSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrEqualiseCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->nrSmoothItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrKappaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrOrientationCheckBox, &QCheckBox::clicked, [this](){ _testAndSetButtons();});

    connect( _ui->nrSigmaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrRegNbsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->nrInitViscoseItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrLastViscoseItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrInitElasticItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});
    connect( _ui->nrLastElasticItsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](){ _testAndSetButtons();});

    connect( _ui->buttonBox, &QDialogButtonBox::helpRequested, this, &PreferencesDialog::showHelp);
    connect( _ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &PreferencesDialog::_reset);
    connect( _ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PreferencesDialog::_apply);

    _refresh(); // Init values

    // Initialise the memory bar
    const uint64_t memBytes = rlib::getMemorySize();
    const uint64_t kbs = uint64_t(double(memBytes) / 1024L);
    _ui->memoryBar->setRange( 0, kbs);
    connect( &_timer, &QTimer::timeout, this, &PreferencesDialog::_updateMemoryBar);
    _timer.start(2000); // Update once every two seconds

    _exeDialog = new QFileDialog( this, tr( "Specify the program to use."));
    _exeDialog->setFileMode(QFileDialog::ExistingFile);
    _exeDialog->setOption( QFileDialog::DontUseNativeDialog);

    _modDialog = new QFileDialog( this, tr( "Specify the model to use."));
    QStringSet exts;
    exts.insert( FMM::fileFormats().preferredExt());
    QStringList filters = FMM::fileFormats().createImportFilters( exts);
    _modDialog->setNameFilters(filters);
    _modDialog->setViewMode(QFileDialog::Detail);
    _modDialog->setFileMode(QFileDialog::ExistingFiles);
    _modDialog->setOption(QFileDialog::DontUseNativeDialog);
    _modDialog->setOption(QFileDialog::DontUseCustomDirectoryIcons);
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


void PreferencesDialog::_chooseModel( QLineEdit* ledit)
{
    if ( _modDialog->exec())
    {
        if ( !_modDialog->selectedFiles().empty())
        {
            QString fname = _modDialog->selectedFiles().first();
            ledit->setText(fname);
            testValidModelPath(ledit);
        }   // end if
    }   // end if
}   // end _chooseModel


bool PreferencesDialog::_hasValidEntries() const
{
    if ( !_ui->latexExeLineEdit->text().isEmpty() && !testValidExe( _ui->latexExeLineEdit))
        return false;
    if ( !_ui->inkscapeExeLineEdit->text().isEmpty() && !testValidExe( _ui->inkscapeExeLineEdit))
        return false;
    if ( !_ui->maskPathLineEdit->text().isEmpty() && !testValidModelPath( _ui->maskPathLineEdit))
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
    using QMB = QMessageBox;
    const Options guiOptions = _toOptions();
    Preferences::setOptions( guiOptions);
    Preferences::apply();
    emit onUpdated();
    if ( !Preferences::writeConfig())
        QMB::warning( this, tr("Preferences write error!"), tr("Unable to update preferences configuration file!"), QMB::Close);
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

    opts.setNrMaskPath( _ui->maskPathLineEdit->text());
    opts.setPdfLatex( _ui->latexExeLineEdit->text());
    opts.setInkscape( _ui->inkscapeExeLineEdit->text());

    opts.setOpenPDFOnSave( _ui->viewReportsCheckBox->isChecked());

    opts.setMaxLoad( _ui->maxModelsSpinBox->value());
    opts.setMaxMan( _ui->maxManifoldsSpinBox->value());

    opts.setAutoFocus( _ui->autoFocusCheckBox->isChecked());
    opts.setShowBoxes( _ui->showBoxesCheckBox->isChecked());
    opts.setWhiteBG( _ui->whiteBGCheckBox->isChecked());
    opts.setAntiAlias( _ui->antiAliasCheckBox->isChecked());
    opts.setSmoothLighting( _ui->smoothLightingCheckBox->isChecked());
    opts.setInterpolatedShading( _ui->interpolatedShadingCheckBox->isChecked());
    opts.setParallelProjectionMetrics( _ui->parallelProjectionMetricsCheckBox->isChecked());
    opts.setViewAngle( _ui->viewAngleSpinBox->value());

    opts.setOverlapOpacityReduction( _ui->overlapOpacitySpinBox->value());

    opts.setMaxSmoothCurv( _ui->maxCurvatureSpinBox->value());
    opts.setCropRadius( _ui->cropRadiusSpinBox->value());
    opts.setCurvDistTool( _ui->useCurveDistCheckBox->isChecked());

    opts.setNrTotalIts( _ui->nrTotalItsSpinBox->value());

    // Correspondence finding
    opts.setNrKnnReg( _ui->nrKnnSpinBox->value());
    opts.setNrFlagThresh( _ui->nrFlagThreshSpinBox->value());
    opts.setNrEqPushPull( _ui->nrEqualiseCheckBox->isChecked());

    // Inlier finding
    opts.setNrSmoothIts( _ui->nrSmoothItsSpinBox->value());
    opts.setNrKappa( _ui->nrKappaSpinBox->value());
    opts.setNrOrient( _ui->nrOrientationCheckBox->isChecked());

    // Regularisation
    opts.setNrSigma( _ui->nrSigmaSpinBox->value());
    opts.setNrRegNbs( _ui->nrRegNbsSpinBox->value());

    // Transformation
    opts.setNrInitVisIts( _ui->nrInitViscoseItsSpinBox->value());
    opts.setNrLastVisIts( _ui->nrLastViscoseItsSpinBox->value());
    opts.setNrInitElsIts( _ui->nrInitElasticItsSpinBox->value());
    opts.setNrLastElsIts( _ui->nrLastElasticItsSpinBox->value());

    return opts;
}   // end _toOptions


void PreferencesDialog::_fromOptions( const Cliniface::Options &opts)
{
    _ui->latexExeLineEdit->setText( opts.pdflatex());
    _ui->inkscapeExeLineEdit->setText( opts.inkscape());
    _ui->viewReportsCheckBox->setChecked( opts.openPDFOnSave());

    _ui->maxModelsSpinBox->setValue( opts.maxLoad());
    _ui->maxManifoldsSpinBox->setValue( opts.maxMan());

    _ui->autoFocusCheckBox->setChecked( opts.autoFocus());
    _ui->showBoxesCheckBox->setChecked( opts.showBoxes());
    _ui->whiteBGCheckBox->setChecked( opts.whiteBG());
    _ui->antiAliasCheckBox->setChecked( opts.antiAlias());
    _ui->smoothLightingCheckBox->setChecked( opts.smoothLighting());
    _ui->interpolatedShadingCheckBox->setChecked( opts.interpolatedShading());
    _ui->parallelProjectionMetricsCheckBox->setChecked( opts.parallelProjectionMetrics());
    _ui->viewAngleSpinBox->setValue( opts.viewAngle());

    _ui->overlapOpacitySpinBox->setValue( opts.overlapOpacityReduction());

    _ui->maxCurvatureSpinBox->setValue( opts.maxSmoothCurv());
    _ui->cropRadiusSpinBox->setValue( opts.cropRadius());
    _ui->useCurveDistCheckBox->setChecked( opts.curvDistTool());

    // Non-rigid general
    _ui->maskPathLineEdit->setText( opts.nrMaskPath());
    _ui->nrTotalItsSpinBox->setValue( opts.nrTotalIts());

    // Correspondence finding
    _ui->nrKnnSpinBox->setValue( opts.nrKnnReg());
    _ui->nrFlagThreshSpinBox->setValue( opts.nrFlagThresh());
    _ui->nrEqualiseCheckBox->setChecked( opts.nrEqPushPull());

    // Inlier finding
    _ui->nrSmoothItsSpinBox->setValue( opts.nrSmoothIts());
    _ui->nrKappaSpinBox->setValue( opts.nrKappa());
    _ui->nrOrientationCheckBox->setChecked( opts.nrOrient());

    // Regularisation
    _ui->nrSigmaSpinBox->setValue( opts.nrSigma());
    _ui->nrRegNbsSpinBox->setValue( opts.nrRegNbs());

    // Transformation
    _ui->nrInitViscoseItsSpinBox->setValue( opts.nrInitVisIts());
    _ui->nrLastViscoseItsSpinBox->setValue( opts.nrLastVisIts());
    _ui->nrInitElasticItsSpinBox->setValue( opts.nrInitElsIts());
    _ui->nrLastElasticItsSpinBox->setValue( opts.nrLastElsIts());
}   // end _fromOptions


void PreferencesDialog::reject()
{
    _refresh();
    QDialog::reject();
}   // end reject


void PreferencesDialog::_updateMemoryBar()
{
    const uint64_t nb = rlib::getCurrentRSS();
    const uint64_t kbs = uint64_t(double(nb) / 1024L);
    _ui->memoryBar->setValue(kbs);
}   // end _updateMemoryBar


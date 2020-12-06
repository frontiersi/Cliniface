/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#include <FASDDialog.h>
#include <ui_FASDDialog.h>

#include <FaceTools/Ethnicities.h>
#include <FaceTools/FaceModel.h>
#include <FaceTools/U3DCache.h>
#include <FaceTools/Action/ModelSelector.h>
#include <FaceTools/Action/ActionExportPDF.h>
#include <FaceTools/Metric/MetricManager.h>
#include <FaceTools/Report/ReportManager.h>

#include <rlib/MathUtil.h>

#include <QMessageBox>
#include <QToolButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <cassert>
using Cliniface::FASDDialog;
using MS = FaceTools::Action::ModelSelector;
using MM = FaceTools::Metric::MetricManager;
using MC = FaceTools::Metric::Metric;
using FaceTools::Report::Report;
using FaceTools::Report::ReportManager;
using FaceTools::FaceSide;
using FaceTools::FM;

QString FASDDialog::REPORT_NAME;


namespace {
static const int PFL_ID = 10;       // Palpebral Fissure Length
static const int ULIPC_ID = 1000;   // Upper Lip Circularity
static const int PHLD_ID = 2001;    // Philtral Depth
static bool REPORT_GENERATION_FAILED(false);


QString toString( double v, int ndp=2) { return QString("%1").arg( v, 0, 'f', ndp);}

QString toString( int v) { return QString("%1").arg(v);}


int getLipRank( const std::vector<double>& bnds, double val)
{
    if ( val <= bnds[0])
        return 1;
    if ( val <= bnds[1])
        return 2;
    if ( val <= bnds[2])
        return 3;
    if ( val <= bnds[3])
        return 4;
    return 5;
}   // end getLipRank


int getRankFromZScore( double z)
{
    if ( z <= -3.0)
        return 5;
    if ( z <= -2.0)
        return 4;
    if ( z <= -1.0)
        return 3;
    if ( z < 0.0)
        return 2;
    return 1;
}   // end getRankFromZScore


QString getLabelFromRank( int idx)
{
    QString label = "None (1)";
    switch ( idx)
    {
        case 2:
            label = "Mild (2)";
            break;
        case 3:
            label = "Moderate (3)";
            break;
        case 4:
            label = "Severe (4)";
            break;
    }   // end swtich
    return label;
}   // end getLabelFromRank


int getABCIndexFromRank( int rank)
{
    if ( rank <= 2)
        return 0;
    if ( rank == 3)
        return 1;
    return 2;
}   // end getABCIndexFromRank


// Returns value of {0 = A, 1 = B, 2 = C}
QString getLabelFromIndex( int score)
{
    if ( score == 2)
        return "C";
    if ( score == 1)
        return "B";
    return "A";
}   // end getLabelFromIndex


bool isReportGenerationPossible()
{
    if ( REPORT_GENERATION_FAILED)
        return false;
    const FM* fm = MS::selectedModel();
    if ( !FaceTools::Action::ActionExportPDF::isAvailable( fm))
        return false;
    Report::CPtr report = ReportManager::report(FASDDialog::REPORT_NAME);
    assert(report);
    if ( !report)
        return false;
    return report->isAvailable(fm);
}   // end isReportGenerationPossible


QToolButton* setToolButtonBorder( QToolButton* b)
{
    b->setStyleSheet( "\
            QToolButton{ border: none;} \
            QToolButton:hover{ \
                border: 1px solid #8f8f91; \
                border-radius: 4px; \
            }");
    return b;
}   // end setToolButtonBorder

}   // end namespace


void FASDDialog::_createGuides()
{
    _ui->guideComboBox->setEditable(true);
    _ui->guideComboBox->lineEdit()->setAlignment(Qt::AlignmentFlag::AlignCenter);

    // Create the Caucasian guide
    _ui->guideComboBox->addItem( FaceTools::Ethnicities::name(905), 905);
    std::vector<double>& vals1 = _lcguides[905];
    vals1.push_back(42.4f);
    vals1.push_back(57.45f);
    vals1.push_back(75.45f);
    vals1.push_back(131.45f);

    // Create the African American guide
    _ui->guideComboBox->addItem( FaceTools::Ethnicities::name(8101), 8101);
    std::vector<double>& vals2 = _lcguides[8101];
    vals2.push_back(27.4f);
    vals2.push_back(30.05f);
    vals2.push_back(52.05f);
    vals2.push_back(62.05f);

    _ui->guideComboBox->setEditable(false);

    connect( _ui->guideComboBox, QOverload<int>::of(&QComboBox::activated), this, &FASDDialog::_doOnGuideChanged);
}   // end createGuides


FASDDialog::FASDDialog( QWidget *parent)
    : QDialog(parent), _ui(new Ui::FASDDialog), _sdialog(nullptr)
{
    _ui->setupUi(this);
    _createGuides();
    _ui->pflDistanceLabel->setText( QString("Distance (%1)").arg(FM::LENGTH_UNITS));

    connect( _ui->philtrumRankSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FASDDialog::_doOnUpdateOverallRank);

    connect( _ui->reportButton, &QToolButton::clicked, this, &FASDDialog::_doOnGenerateReport);
    connect( _ui->helpButton, &QToolButton::clicked, this, &FASDDialog::onShowHelp);

    setToolButtonBorder( _ui->reportButton);
    setToolButtonBorder( _ui->helpButton);

    // Add the required functionality to the report via a delegate
    Report::Ptr report = ReportManager::report( REPORT_NAME);
    report->addCustomLuaFn( "addFASDTable", [this](){ return this->_addReportTable();});
    report->addCustomLuaFn( "addFASDFootnotes", [this](){ return this->_addReportFootnotes();});

    _sdialog = FaceTools::Action::ActionExportPDF::createSaveDialog( this);
}   // end ctor


FASDDialog::~FASDDialog() { delete _ui;}


void FASDDialog::_doOnGenerateReport()
{
    Report::Ptr report = ReportManager::report(REPORT_NAME);

    // Generate the rpeort
    const FM* fm = MS::selectedModel();
    const FaceTools::U3DCache::Filepath u3dfile = FaceTools::U3DCache::u3dfilepath(fm);
    QTemporaryDir tmpdir;
    QString tmpfile = tmpdir.filePath( "report.pdf");
    report->setModelFile( *u3dfile);
    const bool genok = report->generate( fm, tmpfile);
    if ( genok)
        FaceTools::Action::ActionExportPDF::saveGeneratedReport( tmpfile, _sdialog);
    else
    {
        REPORT_GENERATION_FAILED = true;    // Will prevent further report generation
        QMessageBox::warning( this, tr("Report Creation Error!"), QString("Unable to generate %1").arg(REPORT_NAME));
    }   // end if
}   // end _doOnGenerateReport


void FASDDialog::_addReportTable()
{
    const QString lpflv = _ui->leftPFLValueLabel->text();
    const QString lpflz = _ui->leftPFLZScoreLabel->text();
    const QString rpflv = _ui->rightPFLValueLabel->text();
    const QString rpflz = _ui->rightPFLZScoreLabel->text();
    const QString mpflv = _ui->meanPFLValueLabel->text();
    const QString mpflz = _ui->meanPFLZScoreLabel->text();
    const QString pflRank = _ui->pflRankLabel->text();
    const QString pflABC = _ui->pflABCLabel->text();

    QString ostr;
    QTextStream os(&ostr);

    os << "\\begin{center}" << Qt::endl
       << "\\textbf{University of Washington FASD Diagnostics} \\\\" << Qt::endl
       << "This report uses the 2004 version of the FASD 4-Digit Diagnostic Guide." << Qt::endl
       << "For details, visit \\href{http://www.fasdpn.org}{www.fasdpn.org}" << Qt::endl
       << "\\end{center}" << Qt::endl;

    // Add the PFL table
    os << R"~(\begin{table}[H]
            \centering
            \caption*{\textbf{Palpebral Fissure Lengths} \footnotemark[1]}
            \begin{tabular}{r||c|c|c|c|}
                & Length ()~" << FM::LENGTH_UNITS << R"~() & Z-score & Rank & ABC Score \\ \hline)~";
    os << QString("\\ Right & %1 & %2 & \\multicolumn{2}{|c|}{n/a} \\\\ \\hline").arg(rpflv, rpflz) << Qt::endl;
    os << QString("\\ Left  & %1 & %2 & \\multicolumn{2}{|c|}{n/a} \\\\ \\hline").arg(lpflv, lpflz) << Qt::endl;
    os << QString("\\ Mean  & %1 & %2 & %3 & %4 \\\\ \\hline").arg(mpflv, mpflz, pflRank, pflABC) << Qt::endl;
    os << R"~(\end{tabular}
            \end{table})~" << Qt::endl;

    // Add the Philtrum depth measurements
    const FM* fm = MS::selectedModel();
    const FaceTools::Metric::MetricSet &ms = fm->currentAssessment()->cmetrics(FaceSide::MID);
    const FaceTools::Metric::MetricValue &mval = ms.metric( PHLD_ID);
    const double p1 = rlib::round( mval.value(0), 2);
    const double p2 = rlib::round( mval.value(1), 2);
    const double p3 = rlib::round( mval.value(2), 2);

    const int pcvRank = _ui->philtrumRankSpinBox->value();
    const QString pcvABC = _ui->philtrumABCLabel->text();
    os << R"~(\begin{table}[H]
            \centering
            \caption*{\textbf{Philtral Depth}}
            \begin{tabular}{|c|c|c|c|c|}
                P1 & P2 & P3 & Rank & ABC Score\\ \hline)~";
    os << QString("\\ %1 & %2 & %3 & %4 & %5 \\\\ \\hline").arg(p1).arg(p2).arg(p3).arg(pcvRank).arg(pcvABC) << Qt::endl
       << R"~(\end{tabular}
            \end{table})~" << Qt::endl;

    // Add the Upper Lip Circularity
    const double ulc = _ui->upperLipValueLabel->text().toDouble();
    const QString ethg = _ui->guideComboBox->currentText();
    const QString ulRank = _ui->upperLipRankLabel->text();
    const QString ulABC = _ui->upperLipABCLabel->text();

    os << R"~(\begin{table}[H]
            \centering
            \caption*{\textbf{Upper Lip Circularity ()~" << ethg << R"~()}}
            \begin{tabular}{|c|c|c|}
               $Perimeter^2 / Area$ & Rank & ABC Score \\ \hline)~";
    os << QString("%1 & %2 & %3 \\\\ \\hline").arg(ulc).arg(ulRank, ulABC) << Qt::endl
       << R"~(\end{tabular}
            \end{table})~" << Qt::endl;

    const QString exlevel = _ui->rankLabel->text();
    os << R"~(\begin{center}
          \textbf{Diagnostic Expression Level:} )~" << exlevel;
/*
    if ( _ui->use2DCheckBox->isChecked())
        os << "\\\\ \\color{red} \\textbf{ATTN: 2D measurements may be less accurate}" << Qt::endl;
*/
    os << "\\end{center}" << Qt::endl;

    Report::Ptr report = ReportManager::report(REPORT_NAME);
    report->addCustomLatex( ostr);
}   // end _addReportTable


void FASDDialog::_addReportFootnotes()
{
    // PFL source for footnote
    Report::Ptr report = ReportManager::report(REPORT_NAME);
    const FaceTools::Metric::GrowthData *gd = MM::metric(PFL_ID)->growthData().current();
    if ( gd)
        report->addCustomLatex( QString("\\footnotetext[1]{%1}").arg( gd->source()));
}   // end _addReportFootnotes


void FASDDialog::_doOnGuideChanged()
{
    const double v = _ui->upperLipValueLabel->text().toDouble();
    const int ethn = _ui->guideComboBox->currentData().toInt();
    const std::vector<double>& lcguide = _lcguides.at(ethn);
    const int rank = getLipRank( lcguide, v);
    _ui->upperLipRankLabel->setText( toString(rank));
    _doOnUpdateOverallRank();
}   // end _doOnGuideChanged


void FASDDialog::_updateMeanPFL( double mv, double mz)
{
    _ui->meanPFLValueLabel->setText( toString( mv));
    _ui->meanPFLZScoreLabel->setText( toString( mz));
    const int rank = getRankFromZScore(mz);
    _ui->pflRankLabel->setText( toString(rank));
    _doOnUpdateOverallRank();
}   // end _updateMeanPFL


void FASDDialog::_doOnUpdateOverallRank()
{
    // A == 0, B == 1, C == 2
    const int r0 = getABCIndexFromRank( _ui->pflRankLabel->text().toInt());
    const int r1 = getABCIndexFromRank( _ui->philtrumRankSpinBox->value());
    const int r2 = getABCIndexFromRank( _ui->upperLipRankLabel->text().toInt());

    _ui->pflABCLabel->setText( getLabelFromIndex( r0));
    _ui->philtrumABCLabel->setText( getLabelFromIndex( r1));
    _ui->upperLipABCLabel->setText( getLabelFromIndex( r2));

    const int tv = r0 + r1 + r2;
    assert( tv >= 0 && tv <= 6);

    int rank = 1;   // None
    if ( tv == 6)
        rank = 4;   // Severe
    else if ( tv == 5)
        rank = 3;   // Moderate
    else if ( r0 == 2 || r1 == 2 || r2 == 2)
        rank = 2;   // Mild

    _ui->rankLabel->setText( getLabelFromRank(rank));
}   // end _doOnUpdateOverallRank


void FASDDialog::refresh()
{
    using namespace FaceTools;
    using namespace FaceTools::Metric;

    // Set the current PFL source
    const GrowthData *gd = MM::metric(PFL_ID)->growthData().current();
    QString src;
    if ( gd)
    {
        //_ui->statisticsSourceLabel->setText( gd->source());
        src = "Reference growth curves: " + gd->source();
    }   // end if
    _ui->zscoreLabel->setToolTip( src);
    _ui->leftPFLZScoreLabel->setToolTip( src);
    _ui->rightPFLZScoreLabel->setToolTip( src);
    _ui->meanPFLZScoreLabel->setToolTip( src);

    double lpflv = 0.0;
    double lz = 0.0;
    double rpflv = 0.0;
    double rz = 0.0;
    double ulv = 0.0;

    const FM* fm = MS::selectedModel();
    if ( fm)
    {
        FaceAssessment::CPtr fass = fm->currentAssessment();
        if ( fass->cmetrics(FaceSide::LEFT).has(PFL_ID))
        {
            const MetricValue& lmv = fass->cmetrics(FaceSide::LEFT).metric(PFL_ID);
            lpflv = rlib::round( lmv.value(), 2);
            lz = rlib::round( lmv.zscore(fm->age()), 2);
        }   // end if

        if ( fass->cmetrics(FaceSide::RIGHT).has(PFL_ID))
        {
            const MetricValue& rmv = fass->cmetrics(FaceSide::RIGHT).metric(PFL_ID);
            rpflv = rlib::round( rmv.value(), 2);
            rz = rlib::round( rmv.zscore(fm->age()), 2);
        }   // end if

        if ( fass->cmetrics(FaceSide::MID).has(ULIPC_ID))
            ulv = rlib::round( fass->cmetrics(FaceSide::MID).metric(ULIPC_ID).value(), 2);  // Upper Lip Circularity
    }   // end if

    // Left PFL
    _ui->leftPFLValueLabel->setText( toString( lpflv));
    _ui->leftPFLZScoreLabel->setText( toString( lz));

    // Right PFL
    _ui->rightPFLValueLabel->setText( toString( rpflv));
    _ui->rightPFLZScoreLabel->setText( toString( rz));

    // Upper Lip
    _ui->upperLipValueLabel->setText( toString( ulv));

    _updateMeanPFL( 0.5*(rpflv + lpflv), 0.5*(rz + lz));

    _doOnGuideChanged();

    _ui->reportButton->setEnabled( isReportGenerationPossible());
}   // end refresh

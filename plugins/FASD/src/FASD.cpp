/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#include <FASD.h>
#include <FASDDialog.h>
#include <FaceTools/FaceModel.h>
#include <FaceTools/Action/FaceAction.h>
#include <FaceTools/Report/ReportManager.h>
using Cliniface::FASD;
using FaceTools::Action::FaceAction;
using FaceTools::Action::Event;
using Cliniface::FASDDialog;
using FaceTools::FM;
using MS = FaceTools::ModelSelect;


class Cliniface_Plugin_FASD_EXPORT ActionShowDialog : public FaceAction
{
public:
    ActionShowDialog( const QString& dname, const QIcon &ic=QIcon(), const QKeySequence& ks=QKeySequence())
        : FaceAction( dname, ic, ks), _dialog(nullptr)
    {
        addRefreshEvent( Event::METRICS_CHANGE);
    }   // end ctor

    QString toolTip() const override
    {
        QStringList txt;
        txt << "Display the University of Washington's Fetal Alcohol Spectrum";
        txt << "Disorder (FASD) facial diagnostic evaluation for the selected model.";
        return txt.join("\n");
    }   // end toolTip

    QString attachToMenu() override { return "Metrics";}

    QString helpFile( const QString& htok) override
    {
        _helptok = htok;
        return ":/FASD/html/MANUAL";
    }   // end helpFile

protected:
    void postInit() override
    {
        QWidget* p = static_cast<QWidget*>(parent());
        _dialog = new FASDDialog(p);
        _dialog->setWindowTitle( p->windowTitle() + " | " + this->displayName());
        connect( _dialog, &FASDDialog::onShowHelp, [this](){ emit FaceAction::onShowHelp(_helptok);});
    }   // end postInit


    bool update( FaceTools::Action::Event e) override
    {
        const FM *fm = MS::selectedModel();
        const bool showing = _dialog->isVisible() && fm && fm->hasLandmarks();
        _dialog->refresh( fm);
        if ( !showing)
            _dialog->hide();
        return showing;
    }   // end update 


    bool isAllowed( FaceTools::Action::Event) override
    {
        const FM* fm = MS::selectedModel();
        return fm && fm->hasLandmarks();
    }   // end isAllowed


    void doAction( FaceTools::Action::Event) override
    {
        _dialog->show();
        _dialog->raise();
        _dialog->activateWindow();
    }   // end doAction

private:
    Cliniface::FASDDialog *_dialog;
    QString _helptok;
};  // end class


FASD::FASD() : QTools::PluginInterface()
{
    appendPlugin( new ActionShowDialog( "FASD Facial Diagnostics", QIcon(":/icons/MEDICAL_FOLDER")));
    FASDDialog::REPORT_NAME = FaceTools::Report::ReportManager::add( ":/reports/REPORT_FASD");
}   // end ctor



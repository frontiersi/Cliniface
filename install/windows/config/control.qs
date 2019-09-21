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

function Controller()
{
}   // end constructor


Controller.prototype.IntroductionPageCallback = function()
{
    if ( installer.isUninstaller())
    {
        var w = gui.currentPageWidget();
        if ( w != null)
        {
            // Don't show buttons since only want to uninstall
            w.findChild( "PackageManagerRadioButton").visible = false;
            w.findChild( "UpdaterRadioButton").visible = false;
            w.findChild( "UninstallerRadioButton").visible = false;
            w.title = "Uninstall Cliniface";
            w.MessageLabel.setText("Click next to uninstall Cliniface.");
        }   // end if
    }   // end if
}   // end function


Controller.prototype.LicenseAgreementPageCallback = function()
{
    var w = gui.currentPageWidget();
    if ( w != null)
        w.AcceptLicenseRadioButton.checked = true;
}   // end function


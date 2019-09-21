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

// Pass to maintenance tool to automatically run uninstallation without needing user interaction.

function Controller()
{
    gui.clickButton(buttons.NextButton);
    gui.clickButton(buttons.NextButton);
    installer.uninstallationFinished.connect(this, this.uninstallationFinished);
}   // end function

Controller.prototype.uninstallationFinished = function()
{
    gui.clickButton(buttons.NextButton);
}   // end function

Controller.prototype.FinishedPageCallback = function()
{
    gui.clickButton(buttons.FinishButton);
}   // end function



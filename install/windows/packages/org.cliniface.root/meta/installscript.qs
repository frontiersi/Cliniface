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

var registerPage = null;
var registerPage = null;

function Component()
{
    component.loaded.connect( this, this.installerLoaded);

    component.fileType0 = "3df";
    component.fileDesc0 = "Cliniface 3D Face Image and Metadata";
    component.fileMedia0 = "application/vnd.cliniface";

    component.fileType1 = "obj";
    component.fileDesc1 = "Wavefront OBJ File Format";
    component.fileMedia1 = "text/plain";

    component.fileType2 = "ply";
    component.fileDesc2 = "Polygon File Format";
    component.fileMedia2 = "text/plain";

    component.fileType3 = "stl";
    component.fileDesc3 = "Stereolithography File Format";
    component.fileMedia3 = "model/stl";
}


// called as soon as the component was loaded
Component.prototype.installerLoaded = function()
{
    // Show only when installing (not updating or uninstalling)
    if (installer.isInstaller())
    {
        // Hide the default target directory page so we can use the custom one
        installer.setDefaultPageVisible( QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible( QInstaller.ComponentSelection, false);

        installer.addWizardPage( component, "TargetWidget", QInstaller.TargetDirectory);
        targetPage = gui.pageWidgetByObjectName( "DynamicTargetWidget");
        targetPage.windowTitle = "Cliniface Installation Directory";
        targetPage.description.setText("Select where to install Cliniface:");
        targetPage.targetDirectory.textChanged.connect( this, this.targetDirectoryChanged);
        targetPage.targetDirectory.setText( installer.value("TargetDir"));
        targetPage.targetChooser.released.connect( this, this.targetChooserClicked);

        gui.pageById( QInstaller.ComponentSelection).entered.connect( this, this.componentSelectionPageEntered);

        installer.addWizardPage( component, "RegisterFileTypesWidget", QInstaller.StartMenuSelection);
        registerPage = gui.pageWidgetByObjectName( "DynamicRegisterFileTypesWidget");
        registerPage.reg0.text = component.fileType0 + " (" + component.fileDesc0 + ")";
        registerPage.reg1.text = component.fileType1 + " (" + component.fileDesc1 + ")";
        registerPage.reg2.text = component.fileType2 + " (" + component.fileDesc2 + ")";
        registerPage.reg3.text = component.fileType3 + " (" + component.fileDesc3 + ")";
    }   // end if
}   // end function


Component.prototype.targetChooserClicked = function()
{
    var dir = QFileDialog.getExistingDirectory("", targetPage.targetDirectory.text);
    targetPage.targetDirectory.setText( dir);
}   // end function


Component.prototype.targetDirectoryChanged = function()
{
    var dir = targetPage.targetDirectory.text;
    
    var wstr = "";
    if ( installer.fileExists(dir))
    {
        wstr = "Selected an existing directory - its contents will be deleted!";
        if ( installer.fileExists( dir + "/ClinifaceMaintenance.exe"))
            wstr = "The existing Cliniface installation will be overwritten.";
    }   // end if
    targetPage.warning.setText("<p style=\"color: red\">" + wstr + "</p>");
    installer.setValue("TargetDir", dir);
}   // end function


Component.prototype.componentSelectionPageEntered = function()
{
    var dir = installer.value("TargetDir");
    if ( installer.fileExists(dir) && installer.fileExists( dir + "/ClinifaceMaintenance.exe"))
        installer.execute( dir + "/ClinifaceMaintenance.exe", "--script=" + dir + "/scripts/auto_uninstall.qs");
}   // end function


function registerFileType( ftype, fdesc, fmedia)
{
    var exePath = "@TargetDir@\\bin\\cliniface.exe"
    component.addOperation("RegisterFileType",
                           ftype,
                           exePath + " \"%1\"",
                           fdesc,
                           fmedia,
                           exePath,
                           "ProgId=Cliniface." + ftype);
}   // end registerFileType


// here we are creating the operation chain which will be processed at the real installation part later
Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows")
    {
        // Register file types
        if ( registerPage.reg0.checked)
            registerFileType( component.fileType0, component.fileDesc0, component.fileMedia0);
        if ( registerPage.reg1.checked)
            registerFileType( component.fileType1, component.fileDesc1, component.fileMedia1);
        if ( registerPage.reg2.checked)
            registerFileType( component.fileType2, component.fileDesc2, component.fileMedia2);
        if ( registerPage.reg3.checked)
            registerFileType( component.fileType3, component.fileDesc3, component.fileMedia3);

        component.addOperation("CreateShortcut",
                               "@TargetDir@/bin/Cliniface.exe",
                               "@StartMenuDir@/Cliniface.lnk",
                               "workingDirectory=@TargetDir@/bin",
                               "iconPath=@TargetDir@/bin/Cliniface.exe",
                               "description=Cliniface (3D Facial Image Visualisation and Analysis)");

        component.addOperation("CreateShortcut",
                               "@TargetDir@/bin/Cliniface.exe",
                               "@DesktopDir@/Cliniface.lnk",
                               "workingDirectory=@TargetDir@/bin",
                               "iconPath=@TargetDir@/bin/Cliniface.exe",
                               "description=Cliniface (3D Facial Image Visualisation and Analysis)");

        component.addOperation("CreateShortcut",
                               "@TargetDir@/ClinifaceMaintenance.exe",
                               "@StartMenuDir@/ClinifaceMaintenance.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=@TargetDir@/bin/Cliniface.exe",
                               "description=Cliniface Update/Uninstall");
    }   // end if
}   // end createOperations


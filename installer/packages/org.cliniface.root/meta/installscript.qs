/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    component.loaded.connect( this, addRegisterFileCheckBox);
    component.fileType0 = "3df"
    component.fileType1 = "obj"
}   // end constructor


// called as soon as the component was loaded
addRegisterFileCheckBox = function()
{
    // Show only when installing (not updating or uninstalling)
    if (installer.isInstaller())
    {
        installer.addWizardPageItem(component, "RegisterFileCheckBoxForm", QInstaller.TargetDirectory);
        component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox0.text =
            component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox0.text + component.fileType0;
        component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox1.text =
            component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox1.text + component.fileType1;
    }   // end if
}   // end addRegisterFileCheckBox


function registerFileType( ftype, fdesc)
{
    var exePath = "@TargetDir@\\bin\\Cliniface.exe"
    component.addOperation("RegisterFileType",
                           ftype,
                           exePath + " \"%1\"",
                           fdesc,
                           "application/x-binary",
                           exePath,
                           "ProgId=Cliniface." + ftype);
}   // end registerFileType


// here we are creating the operation chain which will be processed at the real installation part later
Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows")
    {
        // Register the file types
        if ( component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox0.checked)
            registerFileType( component.fileType0, "Cliniface 3DF")
        if ( component.userInterface("RegisterFileCheckBoxForm").RegisterFileCheckBox1.checked)
            registerFileType( component.fileType1, "Wavefront OBJ")

        component.addOperation("CreateShortcut", "@TargetDir@/bin/Cliniface.exe", "@StartMenuDir@/Cliniface.lnk",
                               "workingDirectory=@TargetDir@/bin", "iconPath=@TargetDir@/bin/Cliniface.exe",
                               "iconId=0", "description=Cliniface (3D Facial Image Visualisation and Analysis)");

        component.addOperation("CreateShortcut", "@TargetDir@/ClinifaceMaintenance.exe", "@StartMenuDir@/ClinifaceMaintenance.lnk",
                               "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/bin/Cliniface.exe",
                               "iconId=0", "description=Cliniface Update/Uninstall");
    }   // end if
}   // end createOperations

cmake_minimum_required(VERSION 3.12.2 FATAL_ERROR)

PROJECT(Cliniface)

set( APP_NAME          "${PROJECT_NAME}")
set( APP_CONTACT_EMAIL "r.l.palmer@curtin.edu.au")
set( APP_AUTHOR_NAME   "Richard Palmer")
set( APP_ORGANISATION  "Spatial Information Systems Research Limited")
set( APP_DESCRIPTION   "3D Facial Image Visualisation and Analysis")
set( APP_WEBSITE       "https://${PROJECT_NAME}.org")
set( APP_SOURCE        "https://github.com/crc-si/${PROJECT_NAME}/releases")

# Version code: A.B.C.D
# A: APP_VERSION_MAJOR - Significant feature additions or major changes to how the user works with the tool.
# B: APP_VERSION_MINOR - Some feature additions or significant changes to how existing features are implemented.
# C: APP_VERSION_PATCH - Bug fixes and/or minor changes to how existing features are implemented.
# D: APP_BUILD_DTSTAMP - Date-time stamp of build (coordinated universal time).
set( APP_VERSION_MAJOR 3)
set( APP_VERSION_MINOR 2)
set( APP_VERSION_PATCH 0)
string( TIMESTAMP APP_BUILD_DTSTAMP "%y%m%d" UTC)
set( APP_VERSION_STRING "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_BUILD_DTSTAMP}")

set( _data "data")

set( _faceModelsDir "${_data}/facemodels")
set( _haar_cascades_models "${_faceModelsDir}/haarcascades")
set( _face_shape_landmarks "${_faceModelsDir}/shape_predictor_68_face_landmarks.dat")

# Set locations of IDTF converter (for U3D export) and pdflatex (assume texlive installed for Linux)
set( _u3dIntel "u3dIntel")
set( _pdflatex "/usr/bin/pdflatex")
set( _idtf_converter "${_u3dIntel}/bin/IDTFConverter")
set( _pdfreader "/usr/bin/evince")
set( _inkscape "/usr/bin/inkscape")
if(WIN32)
    set( _pdflatex "texmfs/install/miktex/bin/pdflatex.exe")
    set( _u3dIntel "u3dIntelWin")
    set( _idtf_converter "${_u3dIntel}/IDTFConverter.exe")
    set( _pdfreader "/Program Files (x86)/Adobe/Acrobat Reader DC/Reader/AcroRd32.exe")
    set( _inkscape "/Program Files/Inkscape/inkscape.exe")
endif()

file( TO_CMAKE_PATH "$ENV{DEV_PARENT_DIR}" DEV_PARENT_DIR)

set( WITH_FACETOOLS TRUE)
include( "${DEV_PARENT_DIR}/libbuild/cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set( FORMS_DIR "${PROJECT_SOURCE_DIR}/forms")

# Configure application includes and the preferences file
set( _app_prefs "prefs.lua")
configure_file( "prefs.lua.in" "${_app_prefs}")
configure_file( "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in" "${PROJECT_NAME}_Config.h")


set( FORMS
    "${FORMS_DIR}/${PROJECT_NAME}Main.ui"
    "${FORMS_DIR}/AboutDialog.ui"
    "${FORMS_DIR}/HelpDialog.ui"
    )

set( QOBJECTS
    "${INCLUDE_DIR}/${PROJECT_NAME}Main.h"
    "${INCLUDE_DIR}/AboutDialog.h"
    "${INCLUDE_DIR}/HelpDialog.h"
    "${INCLUDE_DIR}/ClinifacePluginsLoader.h"
    "${INCLUDE_DIR}/VisualisationsOrganiser.h"
    )

set( INCLUDE_FILES
    ${QOBJECTS}
    "${INCLUDE_DIR}/Preferences.h"
    )

set( SRC_FILES
    ${SRC_DIR}/main
    ${SRC_DIR}/${PROJECT_NAME}Main
    ${SRC_DIR}/AboutDialog
    ${SRC_DIR}/HelpDialog
    ${SRC_DIR}/ClinifacePluginsLoader
    ${SRC_DIR}/Preferences
    ${SRC_DIR}/VisualisationsOrganiser
    )

set( RESOURCE_FILE "resources.qrc")
qt5_wrap_ui( FORM_HEADERS ${FORMS})
qt5_wrap_cpp( QOBJECT_MOCS ${QOBJECTS})
qt5_add_resources( RESOURCE_FILE ${RESOURCE_FILE})

# Resource file for exe icon (used by Windows)
set( ICON_RC "icon.rc")

include_directories( ${INCLUDE_DIR})
include_directories( ${PROJECT_BINARY_DIR})

add_executable( ${PROJECT_NAME} WIN32  
    ${SRC_FILES}
    ${INCLUDE_FILES}
    ${FORMS}
    ${FORM_HEADERS}
    ${QOBJECT_MOCS}
    ${ICON_RC}
    ${RESOURCE_FILE}
    "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in"
    )

set( _exe_dir "${PROJECT_BINARY_DIR}/bin")
set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${_exe_dir}")

# On Windows, there's no RPATH so we must collect all the DLLs and place into the project binary dir.
include( "${DEV_PARENT_DIR}/libbuild/cmake/ExeInstall.cmake")


set( _pluginsDir "${LIB_PRE_REQS}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/plugins")
add_custom_target( a1 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${_pluginsDir} ${_exe_dir}/plugins COMMENT "Copying over plugins")
add_custom_target( a2 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${_data} ${_exe_dir}/${_data} COMMENT "Copying over data")


######## Copy/extract ancillary files to build directory ##

file( COPY "${PROJECT_BINARY_DIR}/${_app_prefs}" DESTINATION "${_exe_dir}")

# Unzip the examples archive into the build location.
if ( NOT EXISTS "${PROJECT_BINARY_DIR}/examples")
    message( STATUS "Extracting examples into ${PROJECT_BINARY_DIR}")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/examples.tgz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
endif()

# Extract the IDTFConverter tool (for U3D export) into the build binary directory
if ( NOT EXISTS "${_exe_dir}/${_u3dIntel}")
    message( STATUS "Extracting ${_u3dIntel} into ${_exe_dir}")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/${_u3dIntel}.tgz" WORKING_DIRECTORY "${_exe_dir}")
endif()

# Extract the face detection models to the build binary data directory
if ( NOT EXISTS "${_exe_dir}/${_faceModelsDir}")
    message( STATUS "Extracting facemodels.tgz into ${_exe_dir}/${_data}")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/facemodels.tgz" WORKING_DIRECTORY "${_exe_dir}/${_data}")
endif()

# On Linux, we assume that there's a valid installation of pdflatex on the path (though extra packages may be needed at runtime).
if ( WIN32 AND NOT EXISTS "${_exe_dir}/texmfs")
    message( STATUS "Extracting texmfs into ${_exe_dir}")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/texmfs.tgz" WORKING_DIRECTORY "${_exe_dir}")
endif()


######## Install into packages for QtInstallerFramework
set( APP_ADMIN_TARGET_DIR "@ApplicationsDirX64@/${APP_NAME}")
set( APP_TARGET_DIR       "@HomeDir@/${APP_NAME}")
set( APP_RUN_PROGRAM      "@TargetDir@/bin/${APP_NAME}")
set( APP_REPO_0           "https://frontiersi.github.io/${PROJECT_NAME}Web/repo")
set( APP_REPO_0_TXT       "Main ${PROJECT_NAME} Repository")
set( APP_REPO_0_ENBLD 1)
string( TIMESTAMP APP_RELEASE_DATE "%Y-%m-%d" UTC)

set( _config "config")
set( _packages "packages")
set( _configXML "${_config}/config.xml")
set( _packageRoot "${_packages}/org.cliniface.root")
set( _rootPackageXML "${_packageRoot}/meta/package.xml")

# Copy over the base directory structure for the config and packages folders from the source directory.
install( DIRECTORY "installer/" DESTINATION "${PROJECT_BINARY_DIR}" PATTERN "*.py" EXCLUDE PATTERN "*.xml.in" EXCLUDE)
# Configure the required package installer configuration files
configure_file( "installer/${_rootPackageXML}.in" "${_rootPackageXML}")
configure_file( "installer/${_configXML}.in"      "${_configXML}")

# Create the "data" directories with required folder structure
set( _packageRootData "${_packageRoot}/data")
set( _packageTexmfsData "${_packageRoot}.texmfs/data")
set( _packagePluginsData "${_packageRoot}.plugins/data")
set( _packageExamplesData "${_packageRoot}.examples/data")

install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packageRootData}")
install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packageTexmfsData}")
install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packagePluginsData}")
install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packageExamplesData}")
set( _packagePluginsData "${_packagePluginsData}/bin")
set( _packageTexmfsData "${_packageTexmfsData}/bin")
install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packagePluginsData}")
install( DIRECTORY DESTINATION "${PROJECT_BINARY_DIR}/${_packageTexmfsData}")

# Install the config and packages directories in the installation directory
install( DIRECTORY "${PROJECT_BINARY_DIR}/${_config}"   DESTINATION ".")
install( DIRECTORY "${PROJECT_BINARY_DIR}/${_packages}" DESTINATION ".")

# Install the package data (note initial exclusion of u3dIntel - this is because it has its own Plugins directory)
install( DIRECTORY "${_exe_dir}"              DESTINATION "${_packageRootData}" USE_SOURCE_PERMISSIONS
                                                            PATTERN "${_u3dIntel}" EXCLUDE
                                                            PATTERN "plugins/*" EXCLUDE
                                                            PATTERN "texmfs" EXCLUDE)
install( DIRECTORY "${_exe_dir}/${_u3dIntel}" DESTINATION "${_packageRootData}/bin" USE_SOURCE_PERMISSIONS)
install( DIRECTORY "${_exe_dir}/plugins"      DESTINATION "${_packagePluginsData}" USE_SOURCE_PERMISSIONS)
install( DIRECTORY "${PROJECT_BINARY_DIR}/examples"         DESTINATION "${_packageExamplesData}" USE_SOURCE_PERMISSIONS)

if ( WIN32) # Install texmfs on Windows
    install( DIRECTORY "${_exe_dir}/texmfs" DESTINATION "${_packageTexmfsData}" USE_SOURCE_PERMISSIONS)
endif()

# Run the Qt installer framework binary generator tool
if ( EXISTS "${QT_INF_BINARY_CREATOR}")
    set( _flags "-v --offline-only")
    set( _iname "${PROJECT_NAME}OfflineInstaller_${APP_VERSION_STRING}")
    install( CODE "execute_process( COMMAND ${QT_INF_BINARY_CREATOR} ${_flags} -c ${_configXML} -p ${_packages} ${_iname}
                                    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})")
else()
    message( WARNING "Cannot find QtInstallerFramework tools! Tried looking in ${QT_INSTALLER_FRAMEWORK}")
endif()

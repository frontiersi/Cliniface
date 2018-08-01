cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

PROJECT( Cliniface)

set( APP_AUTHOR_NAME  "Richard Palmer")
set( APP_AUTHOR_EMAIL "r.l.palmer@curtin.edu.au")
set( APP_NAME         "${PROJECT_NAME}")
set( APP_DESCRIPTION  "3D Facial Image Visualisation and Analysis")
set( APP_WEBSITE      "http://${PROJECT_NAME}.org")
set( APP_SOURCE       "https://github.com/richeytastic/${PROJECT_NAME}/releases")

# Version code: A.B.C.D
# A: APP_VERSION_MAJOR - Significant feature additions or major changes to how the user works with the tool.
# B: APP_VERSION_MINOR - Some feature additions or significant changes to how existing features are implemented.
# C: APP_VERSION_PATCH - Bug fixes and/or minor changes to how existing features are implemented.
# D: APP_BUILD_DTSTAMP - Date-time stamp of build (coordinated universal time).
set( APP_VERSION_MAJOR 3)
set( APP_VERSION_MINOR 0)
set( APP_VERSION_PATCH 7)
string( TIMESTAMP APP_BUILD_DTSTAMP "%y%m%d" UTC)
set( APP_VERSION_STRING "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_BUILD_DTSTAMP}")


set( _faceModelsDir "facemodels")
set( HAAR_CASCADES_MODELS "${_faceModelsDir}/haarcascades")
set( FACE_SHAPE_LANDMARKS "${_faceModelsDir}/shape_predictor_68_face_landmarks.dat")

# Set locations of IDTF converter (for U3D export) and pdflatex
set( _pdflatex "pdflatex")
set( _u3dIntel "u3dIntel")
set( _idtf_converter "${_u3dIntel}/bin/IDTFConverter")
if(WIN32)
    set( _pdflatex "texmfs/install/miktex/bin/pdflatex.exe")
    set( _u3dIntel "u3dIntelWin")
    set( _idtf_converter "${_u3dIntel}/IDTFConverter.exe")
endif()
set( PDF_LATEX "${_pdflatex}")
set( IDTF_CONVERTER "${_idtf_converter}")

set( WITH_FACETOOLS TRUE)
file( TO_CMAKE_PATH $ENV{DEV_PARENT_DIR} DEV_PARENT_DIR)
include( "${DEV_PARENT_DIR}/libbuild/cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set( FORMS_DIR "${PROJECT_SOURCE_DIR}/forms")

configure_file( "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in" "${PROJECT_NAME}_Config.h")


set( FORMS
    "${FORMS_DIR}/${PROJECT_NAME}Main.ui"
    "${FORMS_DIR}/AboutDialog.ui"
    "${FORMS_DIR}/HelpDialog.ui"
    "${FORMS_DIR}/LicensesDialog.ui"
    )

set( QOBJECTS
    "${INCLUDE_DIR}/${PROJECT_NAME}Main.h"
    "${INCLUDE_DIR}/AboutDialog.h"
    "${INCLUDE_DIR}/HelpDialog.h"
    "${INCLUDE_DIR}/LicensesDialog.h"
    )

set( INCLUDE_FILES
    ${QOBJECTS}
    )

set( SRC_FILES
    ${SRC_DIR}/main
    ${SRC_DIR}/${PROJECT_NAME}Main
    ${SRC_DIR}/AboutDialog
    ${SRC_DIR}/HelpDialog
    ${SRC_DIR}/LicensesDialog
    )

set( RESOURCE_FILE "resources.qrc")
qt5_wrap_ui( FORM_HEADERS ${FORMS})
qt5_wrap_cpp( QOBJECT_MOCS ${QOBJECTS})
qt5_add_resources( RESOURCE_FILE ${RESOURCE_FILE})

# Resource file for exe icon (used by Windows)
set( ICON_RC "icon.rc")

include_directories( ${INCLUDE_DIR})
include_directories( ${PROJECT_BINARY_DIR})

file( GLOB _installerFiles "${PROJECT_SOURCE_DIR}/installer/*")

# Add WIN32 to prevent console window from opening
add_executable( ${PROJECT_NAME} WIN32
    ${SRC_FILES}
    ${INCLUDE_FILES}
    ${FORMS}
    ${FORM_HEADERS}
    ${QOBJECT_MOCS}
    ${ICON_RC}
    ${RESOURCE_FILE}
    "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in"
    ${_installerFiles}
    )

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# On Windows, there's no RPATH so we must collect all the DLLs and place into the project binary dir.
include( "${DEV_PARENT_DIR}/libbuild/cmake/ExeInstall.cmake")


######## Discover the separately compiled plugins #########
set( _pluginsSourceDir "${LIB_PRE_REQS}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/bin")
file( GLOB _plugins "${_pluginsSourceDir}/*")
# Copy over plugins to build directory
set( _pluginsDir "${PROJECT_BINARY_DIR}/bin/plugins")
file( MAKE_DIRECTORY "${_pluginsDir}")
message( STATUS "Copying plugins from ${_pluginsSourceDir} to ${_pluginsDir}:")
foreach( _plugin ${_plugins})
    message( STATUS " --> ${_plugin}")
endforeach( _plugin)
file( INSTALL ${_plugins} DESTINATION "${_pluginsDir}")


######## Copy/extract ancillary files to build directory ##
# Copy over default application preferences configuration
#set( APP_PREFS "prefs.etc")
#file( INSTALL "${APP_PREFS}" DESTINATION "${PROJECT_BINARY_DIR}/bin")

# Copy across the docs directory into the build location.
file( INSTALL "docs" DESTINATION "${PROJECT_BINARY_DIR}")

# Unzip the examples archive into the build location.
if ( NOT EXISTS "${PROJECT_BINARY_DIR}/examples")
    message( STATUS "Extracting examples into ${PROJECT_BINARY_DIR}")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/examples.tgz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
endif()

# Extract the IDTFConverter tool (for U3D export) into the build binary directory
if ( NOT EXISTS "${PROJECT_BINARY_DIR}/bin/${_u3dIntel}")
    message( STATUS "Extracting ${_u3dIntel} into ${PROJECT_BINARY_DIR}/bin")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/${_u3dIntel}.tgz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()

# Extract the face detection models to the build binary directory
if ( NOT EXISTS "${PROJECT_BINARY_DIR}/bin/${_faceModelsDir}")
    message( STATUS "Extracting ${_faceModelsDir} into ${PROJECT_BINARY_DIR}/bin")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/facemodels.tgz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()

# On Linux, we assume that there's a valid installation of pdflatex on the path (though extra packages may be needed at runtime).
if ( WIN32 AND NOT EXISTS "${PROJECT_BINARY_DIR}/bin/texmfs")
    message( STATUS "Extracting texmfs into ${PROJECT_BINARY_DIR}/bin")
    execute_process( COMMAND ${CMAKE_COMMAND} -E tar xzf "${PROJECT_SOURCE_DIR}/texmfs.tgz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()


######## Install into packages for QtInstallerFramework
set( APP_ADMIN_TARGET_DIR "@ApplicationsDirX64@/${APP_NAME}")
set( APP_TARGET_DIR       "@HomeDir@/${APP_NAME}")
set( APP_RUN_PROGRAM      "@TargetDir@/bin/${APP_NAME}")
set( APP_REPO_0           "https://richeytastic.github.io/${PROJECT_NAME}Web/repo")
set( APP_REPO_0_TXT       "Main ${PROJECT_NAME} Repository")
set( APP_REPO_0_ENBLD 1)
string( TIMESTAMP APP_RELEASE_DATE "%Y-%m-%d" UTC)

set( _config "config")
set( _packages "packages")
set( _configXML "${_config}/config.xml")
set( _packageRoot "${_packages}/org.cliniface.root")
set( _rootPackageXML "${_packageRoot}/meta/package.xml")

# Copy over the base directory structure for the config and packages folders from the source directory.
file( INSTALL "installer/" DESTINATION "${PROJECT_BINARY_DIR}" PATTERN "*.py" EXCLUDE PATTERN "*.xml.in" EXCLUDE)
# Configure the required package installer configuration files
configure_file( "installer/${_rootPackageXML}.in" "${_rootPackageXML}")
configure_file( "installer/${_configXML}.in"      "${_configXML}")

# Create the "data" directories with required folder structure
set( _packageRootData "${_packageRoot}/data")
set( _packageDocsData "${_packageRoot}.docs/data")
set( _packageTexmfsData "${_packageRoot}.texmfs/data")
set( _packagePluginsData "${_packageRoot}.plugins/data")
set( _packageExamplesData "${_packageRoot}.examples/data")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packageRootData}")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packageDocsData}")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packageTexmfsData}")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packagePluginsData}")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packageExamplesData}")
set( _packagePluginsData "${_packagePluginsData}/bin")
set( _packageTexmfsData "${_packageTexmfsData}/bin")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packagePluginsData}")
file( MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_packageTexmfsData}")

# Install the config and packages directories in the installation directory
install( DIRECTORY "${PROJECT_BINARY_DIR}/${_config}"   DESTINATION ".")
install( DIRECTORY "${PROJECT_BINARY_DIR}/${_packages}" DESTINATION ".")

# Install the package data (note initial exclusion of u3dIntel - this is because it has its own Plugins directory)
install( DIRECTORY "${PROJECT_BINARY_DIR}/bin"              DESTINATION "${_packageRootData}" USE_SOURCE_PERMISSIONS
                                                            PATTERN "${_u3dIntel}" EXCLUDE
                                                            PATTERN "plugins/*" EXCLUDE
                                                            PATTERN "texmfs" EXCLUDE)
install( DIRECTORY "${PROJECT_BINARY_DIR}/bin/${_u3dIntel}" DESTINATION "${_packageRootData}/bin")
install( DIRECTORY "${PROJECT_BINARY_DIR}/bin/plugins"      DESTINATION "${_packagePluginsData}")
install( DIRECTORY "${PROJECT_BINARY_DIR}/examples"         DESTINATION "${_packageExamplesData}")
install( DIRECTORY "${PROJECT_BINARY_DIR}/docs"             DESTINATION "${_packageDocsData}")

if ( WIN32) # Install texmfs on Windows
    install( DIRECTORY "${PROJECT_BINARY_DIR}/bin/texmfs" DESTINATION "${_packageTexmfsData}" USE_SOURCE_PERMISSIONS)
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

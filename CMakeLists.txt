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

configure_file( "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}_Config.h")


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

set( INSTALLER_DIR "installer")
file( GLOB _installerFiles "${PROJECT_SOURCE_DIR}/${INSTALLER_DIR}/*")

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
file( COPY ${_plugins} DESTINATION "${_pluginsDir}")
###########################################################


######## Copy/extract ancillary files to build directory ##
# Copy over default application preferences configuration
#set( APP_PREFS "prefs.etc")
#file( COPY "${APP_PREFS}" DESTINATION "${PROJECT_BINARY_DIR}/bin")

# Copy across the docs directory into the build location.
file( COPY "${PROJECT_SOURCE_DIR}/docs" DESTINATION "${PROJECT_BINARY_DIR}")

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
###########################################################


######## Do Installation ##################################
install( DIRECTORY "${INSTALLER_DIR}/" DESTINATION "${CMAKE_INSTALL_PREFIX}" PATTERN "*.in" EXCLUDE
                                                                             PATTERN "*.py" EXCLUDE)

install( DIRECTORY "${PROJECT_BINARY_DIR}/docs"     DESTINATION "packages/org.cliniface.root.docs/data")
install( DIRECTORY "${PROJECT_BINARY_DIR}/examples" DESTINATION "packages/org.cliniface.root.examples/data")
install( DIRECTORY "${PROJECT_BINARY_DIR}/bin"      DESTINATION "packages/org.cliniface.root/data"
                                                    USE_SOURCE_PERMISSIONS
                                                    PATTERN "texmfs" EXCLUDE
                                                    PATTERN "plugins/*" EXCLUDE)
# Install the plugins
set( PACKAGE_PLUGINS_INSTALLER_DATA "packages/org.cliniface.root.plugins/data")
install( DIRECTORY DESTINATION "${PACKAGE_PLUGINS_INSTALLER_DATA}/bin")
install( DIRECTORY "${PROJECT_BINARY_DIR}/bin/plugins" DESTINATION "${PACKAGE_PLUGINS_INSTALLER_DATA}/bin" USE_SOURCE_PERMISSIONS)

# Install texmfs on Windows
if ( WIN32 AND EXISTS "${PROJECT_BINARY_DIR}/bin/texmfs")
    set( PACKAGE_REPORTS_INSTALLER_DATA "packages/org.cliniface.root.reports/data")
    install( DIRECTORY DESTINATION "${PACKAGE_REPORTS_INSTALLER_DATA}/bin")
    install( DIRECTORY "${PROJECT_BINARY_DIR}/bin/texmfs" DESTINATION "${PACKAGE_REPORTS_INSTALLER_DATA}/bin" USE_SOURCE_PERMISSIONS)
endif()

######## Configure installer files ########################
set( _installer_config_xml "config/config.xml")
set( _installer_root_package_xml "packages/org.cliniface.root/meta/package.xml")
set( APP_TARGET_DIR "@HomeDir@/${APP_NAME}")
set( APP_ADMIN_TARGET_DIR "@ApplicationsDirX64@/${APP_NAME}")
set( APP_RUN_PROGRAM "@TargetDir@/bin/${APP_NAME}")
set( APP_REPO_0       "https://richeytastic.github.io/${PROJECT_NAME}Web/repo")
set( APP_REPO_0_TXT   "Main ${PROJECT_NAME} Repository")
set( APP_REPO_0_ENBLD 1)
string( TIMESTAMP APP_RELEASE_DATE "%Y-%m-%d" UTC)
configure_file( "installer/${_installer_config_xml}.in" "${CMAKE_INSTALL_PREFIX}/${_installer_config_xml}")
configure_file( "installer/${_installer_root_package_xml}.in" "${CMAKE_INSTALL_PREFIX}/${_installer_root_package_xml}")

# Configure and run the Qt installer framework tool
set( QT_INSTALLER_FRAMEWORK "${Qt5_DIR}/../../../../../Tools/QtInstallerFramework/3.0/bin")
set( QT_BINARY_CREATOR "${QT_INSTALLER_FRAMEWORK}/binarycreator")
set( QT_REPO_GEN "${QT_INSTALLER_FRAMEWORK}/repogen")   # Only needed for online installer (repository generation)
if ( WIN32)
    set( QT_BINARY_CREATOR "${QT_BINARY_CREATOR}.exe")
    set( QT_REPO_GEN "${QT_REPO_GEN}.exe")
endif()
set( QT_BIN_CREATOR_FLAGS "-v --offline-only")
set( INSTALLER_NAME "${PROJECT_NAME}OfflineInstaller_${APP_VERSION_STRING}")

if ( EXISTS "${QT_BINARY_CREATOR}")
    install( CODE "execute_process( COMMAND ${QT_BINARY_CREATOR} -c config/config.xml -p packages ${QT_BIN_CREATOR_FLAGS} ${INSTALLER_NAME}
                                    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})")
else()
    message( WARNING "Cannot find QtInstallerFramework tools! Tried looking in ${QT_INSTALLER_FRAMEWORK}")
endif()
###########################################################

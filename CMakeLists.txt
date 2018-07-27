cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

PROJECT( Cliniface)

# Version code: A.B.C.D
# A: APP_VERSION_MAJOR - Significant feature additions or major changes to how the user works with the tool.
# B: APP_VERSION_MINOR - Some feature additions or significant changes to how existing features are implemented.
# C: APP_VERSION_PATCH - Bug fixes and/or minor changes to how existing features are implemented.
# D: APP_BUILD_DTSTAMP - Date-time stamp of build (coordinated universal time).
set( APP_VERSION_MAJOR 3)
set( APP_VERSION_MINOR 0)
set( APP_VERSION_PATCH 4)
string( TIMESTAMP APP_BUILD_DTSTAMP "%y%m%d" UTC)
set( APP_AUTHOR_NAME "\"Richard Palmer\"")
set( APP_AUTHOR_EMAIL "\"r.l.palmer@curtin.edu.au\"")
set( APP_NAME "\"Cliniface\"")
set( APP_DESCRIPTION "\"\"")
set( APP_WEBSITE "\"http://cliniface.org\"")
set( APP_SOURCE  "\"https://github.com/richeytastic/Cliniface/releases\"")

set( _faceModelsDir "facemodels")
set( HAAR_CASCADES_MODELS "\"${_faceModelsDir}/haarcascades\"")
set( FACE_SHAPE_LANDMARKS "\"${_faceModelsDir}/shape_predictor_68_face_landmarks.dat\"")

# Set locations of IDTF converter (for U3D export) and pdflatex
set( _pdflatex "pdflatex")
set( _u3dIntel "u3dIntel")
set( _idtf_converter "${_u3dIntel}/bin/IDTFConverter")
if(WIN32)
    set( _pdflatex "texmfs/install/miktex/bin/pdflatex.exe")
    set( _u3dIntel "u3dIntelWin")
    set( _idtf_converter "${_u3dIntel}/IDTFConverter.exe")
endif()
set( PDF_LATEX "\"${_pdflatex}\"")
set( IDTF_CONVERTER "\"${_idtf_converter}\"")

#set( APP_OPTION_PREFS "\"prefs.etc\"")
#file( COPY "prefs.etc" DESTINATION "${PROJECT_BINARY_DIR}/bin") # Copy over the preferences file

set( WITH_FACETOOLS TRUE)
file( TO_CMAKE_PATH $ENV{DEV_PARENT_DIR} DEV_PARENT_DIR)
include( "${DEV_PARENT_DIR}/libbuild/cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set( FORMS_DIR "${PROJECT_SOURCE_DIR}/forms")

configure_file( "${INCLUDE_DIR}/${PROJECT_NAME}_Config.h.in"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}_Config.h")

set( FORMS
    "${FORMS_DIR}/ClinifaceMain.ui"
    "${FORMS_DIR}/AboutDialog.ui"
    "${FORMS_DIR}/HelpDialog.ui"
    "${FORMS_DIR}/LicensesDialog.ui"
    )

set( QOBJECTS
    "${INCLUDE_DIR}/ClinifaceMain.h"
    "${INCLUDE_DIR}/AboutDialog.h"
    "${INCLUDE_DIR}/HelpDialog.h"
    "${INCLUDE_DIR}/LicensesDialog.h"
    )

set( INCLUDE_FILES
    ${QOBJECTS}
    )

set( SRC_FILES
    ${SRC_DIR}/main
    ${SRC_DIR}/ClinifaceMain
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
    )

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# On Windows, there's no RPATH so we must collect all the DLLs and place into the project binary dir.
include( "${DEV_PARENT_DIR}/libbuild/cmake/ExeInstall.cmake")

# Copy over dynamically loaded plugins to bin/plugins directory
file( GLOB _plugins "${LIB_PRE_REQS}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/bin/*")
set( _pluginsDir "${PROJECT_BINARY_DIR}/bin/plugins")
file( MAKE_DIRECTORY "${_pluginsDir}")
message( STATUS "Copying over the following plugins to ${_pluginsDir}:")
message( STATUS " +  ${_plugins}")
file( COPY ${_plugins} DESTINATION ${_pluginsDir})

# Copy across the docs, and installer directories into the build location.
file( COPY "${PROJECT_SOURCE_DIR}/docs" DESTINATION "${PROJECT_BINARY_DIR}")
file( COPY "${PROJECT_SOURCE_DIR}/installer" DESTINATION "${PROJECT_BINARY_DIR}")

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

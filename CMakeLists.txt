cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

PROJECT( Cliniface)

# Version code: A.B.C.D
# A: APP_VERSION_MAJOR - Significant feature additions or major changes to how the user works with the tool.
# B: APP_VERSION_MINOR - Some feature additions or significant changes to how existing features are implemented.
# C: APP_VERSION_PATCH - Bug fixes and/or minor changes to how existing features are implemented.
# D: APP_BUILD_DTSTAMP - Date-time stamp of build (coordinated universal time).
set( APP_VERSION_MAJOR 3)
set( APP_VERSION_MINOR 0)
set( APP_VERSION_PATCH 2)
string( TIMESTAMP APP_BUILD_DTSTAMP "%y%m%d" UTC)
set( APP_AUTHOR_NAME "\"Richard Palmer\"")
set( APP_AUTHOR_EMAIL "\"r.l.palmer@curtin.edu.au\"")
set( APP_NAME "\"Cliniface\"")
set( APP_DESCRIPTION "\"\"")
set( APP_WEBSITE "\"http://cliniface.org\"")
set( APP_SOURCE  "\"https://github.com/richeytastic/Cliniface\"")

# Copy across the face detection models to the build binary directory
set( FACE_MODELS_DIR "facemodels")
set( HAAR_CASCADES_MODELS "\"${FACE_MODELS_DIR}/haarcascades\"")
set( FACE_SHAPE_LANDMARKS "\"${FACE_MODELS_DIR}/shape_predictor_68_face_landmarks.dat\"")
file( COPY "${PROJECT_SOURCE_DIR}/${FACE_MODELS_DIR}" DESTINATION "${PROJECT_BINARY_DIR}/bin")

# Copy across the IDTF converter and pdflatex (WIN32)
if(WIN32)
    file( COPY "${PROJECT_SOURCE_DIR}/u3dIntelWin" DESTINATION "${PROJECT_BINARY_DIR}/bin")
    set( IDTF_CONVERTER "\"u3dIntelWin/IDTFConverter.exe\"")
    if ( NOT EXISTS "${PROJECT_BINARY_DIR}/bin/texmfs")
        file( COPY "${PROJECT_SOURCE_DIR}/texmfs" DESTINATION "${PROJECT_BINARY_DIR}/bin")  # MiKTeX portable
    endif()
    set( PDF_LATEX "\"texmfs/install/miktex/bin/pdflatex.exe\"")
else()
    file( COPY "${PROJECT_SOURCE_DIR}/u3dIntel" DESTINATION "${PROJECT_BINARY_DIR}/bin")
    set( IDTF_CONVERTER "\"u3dIntel/bin/IDTFConverter\"")
    set( PDF_LATEX "\"pdflatex\"")
endif()

# Copy across the docs and examples directories into the build location.
file( COPY "${PROJECT_SOURCE_DIR}/docs" DESTINATION "${PROJECT_BINARY_DIR}")
file( COPY "${PROJECT_SOURCE_DIR}/examples" DESTINATION "${PROJECT_BINARY_DIR}")

#set( APP_OPTION_PREFS "\"prefs.etc\"")
#file( COPY "prefs.etc" DESTINATION "${PROJECT_BINARY_DIR}/bin") # Copy over the preferences file


set( SHOW_SPLASH 0)

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


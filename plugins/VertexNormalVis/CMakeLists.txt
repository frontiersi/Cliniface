cmake_minimum_required(VERSION 3.12.2 FATAL_ERROR)

project( Cliniface_Plugin_VertexNormalVis)

file( TO_CMAKE_PATH "$ENV{DEV_PARENT_DIR}" DEV_PARENT_DIR)

set(WITH_FACETOOLS TRUE)
include( "${DEV_PARENT_DIR}/libbuild/cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include_directories( ${INCLUDE_DIR})
include_directories( "$ENV{BUILD_PARENT_DIR}/Cliniface/${CMAKE_BUILD_TYPE}")    # For Cliniface_Config.h

set( SOURCES
    "${SRC_DIR}/VertexNormalVis.cpp"
    )

set( QOBJECTS
    "${INCLUDE_DIR}/VertexNormalVis.h"
    )

set( RCC_FILE "resources.qrc")

qt5_wrap_cpp( QOBJECT_MOCS ${QOBJECTS})
qt5_add_resources( RESOURCE_FILE ${RCC_FILE})

add_library( ${PROJECT_NAME} ${SOURCES} ${QOBJECTS} ${QOBJECT_MOCS} ${RESOURCE_FILE} ${RCC_FILE})

# Require generate_export_header for .lib import libraries associated with Windows DLLs (if BUILD_SHARED_LIBS is true)
include(GenerateExportHeader)
generate_export_header( ${PROJECT_NAME}
                        BASE_NAME ${PROJECT_NAME}
                        EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORT
                        EXPORT_FILE_NAME ${PROJECT_NAME}_Export.h
                        STATIC_DEFINE ${PROJECT_NAME}_BUILT_AS_STATIC)
include_directories( ${PROJECT_BINARY_DIR}) # Include because generated header files sit here.

# Rename the library if compiled using MSVC to append the MSVC version so as to avoid DLL HELL.
if(WIN32)
    include( "${DEV_PARENT_DIR}/libbuild/cmake/Macros.cmake")
    get_msvc_version( _msvcv)
    set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "-vc${_msvcv}0.dll")
    set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "-vc${_msvcv}0.lib")
endif(WIN32)

set_target_properties( ${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "."
                                 RUNTIME DESTINATION ".")

include( "${DEV_PARENT_DIR}/libbuild/cmake/LinkTargets.cmake")

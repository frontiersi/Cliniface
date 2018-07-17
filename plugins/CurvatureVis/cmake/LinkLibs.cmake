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
    include("cmake/Macros.cmake")
    get_msvc_version( _msvcv)
    set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "-vc${_msvcv}0.dll")
    set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "-vc${_msvcv}0.lib")
endif(WIN32)

install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin"
                                 LIBRARY DESTINATION "lib"
                                 ARCHIVE DESTINATION "lib/static")
install( FILES ${INCLUDE_FILES} DESTINATION "include")
install( FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}_Export.h" DESTINATION "include")

if(EXISTS "${INCLUDE_DIR}/template")
    install( DIRECTORY "${INCLUDE_DIR}/template" DESTINATION "include")
endif()

install( FILES "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake" DESTINATION "cmake")
install( FILES "${PROJECT_SOURCE_DIR}/cmake/Macros.cmake" DESTINATION "cmake")

include("cmake/LinkTargets.cmake")

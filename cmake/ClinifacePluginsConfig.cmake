# =============================================================================
# The ClinifacePlugins CMake configuration file.
#
# To use from an external project, in your project's CMakeLists.txt add:
#   FIND_PACKAGE( ClinifacePlugins REQUIRED)
#   LINK_DIRECTORIES( ${ClinifacePlugins_LIBRARY_DIR})
#   TARGET_LINK_LIBRARIES( MY_TARGET_NAME ${ClinifacePlugins_LIBRARIES})
#
# This module defines the following variables:
#   - ClinifacePlugins_FOUND         : True if ClinifacePlugins is found.
#   - ClinifacePlugins_ROOT_DIR      : The root directory where ClinifacePlugins is installed.
#   - ClinifacePlugins_LIBRARY_DIR   : The Cliniface plugins directory.
#   - ClinifacePlugins_LIBRARIES     : The Cliniface plugins to link to.
#
# =============================================================================

get_filename_component( ClinifacePlugins_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}"    PATH)
get_filename_component( ClinifacePlugins_ROOT_DIR  "${ClinifacePlugins_CMAKE_DIR}" PATH)

set( ClinifacePlugins_LIBRARY_DIR   "${ClinifacePlugins_ROOT_DIR}/${CMAKE_BUILD_TYPE}/plugins"  CACHE PATH "The ClinifacePlugins library directory.")

include( "${CMAKE_CURRENT_LIST_DIR}/Macros.cmake")
get_library_suffix( _lsuff)
set( _hints ClinifacePlugins${_lsuff} libClinifacePlugins${_lsuff})
find_library( ClinifacePlugins_LIBRARIES NAMES ${_hints} PATHS "${ClinifacePlugins_LIBRARY_DIR}")
set( ClinifacePlugins_LIBRARIES     ${ClinifacePlugins_LIBRARIES}         CACHE FILE "The ClinifacePlugins imported libraries to link to.")

# handle QUIETLY and REQUIRED args and set ClinifacePlugins_FOUND to TRUE if all listed variables are TRUE
include( "${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args( ClinifacePlugins "Found:\t${ClinifacePlugins_LIBRARIES}" ClinifacePlugins_LIBRARIES)

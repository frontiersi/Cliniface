set( CMAKE_COLOR_MAKEFILE TRUE)
set( CMAKE_VERBOSE_MAKEFILE FALSE)

if(UNIX)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations -Wno-error=unknown-pragmas")
endif()
set(CMAKE_CXX_STANDARD 14)

set( LIB_PRE_REQS "$ENV{INSTALL_PARENT_DIR}" CACHE PATH
    "Where library prerequisites are installed (if not in the standard system library locations).")
set( CMAKE_LIBRARY_PATH "${LIB_PRE_REQS}")
set( CMAKE_PREFIX_PATH "${LIB_PRE_REQS}")

set( BUILD_SHARED_LIBS TRUE CACHE BOOL "Dynamic Linking?" FORCE)
set( CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set( BUILD_USING_SHARED_LIBS TRUE)

# Set IS_DEBUG and _dsuffix
set( IS_DEBUG FALSE)
set( _dsuffix "")
string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug")
    set( IS_DEBUG TRUE)
    set(_dsuffix "d")
endif()
unset( CMAKE_BUILD_TYPE_LOWER)
set( CMAKE_DEBUG_POSTFIX ${_dsuffix})


macro( get_msvc_version _ver)
    set( ${_ver} "")
    if(MSVC)
        if( MSVC70 OR MSVC71 )
            set(_msvcv "7")
        elseif( MSVC80 )
            set(_msvcv "8")
        elseif( MSVC90 )
            set(_msvcv "9")
        elseif( MSVC10 )
            set(_msvcv "10")
        elseif( MSVC11 )
            set(_msvcv "11")
        elseif( MSVC12 )
            set(_msvcv "12")
        elseif( MSVC14 )
            set(_msvcv "14")
        else()
            set(_msvcv "15")
        endif()
        set( ${_ver} "${_msvcv}")
    endif(MSVC)
endmacro( get_msvc_version)


# Add definitions for Windows 7 build target (SDK 8.1 support Win 7 and up)
add_definitions( -DWINVER=0x0601)
add_definitions( -D_WIN32_WINNT=0x0601)
if(WIN32)
    set( CMAKE_SYSTEM_VERSION 8.1)
endif()
message( STATUS "OS Name build target:    ${CMAKE_SYSTEM_NAME}")
message( STATUS "OS Version build target: ${CMAKE_SYSTEM_VERSION}")


if(WITH_TESTUTILS)
    set( TestUtils_DIR "${LIB_PRE_REQS}/TestUtils/cmake" CACHE PATH "Location of TestUtilsConfig.cmake")
    find_package( TestUtils REQUIRED)
    include_directories( ${TestUtils_INCLUDE_DIRS})
    link_directories( ${TestUtils_LIBRARY_DIR})
    set(WITH_FACETOOLS TRUE)
endif()

if(WITH_FACETOOLS)
    set( FaceTools_DIR "${LIB_PRE_REQS}/FaceTools/cmake" CACHE PATH "Location of FaceToolsConfig.cmake")
    find_package( FaceTools REQUIRED)
    include_directories( ${FaceTools_INCLUDE_DIRS})
    link_directories( ${FaceTools_LIBRARY_DIR})
    set(WITH_CPD TRUE)
    set(WITH_CGAL TRUE)
    set(WITH_DLIB TRUE)
    set(WITH_QTOOLS TRUE)
    set(WITH_RMODELIO TRUE)
endif()

if(WITH_QTOOLS)
    set( QTools_DIR "${LIB_PRE_REQS}/QTools/cmake" CACHE PATH "Location of QToolsConfig.cmake")
    find_package( QTools REQUIRED)
    include_directories( ${QTools_INCLUDE_DIRS})
    link_directories( ${QTools_LIBRARY_DIR})
    set(WITH_QT TRUE)
    set(WITH_RVTK TRUE)
endif()

if(WITH_RVTK)
    set( rVTK_DIR "${LIB_PRE_REQS}/rVTK/cmake" CACHE PATH "Location of rVTKConfig.cmake")
    find_package( rVTK REQUIRED)
    include_directories( ${rVTK_INCLUDE_DIRS})
    link_directories( ${rVTK_LIBRARY_DIR})
    set(WITH_VTK TRUE)
    set(WITH_RFEATURES TRUE)
endif()

if(WITH_RPASCALVOC)
    set( rPascalVOC_DIR "${LIB_PRE_REQS}/rPascalVOC/cmake" CACHE PATH "Location of rPascalVOCConfig.cmake")
    find_package( rPascalVOC REQUIRED)
    include_directories( ${rPascalVOC_INCLUDE_DIRS})
    link_directories( ${rPascalVOC_LIBRARY_DIR})
    set(WITH_TINYXML TRUE)
    set(WITH_RLEARNING TRUE)
endif()

if(WITH_RLEARNING)
    set( rLearning_DIR "${LIB_PRE_REQS}/rLearning/cmake" CACHE PATH "Location of rLearningConfig.cmake")
    find_package( rLearning REQUIRED)
    include_directories( ${rLearning_INCLUDE_DIRS})
    link_directories( ${rLearning_LIBRARY_DIR})
    set(WITH_RFEATURES TRUE)
endif()

if(WITH_RMODELIO)
    set( rModelIO_DIR "${LIB_PRE_REQS}/rModelIO/cmake" CACHE PATH "Location of rModelIOConfig.cmake")
    find_package( rModelIO REQUIRED)
    include_directories( ${rModelIO_INCLUDE_DIRS})
    link_directories( ${rModelIO_LIBRARY_DIR})
    #set(WITH_VCG TRUE)  
    set(WITH_ASSIMP TRUE)
    set(WITH_LIBLAS FALSE)
    set(WITH_RFEATURES TRUE)
endif()

# RModelIO::U3DExporter requires IDTFConverter
if(WITH_IDTF_CONVERTER)
    set( idtfconv_exe "$ENV{IDTF_CONVERTER_EXE}")   # Set location of IDTFConverter from env var if set
    if( "${idtfconv_exe}" STREQUAL "")              # If not set, try to guess based on platform
        set( idtfconv_exe "${LIB_PRE_REQS}/u3dIntel/bin/IDTFConverter")
    endif()

    if ( NOT EXISTS ${idtfconv_exe})
        set( idtfconv_exe "rModelIO_IDTF_CONVERTER-NOTFOUND")
    endif()

    set( rModelIO_IDTF_CONVERTER ${idtfconv_exe} CACHE FILEPATH "Location of the IDTFConverter executable.")
    if ( EXISTS ${rModelIO_IDTF_CONVERTER})
        message( STATUS "[rModelIO] RModelIO::U3DExporter using IDTFConverter at ${rModelIO_IDTF_CONVERTER}")
        add_definitions( -DIDTF_CONVERTER=\"${rModelIO_IDTF_CONVERTER}\")
    else()
        message( STATUS "[rModelIO] IDTFConverter not found; RModelIO::U3DExporter disabled.")
    endif()
endif()

if(WITH_RFEATURES)
    set( rFeatures_DIR "${LIB_PRE_REQS}/rFeatures/cmake" CACHE PATH "Location of rFeaturesConfig.cmake")
    find_package( rFeatures REQUIRED)
    include_directories( ${rFeatures_INCLUDE_DIRS})
    link_directories( ${rFeatures_LIBRARY_DIR})
    set(WITH_OPENCV TRUE)
    set(WITH_RLIB TRUE)
endif()

if(WITH_RLIB)
    set( rlib_DIR "${LIB_PRE_REQS}/rlib/cmake" CACHE PATH "Location of rlibConfig.cmake")
    find_package( rlib REQUIRED)
    include_directories( ${rlib_INCLUDE_DIRS})
    link_directories( ${rlib_LIBRARY_DIR})
    set(WITH_BOOST TRUE)
    set(WITH_EIGEN TRUE)
endif()



if(WITH_ASSIMP)     # AssImp
    set( ASSIMP_DIR "${LIB_PRE_REQS}/AssImp/lib/cmake/assimp-3.3" CACHE PATH "Location of assimp-config.cmake")
    find_package( ASSIMP REQUIRED)
    include_directories( ${ASSIMP_INCLUDE_DIRS})
    link_directories( ${ASSIMP_LIBRARY_DIRS})
endif()

#if(WITH_LIBLAS)     # libLAS
#    set( LibLAS_DIR "${LIB_PRE_REQS}/libLAS-1.7.0" CACHE PATH "Location of LibLASConfig.cmake")
#    find_package( LibLAS REQUIRED)
#    include_directories( ${LibLAS_INCLUDE_DIR})
#endif()

if(WITH_TINYXML)    # tinyxml
    set( tinyxml_DIR "${LIB_PRE_REQS}/tinyxml/cmake" CACHE PATH "Location of tinyxmlConfig.cmake")
    find_package( tinyxml REQUIRED)
    include_directories( ${tinyxml_INCLUDE_DIR})
    link_directories( ${tinyxml_LIBRARY_DIR})
endif()


if(WITH_BOOST)  # Boost
    set( BOOST_ROOT "${LIB_PRE_REQS}/boost_1_64_0" CACHE PATH "Location of boost")
    set( BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

    if(WIN32)
        get_msvc_version( _msvcv)
        set( BOOST_LIBRARYDIR "${BOOST_ROOT}/lib64-msvc-${_msvcv}.0")
        add_definitions( ${Boost_LIB_DIAGNOSTIC_DEFINITIONS})   # Info about Boost's auto linking
    endif()

    set( Boost_NO_SYSTEM_PATHS TRUE)  # Don't search for Boost in system directories
    set( Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static Boost libraries")
    set( Boost_USE_MULTITHREADED ON)
    set( Boost_USE_STATIC_RUNTIME OFF)
    add_definitions( -DBOOST_ALL_NO_LIB)    # Disable autolinking
    add_definitions( -DBOOST_ALL_DYN_LINK)  # Force dynamic linking (probably don't need this)

    find_package( Boost 1.64 REQUIRED COMPONENTS system filesystem thread random regex)
    include_directories( ${Boost_INCLUDE_DIRS})

    #message( STATUS "Boost_VERSION: ${Boost_VERSION}")
    #message( STATUS "Boost_LIB_VERSION: ${Boost_LIB_VERSION}")
    #message( STATUS "Boost_MAJOR_VERSION: ${Boost_MAJOR_VERSION}")
    #message( STATUS "Boost_MINOR_VERSION: ${Boost_MINOR_VERSION}")
    #message( STATUS "Boost_SUBMINOR_VERSION: ${Boost_SUBMINOR_VERSION}")
endif()


if(WITH_EIGEN) # Eigen3
    if(WIN32)
        set( EIGEN3_INCLUDE_DIR "${LIB_PRE_REQS}/eigen3/include/eigen3" CACHE PATH "Location of Eigen3 headers (Eigen/*.h) directory")
    elseif(UNIX)
        find_package( Eigen3 REQUIRED)
    endif()
    include_directories( ${EIGEN3_INCLUDE_DIR})
endif()

if(WITH_OPENCV) # OpenCV
    if(WIN32)
        if(IS_DEBUG)
            set( OpenCV_DIR "${LIB_PRE_REQS}/opencv/debug" CACHE PATH "Location of OpenCVConfig.cmake")
        else()
            set( OpenCV_DIR "${LIB_PRE_REQS}/opencv/release" CACHE PATH "Location of OpenCVConfig.cmake")
        endif()
        get_msvc_version( _msvcv)
        set( OpenCV_BIN "${OpenCV_DIR}/x64/vc${_msvcv}/bin" CACHE PATH "Location of OpenCV binary (dll) files")
    endif()
    find_package( OpenCV 2.4 REQUIRED)
    include_directories( ${OpenCV_INCLUDE_DIRS})
endif()

if(WITH_VTK)    # VTK
    set( VTK_VER 8.1)
    if(IS_DEBUG)
        set( VTK_DIR "${LIB_PRE_REQS}/VTK/debug/lib/cmake/vtk-${VTK_VER}" CACHE PATH "Location of VTKConfig.cmake")
    else()
        set( VTK_DIR "${LIB_PRE_REQS}/VTK/release/lib/cmake/vtk-${VTK_VER}" CACHE PATH "Location of VTKConfig.cmake")
    endif()
    if(NOT IS_DIRECTORY ${VTK_DIR})
        message( STATUS "VTK_DIR=${VTK_DIR}")
        message( FATAL_ERROR "Can't find VTK! Set VTK_VER correctly for the version of VTK you want to build against!")
    endif()
    set( VTK_BIN "${VTK_DIR}/../../../bin" CACHE PATH "Location of VTK binary (dll) files")
    find_package( VTK REQUIRED)
    include( ${VTK_USE_FILE})
endif()

if(WITH_QT)     # Qt5
    set( Qt5_DIR "$ENV{QT5_CMAKE_PATH}" CACHE PATH "Location of Qt5Config.cmake")
    if(NOT IS_DIRECTORY ${Qt5_DIR})
        message( STATUS "QT5_CMAKE_PATH=$ENV{QT5_CMAKE_PATH}")
        message( FATAL_ERROR "Can't find Qt5! Set environment variable QT5_CMAKE_PATH to the location of Qt5Config.cmake")
    endif()
    find_package( Qt5 REQUIRED Widgets Sql)
    include_directories( ${Qt5Widgets_INCLUDE_DIRS})
    add_definitions( ${Qt5Widgets_DEFINITIONS})
    add_definitions( ${Qt5Sql_DEFINITIONS})
    set( QT_LIBRARIES Qt5::Widgets Qt5::Sql)
    message( STATUS "Using Qt5 at ${Qt5_DIR}")
endif()

if(WITH_CGAL)   # CGAL
    if(IS_DEBUG)
        set( CGAL_DIR "${LIB_PRE_REQS}/CGAL/debug/lib/CGAL" CACHE PATH "Location of CGALConfig.cmake")
    else()
        set( CGAL_DIR "${LIB_PRE_REQS}/CGAL/release/lib/CGAL" CACHE PATH "Location of CGALConfig.cmake")
    endif()
    set( CGAL_BIN "${CGAL_DIR}/../../bin" CACHE PATH "Location of CGAL binary (dll) files")
    find_package( CGAL COMPONENTS Core)
    set( CGAL_DONT_OVERRIDE_CMAKE_FLAGS TRUE CACHE BOOL "Prevent CGAL from overwritting CMake flags.")
    include( ${CGAL_USE_FILE})
endif()

if(WITH_DLIB)   # dlib
    set( dlib_ROOT "${LIB_PRE_REQS}/dlib" CACHE PATH "Location of dlib")
    if(IS_DEBUG)
        set( dlib_DIR "${dlib_ROOT}/debug/lib/cmake/dlib" CACHE PATH "Location of dlibConfig.cmake")
    else()
        set( dlib_DIR "${dlib_ROOT}/release/lib/cmake/dlib" CACHE PATH "Location of dlibConfig.cmake")
    endif()
    find_package( dlib REQUIRED)
    include_directories( ${dlib_INCLUDE_DIRS})
endif()

#if(WITH_VCG)    # VCGLib (Visual and Computer Graphics Library)
#    set( VCG_DIR "${LIB_PRE_REQS}/vcglib" CACHE PATH "Location of VCG header only library.")
#    include_directories( ${VCG_DIR})    # Header only library
#endif()

if(WITH_LIBICP) # The ICP source library (Andreas Geiger) (https://github.com/symao/libicp)
    set( LIBICP_DIR "${LIB_PRE_REQS}/libICP" CACHE PATH "Location of libICP source files.")
    set( LIBICP_SRC "${LIBICP_DIR}/src")
    include_directories( "${LIBICP_SRC}")
    set( LIBICP_SRC_FILES
        "${LIBICP_SRC}/icp.cpp"
        "${LIBICP_SRC}/icpPointToPlane.cpp"
        "${LIBICP_SRC}/icpPointToPoint.cpp"
        "${LIBICP_SRC}/kdtree.cpp"
        "${LIBICP_SRC}/matrix.cpp"
        )
endif()

if(WITH_CPD) # Coherent Point Drift
    set( Cpd_ROOT "${LIB_PRE_REQS}/cpd" CACHE PATH "Location of CPD (Coherent Point Drift)")
    if(IS_DEBUG)
        set( Cpd_DIR "${Cpd_ROOT}/debug/lib/cmake/cpd" CACHE PATH "Location of cpd-config.cmake")
    else()
        set( Cpd_DIR "${Cpd_ROOT}/release/lib/cmake/cpd" CACHE PATH "Location of cpd-config.cmake")
    endif()
    message( STATUS "Cpd_DIR: ${Cpd_DIR}")
    find_package( Cpd REQUIRED)
    set( Cpd_INCLUDE_DIRS "${Cpd_DIR}/../../../include")
    include_directories( ${Cpd_INCLUDE_DIRS})
endif()


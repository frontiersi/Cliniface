# Sets the correct library suffix for the linker given the platform and whether we want a debug build or not.
# User should set CMAKE_BUILD_TYPE [debug/...] and BUILD_SHARED_LIBS [true/false] (or BUILD_USING_SHARED_LIBS) before calling.
# lsuffix set to one of [platform;build type]:
# .so   [UNIX;SHARED]          d.so  [UNIX;DEBUG,SHARED]
# .a    [UNIX;STATIC]          d.a   [UNIX;DEBUG,STATIC]
# .lib  [WIN32;STATIC/SHARED]  d.lib  [WIN32;DEBUG,STATIC/SHARED]


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


macro( get_debug_suffix dsuff)
    set( ${dsuff} "")
    string( TOLOWER "${CMAKE_BUILD_TYPE}" _build_type)
    if( _build_type MATCHES "debug")
        set( ${dsuff} "d")
    endif()
endmacro( get_debug_suffix)


macro( get_shared_suffix lsuffix)
    get_debug_suffix( _dsuffix)
    if(UNIX)
        set( ${lsuffix} "${_dsuffix}.so")
    elseif(WIN32)
        get_msvc_version( _msvcver)
        set( ${lsuffix} "${_dsuffix}-vc${_msvcver}0.dll")
    endif()
endmacro( get_shared_suffix)


macro( get_static_suffix lsuffix)
    get_debug_suffix( _dsuffix)
    if(UNIX)
        set( ${lsuffix} "${_dsuffix}.a")
    elseif(WIN32)
        get_msvc_version( _msvcver)
        set( ${lsuffix} "${_dsuffix}-vc${_msvcver}0.lib")
    endif()
endmacro( get_static_suffix)


# For looking for libraries
macro( get_library_suffix lsuffix)
    if(UNIX)
        if(BUILD_USING_SHARED_LIBS)
            get_shared_suffix( _lsuffix)
        else()
            get_static_suffix( _lsuffix)
        endif()
    elseif(WIN32)
        get_static_suffix( _lsuffix)
    else()
        message( FATAL_ERROR "Platform not supported!")
    endif()
    set( ${lsuffix} ${_lsuffix})
endmacro( get_library_suffix)


macro( copy_over_dll libName)
    get_shared_suffix( sosuff)
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD      # post build event
        COMMAND ${CMAKE_COMMAND} -E copy_if_different          # execs "cmake -R copy_if_different ..."
        "$ENV{INSTALL_PARENT_DIR}/${libName}/bin/${libName}${sosuff}"         # dll to copy
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                    # out path
endmacro( copy_over_dll)

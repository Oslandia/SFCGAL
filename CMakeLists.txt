cmake_minimum_required( VERSION 2.8 )
project( SFCGAL )

set( CMAKE_DEBUG_POSTFIX "d" )

#----------------------------------------------------------------------------
# build options
#----------------------------------------------------------------------------
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

option( SFCGAL_BUILD_EXAMPLES "build examples" OFF )
option( SFCGAL_BUILD_TESTS "build unit, garden and regress tests" OFF )
option( SFCGAL_BUILD_BENCH "Build benchmarks" OFF )

option( SFCGAL_WITH_OSG "Compile with OpenSceneGraph support" OFF )

#-- include finders and co
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}" )

set( SFCGAL_VERSION_MAJOR 1 )
set( SFCGAL_VERSION_MINOR 3 )
set( SFCGAL_VERSION_PATCH 7 )

set( SFCGAL_VERSION "${SFCGAL_VERSION_MAJOR}.${SFCGAL_VERSION_MINOR}.${SFCGAL_VERSION_PATCH}" )

# disabled for 1.0
#include( PrecompiledHeader )
#option( Use_precompiled_headers "Use precompiled headers" OFF )

include( Libtoolize )

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
    set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

#-----------------------------------------------------------
# dependencies
#-----------------------------------------------------------

#-- find CGAL  ---------------------------------------------
option( CGAL_USE_AUTOLINK "disable CGAL autolink" OFF )
if( ${CGAL_USE_AUTOLINK} )
	add_definitions( "-DCGAL_NO_AUTOLINK" )
endif()

# 4.3 minimal
# 4.13 recommended
find_package( CGAL COMPONENTS Core REQUIRED )
message( STATUS "CGAL ${CGAL_VERSION} found" )

include_directories( ${CMAKE_BINARY_DIR}/include )

# For CGAL versions < 4.3, we add a local directory that contains some tweaked include files from unreleased versions
# They will overwrite files from the CGAL installation
if( "${CGAL_VERSION}" VERSION_LESS "4.3" )
  include_directories( patches/CGAL-4.2 )
elseif( "${CGAL_VERSION}" VERSION_LESS "4.10")
  include_directories( patches/CGAL-4.3 )
  add_definitions( "-DCGAL_INTERSECTION_VERSION=1" )
endif()

#-- BOOST --------------------------------------------------
option( Boost_USE_AUTO_LINK "boost use autolink" OFF )
if( NOT ${Boost_USE_AUTO_LINK} )
	add_definitions( "-DBOOST_ALL_NO_LIB" )
endif()

option( Boost_USE_STATIC_LIBS "boost use dynamic libraries" OFF )
if( Boost_USE_STATIC_LIBS )
	message( STATUS "Boost_USE_STATIC_LIBS=ON" )
	add_definitions( "-DBOOST_THREAD_USE_LIB" )
else()
	message( STATUS "Boost_USE_STATIC_LIBS=OFF" )
#	add_definitions( "-DBOOST_TEST_DYN_LINK" )
	add_definitions( "-DBOOST_ALL_DYN_LINK" )
endif()

option( Boost_USE_MULTITHREAD "boost use multithread libraries" ON )
if( ${Boost_USE_MULTITHREAD} )
	message( STATUS "Boost_USE_MULTITHREAD=ON" )
else()
	message( STATUS "Boost_USE_MULTITHREAD=OFF" )
endif()

#-- minimalist build allowed with boost version older than 1.48
set( SFCGAL_Boost_COMPONENTS thread system serialization )
#-- unit test
if ( SFCGAL_BUILD_TESTS )
	set( SFCGAL_Boost_COMPONENTS unit_test_framework program_options ${SFCGAL_Boost_COMPONENTS} )
endif()
#-- program_options
if ( SFCGAL_BUILD_TESTS OR SFCGAL_BUILD_EXAMPLES OR SFCGAL_BUILD_OSG )
	set( SFCGAL_Boost_COMPONENTS program_options chrono filesystem timer ${SFCGAL_Boost_COMPONENTS} )
endif()
find_package( Boost COMPONENTS ${SFCGAL_Boost_COMPONENTS} REQUIRED )
if((${Boost_MAJOR_VERSION} EQUAL 1) AND (${Boost_MINOR_VERSION} EQUAL 58))
	message( STATUS "Defining BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT" )
	add_definitions( "-DBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT" )
endif()
# patch boost 1.60.0      
if((${Boost_MAJOR_VERSION} EQUAL 1) AND (${Boost_MINOR_VERSION} EQUAL 60) AND (${Boost_SUBMINOR_VERSION} EQUAL 0))
  include_directories( patches/boost-1.60.0 )
endif()

if( SFCGAL_USE_STATIC_LIBS )
  add_definitions( "-DSFCGAL_USE_STATIC_LIBS" )
  if (NOT MSVC)
    add_definitions( "-fPIC" )
  endif()
endif()

#-- OpenScenegraph -----------------------------------------
if ( SFCGAL_WITH_OSG )
  find_package( OpenSceneGraph COMPONENTS osgDB osgUtil )
  if( ${OPENSCENEGRAPH_FOUND} )
    message( STATUS "OPENSCENEGRAPH_INCLUDE_DIRS = ${OPENSCENEGRAPH_INCLUDE_DIRS}" )
    message( STATUS "OPENSCENEGRAPH_LIBRARIES = ${OPENSCENEGRAPH_LIBRARIES}" )

    include_directories( SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS} )
  endif()
endif()

#-- note that SYSTEM turns -I/path to -isystem and avoid warnings in CGAL and Boost
include_directories( SYSTEM 
	${CGAL_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}	
)
link_directories(
	${CGAL_LIBRARY_DIRS}
	${Boost_LIBRARY_DIRS}	
)

#-- Warnings, frounding-math and gprof  ------------------------------------------
option( SFCGAL_WARNING_AS_ERROR "fail the build on warnings" OFF )
option( SFCGAL_BUILD_WITH_GPROF "build with gprof" OFF )
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic -Wpointer-arith -Wcast-align -Wcast-qual -Wno-overloaded-virtual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wwrite-strings -Wno-error=undef")#-Wfloat-equal -Wconversion -Wshadow 
    if( SFCGAL_WARNING_AS_ERROR )
		 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-error=format")
	endif()

	# Allows profiling with gprof
	if(SFCGAL_BUILD_WITH_GPROF)
		set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
	endif()
endif()


if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(_LIBDIR_DEFAULT "lib")
  # Override this default 'lib' with 'lib64' iff:
  #  - we are on Linux system but NOT cross-compiling
  #  - we are NOT on debian
  #  - we are on a 64 bits system
  # reason is: amd64 ABI: http://www.x86-64.org/documentation/abi.pdf
  # Note that the future of multi-arch handling may be even
  # more complicated than that: http://wiki.debian.org/Multiarch
  if(CMAKE_SYSTEM_NAME MATCHES "Linux"
      AND NOT CMAKE_CROSSCOMPILING
      AND NOT EXISTS "/etc/debian_version")
    if(NOT DEFINED CMAKE_SIZEOF_VOID_P)
      message(AUTHOR_WARNING
        "Unable to determine default CMAKE_INSTALL_LIBDIR directory because no target architecture is known. "
        "Please enable at least one language before including GNUInstallDirs.")
    else()
      if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(_LIBDIR_DEFAULT "lib64")
      endif()
    endif()
  endif()
  set(CMAKE_INSTALL_LIBDIR "${_LIBDIR_DEFAULT}" CACHE PATH "object code libraries (${_LIBDIR_DEFAULT})")
endif()

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#SET(CMAKE_MACOSX_RPATH ON)

#-- generate library headers
configure_file( ${CMAKE_SOURCE_DIR}/src/config.h.cmake include/SFCGAL/config.h )
configure_file( ${CMAKE_SOURCE_DIR}/src/version.h.cmake include/SFCGAL/version.h )

enable_testing()

#note : not available on windows without export/import
OPTION( SFCGAL_USE_STATIC_LIBS "define if SFCGAL is build as a static library" OFF )

#-- build the library
add_subdirectory( src )

#-- build test (todo only if boost use dyn link)
add_subdirectory( test )

#-- build examples
if( SFCGAL_BUILD_EXAMPLES )
	add_subdirectory( example )
endif()

#-- doxygen documentation (allows make doc when doxygen is found)
add_subdirectory( doc )

#-- install directories
install(DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION .)


#-- create a libtool file for SFCGAL (needed by PostGIS)
create_libtool_file( SFCGAL )

#-- sfcgal-config
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
  set( SFCGAL_LIB_NAME "SFCGAL${CMAKE_DEBUG_POSTFIX}" )
else()
  set( SFCGAL_LIB_NAME "SFCGAL" )
endif()
#set( SFCGAL_LIB_NAME ${${CMAKE_BUILD_TYPE}
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sfcgal-config.in ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config @ONLY)
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config DESTINATION bin )

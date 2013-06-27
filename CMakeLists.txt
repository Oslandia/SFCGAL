cmake_minimum_required( VERSION 2.8 )
project( SFCGAL )

set( CMAKE_DEBUG_POSTFIX "d" )

#----------------------------------------------------------------------------
# build options
#----------------------------------------------------------------------------
option( SFCGAL_BUILD_EXAMPLES "build examples" OFF )
option( SFCGAL_BUILD_OSG "build SFCGAL with OpenSceneGraph support (outputs in 3D formats)" OFF )
option( SFCGAL_BUILD_UNIT_TESTS "build unit tests" OFF )
option( SFCGAL_BUILD_REGRESS_TESTS "build regress tests" OFF )
option( SFCGAL_BUILD_BENCH "Build benchmarks" OFF )
option( SFCGAL_BUILD_TOOLS "build tools" OFF )
option( SFCGAL_BUILD_VIEWER "build viewer" OFF )

#-- include finders and co
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}" )

set( SFCGAL_VERSION_MAJOR 0 )
set( SFCGAL_VERSION_MINOR 2 )
set( SFCGAL_VERSION_PATCH 0-master )

set( SFCGAL_VERSION "${SFCGAL_VERSION_MAJOR}.${SFCGAL_VERSION_MINOR}.${SFCGAL_VERSION_PATCH}" )

include( PrecompiledHeader )
option( Use_precompiled_headers "Use precompiled headers" OFF )

include( Libtoolize )

#-----------------------------------------------------------
# dependencies
#-----------------------------------------------------------

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
set( SFCGAL_Boost_COMPONENTS date_time thread system serialization )
#-- unit test
if ( SFCGAL_BUILD_UNIT_TESTS OR SFCGAL_BUILD_REGRESS_TESTS )
	set( SFCGAL_Boost_COMPONENTS unit_test_framework ${SFCGAL_Boost_COMPONENTS} )
endif()
#-- program_options
if ( SFCGAL_BUILD_REGRESS_TESTS OR SFCGAL_BUILD_EXAMPLES OR SFCGAL_BUILD_OSG OR SFCGAL_BUILD_TOOLS OR SFCGAL_BUILD_VIEWER )
	set( SFCGAL_Boost_COMPONENTS program_options chrono filesystem timer ${SFCGAL_Boost_COMPONENTS} )
endif()
find_package( Boost COMPONENTS ${SFCGAL_Boost_COMPONENTS} REQUIRED )


#-- GMP (facultative)  -------------------------------------
find_package( GMP )
if( ${GMP_FOUND} )
	message( STATUS "GMP_INCLUDE_DIRS = ${GMP_INCLUDE_DIRS}" )
	message( STATUS "GMP_LIBRARIES = ${GMP_LIBRARIES}" )
	set( SFCGAL_WITH_GMP ON )
endif()



#-- MPFR (needs GMP)  --------------------------------------
if( ${GMP_FOUND} )
	find_package( MPFR )
	if( ${MPFR_FOUND} )
		message( STATUS "MPFR_INCLUDE_DIRS = ${MPFR_INCLUDE_DIRS}" )
		message( STATUS "MPFR_LIBRARIES = ${MPFR_LIBRARIES}" )
		set( SFCGAL_WITH_MPFR ON )
	endif()
endif()

if( NOT ${GMP_FOUND} OR NOT ${MPFR_FOUND} )
	message( WARNING "can't find GMP and MPFR, using default kernel" ) 
	set( EPLU_USE_CARTESIAN_KERNEL ON )
endif()


#-- find CGAL  ---------------------------------------------
option( CGAL_USE_AUTOLINK "disable CGAL autolink" OFF )
if( ${CGAL_USE_AUTOLINK} )
	add_definitions( "-DCGAL_NO_AUTOLINK" )
endif()

find_package( CGAL COMPONENTS Core REQUIRED )
message( STATUS "CGAL ${CGAL_VERSION} found" )

include_directories( 
	include
	${CGAL_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}	
	${MPFR_INCLUDE_DIRS}	
	${GMP_INCLUDE_DIRS}	
)
link_directories(
	${CGAL_LIBRARY_DIRS}
	${Boost_LIBRARY_DIRS}	
	${MPFR_LIBRARY_DIRS}	
	${GMP_LIBRARY_DIRS}	
)

#-- optional OpenSceneGraph support
if ( SFCGAL_BUILD_OSG )
	find_package( OpenSceneGraph COMPONENTS osgDB osgUtil REQUIRED )
	include_directories( ${OPENSCENEGRAPH_INCLUDE_DIRS} ) 
	
	# config.h generation
	set( SFCGAL_WITH_OSG "ON" )
endif()



#-- Warnings, frounding-math and gprof  ------------------------------------------
option( SFCGAL_BUILD_FAILED_ON_WARNING "fail the build on warnings" OFF )
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
	if( SFCGAL_BUILD_FAILED_ON_WARNING )
		 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-error=format")
	endif()

	add_definitions( "-frounding-math" )
	
	# Allows profiling with gprof
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
endif()

#-- generate library headers
configure_file( ${CMAKE_SOURCE_DIR}/include/SFCGAL/config.h.cmake ${CMAKE_SOURCE_DIR}/include/SFCGAL/config.h )
configure_file( ${CMAKE_SOURCE_DIR}/include/SFCGAL/version.h.cmake ${CMAKE_SOURCE_DIR}/include/SFCGAL/version.h )

enable_testing()

#note : not available on windows without export/import
OPTION( SFCGAL_USE_STATIC_LIBS "define if SFCGAL is build as a static library" OFF )

# option : do we check geometry validity before each processing ?
option( SFCGAL_CHECK_VALIDITY "Always checks geometry validity" ON )
if ( NOT SFCGAL_CHECK_VALIDITY )
  add_definitions( "-DSFCGAL_NEVER_CHECK_VALIDITY" )
endif()

#-- build the library
add_subdirectory( src )

#-- build test (todo only if boost use dyn link)
add_subdirectory( test )

#-- build examples
if( SFCGAL_BUILD_EXAMPLES )
	add_subdirectory( example )
endif()

#-- build tools
if( SFCGAL_BUILD_TOOLS )
	add_subdirectory( tools )
endif()

#-- viewer 
if( SFCGAL_BUILD_VIEWER )
	if ( NOT SFCGAL_BUILD_OSG )
		message( FATAL_ERROR "can't build viewer without OSG support (set SFCGAL_BUILD_OSG to ON)" )
	endif()
	add_subdirectory( viewer )
endif()

#-- doxygen documentation (allows make doc when doxygen is found)
add_subdirectory( doc )

#-- install directories
install(DIRECTORY ./include DESTINATION .)


#-- create a libtool file for SFCGAL (needed by PostGIS)
create_libtool_file( SFCGAL /lib )

#-- sfcgal-config
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
  set( SFCGAL_LIB_NAME "SFCGAL${CMAKE_DEBUG_POSTFIX}" )
else()
  set( SFCGAL_LIB_NAME "SFCGAL" )
endif()
#set( SFCGAL_LIB_NAME ${${CMAKE_BUILD_TYPE}
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sfcgal-config.in ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config @ONLY)
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config DESTINATION bin )

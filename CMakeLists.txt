cmake_minimum_required( VERSION 2.8 )
project( SFCGAL )

set( CMAKE_DEBUG_POSTFIX "d" )

#-- include finders and co
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}" )

set( SFCGAL_VERSION_MAJOR 0 )
set( SFCGAL_VERSION_MINOR 2 )
set( SFCGAL_VERSION_PATCH 0-master )

set( SFCGAL_VERSION "${SFCGAL_VERSION_MAJOR}.${SFCGAL_VERSION_MINOR}.${SFCGAL_VERSION_PATCH}" )

include( PrecompiledHeader )
option( Use_precompiled_headers "Use precompiled headers" OFF )

#-----------------------------------------------------------
# dependencies
#-----------------------------------------------------------

#-- BOOST --------------------------------------------------
option( Boost_USE_AUTO_LINK "boost use autolink" OFF )
if( NOT ${Boost_USE_AUTO_LINK} )
	add_definitions( "-DBOOST_ALL_NO_LIB" )
endif()

option( Boost_USE_STATIC_LIBS "boost use dynamic libraries" OFF )
if( ${Boost_USE_STATIC_LIBS} )
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
find_package( Boost COMPONENTS chrono program_options unit_test_framework filesystem date_time system thread serialization timer REQUIRED )


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
option( SFCGAL_BUILD_OSG "build SFCGAL with OpenSceneGraph support (outputs in 3D formats)" OFF )
if ( SFCGAL_BUILD_OSG )
	find_package( OpenSceneGraph COMPONENTS osgDB osgUtil REQUIRED )
	include_directories( ${OPENSCENEGRAPH_INCLUDE_DIRS} ) 
	
	# config.h generation
	set( SFCGAL_WITH_OSG "ON" )
endif()



#-- Warnings, frounding-math and gprof  ------------------------------------------
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

	add_definitions( "-frounding-math" )
	
	# Allows profiling with gprof
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
endif()

#-- generate library headers
configure_file( ${CMAKE_SOURCE_DIR}/include/SFCGAL/config.h.cmake ${CMAKE_SOURCE_DIR}/include/SFCGAL/config.h )
configure_file( ${CMAKE_SOURCE_DIR}/include/SFCGAL/version.h.cmake ${CMAKE_SOURCE_DIR}/include/SFCGAL/version.h )


#note : not available on windows without export/import
OPTION( SFCGAL_USE_STATIC_LIBS "define if SFCGAL is build as a static library" OFF )


#-- build the library
add_subdirectory( src )

#-- build test (todo only if boost use dyn link)
add_subdirectory( test )

#-- build examples
option( SFCGAL_BUILD_EXAMPLES "build examples" OFF )
if( ${SFCGAL_BUILD_EXAMPLES} )
	add_subdirectory( example )
endif()

#-- build tools
option( SFCGAL_BUILD_TOOLS "build tools" ON )
if( ${SFCGAL_BUILD_TOOLS} )
	add_subdirectory( tools )
endif()

#-- viewer 
option( SFCGAL_BUILD_VIEWER "build viewer" OFF )
if( ${SFCGAL_BUILD_VIEWER} )
	if ( NOT SFCGAL_BUILD_OSG )
		message( FATAL_ERROR "can't build viewer without OSG support (set SFCGAL_BUILD_OSG to ON)" )
	endif()
	add_subdirectory( viewer )
endif()

#-- doxygen documentation (allows make doc when doxygen is found)
add_subdirectory( doc )

#-- install directories
install(DIRECTORY ./include DESTINATION .)


#-- sfcgal-config
if ( ${CMAKE_BUILD_TYPE} STREQUAL Debug )
  set( SFCGAL_LIB_NAME "SFCGAL${CMAKE_DEBUG_POSTFIX}" )
else()
  set( SFCGAL_LIB_NAME "SFCGAL" )
endif()
#set( SFCGAL_LIB_NAME ${${CMAKE_BUILD_TYPE}
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sfcgal-config.in ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config @ONLY)
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/sfcgal-config DESTINATION bin )
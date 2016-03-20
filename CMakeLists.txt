project(SFCGAL2)
cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------
# enable C++11
#--------------------------------------------------
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

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

#--------------------------------------------------
# define library version
#--------------------------------------------------
set( SFCGAL_VERSION_MAJOR 2 )
set( SFCGAL_VERSION_MINOR 0 )
set( SFCGAL_VERSION_PATCH 0 )
set( SFCGAL_VERSION "${SFCGAL_VERSION_MAJOR}.${SFCGAL_VERSION_MINOR}.${SFCGAL_VERSION_PATCH}" )

#------------------------------------------------------------
# Override the default lib dir to handle lib/lib64 switch
#------------------------------------------------------------
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

#--------------------------------------------------
# find dependencies
#--------------------------------------------------

# include finders...
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake/Modules")

# find boost...
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
	add_definitions( "-DBOOST_ALL_DYN_LINK" )
endif()

option( Boost_USE_MULTITHREAD "boost use multithread libraries" ON )
if( ${Boost_USE_MULTITHREAD} )
	message( STATUS "Boost_USE_MULTITHREAD=ON" )
else()
	message( STATUS "Boost_USE_MULTITHREAD=OFF" )
endif()

#-- minimalist build allowed with boost version older than 1.48
set( SFCGAL_Boost_COMPONENTS log date_time thread system serialization )
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

#-- find CGAL and it's dependencies
find_package( CGAL 4.8 COMPONENTS Core REQUIRED )
message( STATUS "CGAL ${CGAL_VERSION} found" )

find_package(MPFR REQUIRED)
find_package(GMP REQUIRED)

#--------------------------------------------------
#-- generate library headers
#--------------------------------------------------
configure_file( ${CMAKE_SOURCE_DIR}/src/SFCGAL/config.h.cmake ${CMAKE_SOURCE_DIR}/src/SFCGAL/config.h )
configure_file( ${CMAKE_SOURCE_DIR}/src/SFCGAL/version.h.cmake ${CMAKE_SOURCE_DIR}/src/SFCGAL/version.h )

#--------------------------------------------------
#-- enable testing system (usefull if test are disabled?)
#--------------------------------------------------
enable_testing()

#--------------------------------------------------
#-- set include directories
#--------------------------------------------------
include_directories(
    src                       # main headers
    ${CMAKE_BINARY_DIR}/src   # generated headers
)

add_subdirectory(src)

if(SFCGAL_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(SFCGAL_BUILD_TESTS)
    add_subdirectory(test)
endif()

#add_directory(bench)




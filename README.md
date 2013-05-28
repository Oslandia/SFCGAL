SFCGAL
======

SFCGAL is a C++ wrapper library around [CGAL](http://www.cgal.org) with the aim of supporting ISO 19107 and OGC Simple Features for 3D operations.

It depends on CGAL 4.1

Requirements
-----------

* C++ compiler
* CMake 2.8.6+
* CGAL 4.1
* Boost (see CGAL for version)
* MPFR (CGAL Epeck)
* GMP (CGAL Epeck)

Optional dependencies (viewer and 3D format export)

* OpenSceneGraph (version >= 3.1)
* Qt4


Compilation
-----------

* Use cmake or cmake-gui

Build options (ON/OFF cmake variable)

<pre>
SFCGAL_BUILD_UNIT_TESTS : Build unit tests
SFCGAL_BUILD_REGRESS_TESTS : Build regress tests
SFCGAL_BUILD_BENCH : Build bench tests
SFCGAL_BUILD_EXAMPLES : Build examples
SFCGAL_BUILD_OSG : Build components depending on OpenSceneGraph
SFCGAL_BUILD_VIEWER : Build basic viewer
</pre>

Dependencies location (environment variables)

<pre>
GMP_DIR : GMP location
MPFR_DIR : MPFR location
CGAL_DIR : CGAL location
BOOST_ROOT : Boost location
</pre>

* make / make install

CGAL uses a lot of templated constructions. The whole building process may then take a while.


Documentation
-------------

If doxygen is available, you may run 'make doc' to generate the code documentation.


Test & usage
------------

In addition to the main SFCGAL library, the following optional components are part of the building process:
* PostGIS (2.2-trunk) throw --with-sfcgal option in configure
* a unit test suite, see the test/unit directory
* a regression test suite, see the test/regress directory
* a benchmark suite, see the test/bench directory
* a 3D viewer, based on OpenSceneGraph and Qt



Troubleshouting
--------------------

Windows/MinGW
--

* MinGW fail to build boost thread for version 1.46.1 (fixed in boost 1.47.0, please upgrade your boost version)
* Use the same cmake option for boost with CGAL and SFCGAL (particularly Boost_USE_STATIC_LIBS=OFF)
* Missing boost definitions in CGAL-4.1 to use static libraries (dllexport/dllimport)

Patch CGAL-4.1/CMakeLists.txt with the following lines to build with boost static libraries (Boost_USE_STATIC_LIBS=ON). 

<pre>
option( Boost_USE_STATIC_LIBS "boost use dynamic libraries" OFF )
if( Boost_USE_STATIC_LIBS )
  message( STATUS "Boost_USE_STATIC_LIBS=ON" )
	add_definitions( "-DBOOST_THREAD_USE_LIB" )
else()
	message( STATUS "Boost_USE_STATIC_LIBS=OFF" )
	add_definitions( "-DBOOST_ALL_DYN_LINK" )
endif()
</pre>






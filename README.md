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
* Qt


Compilation
-----------

* Use cmake or cmake-gui
* make / make install

CGAL uses a lot of templated constructions. The whole building process may then take a while.


* Build options (ON/OFF cmake variable)


SFCGAL_BUILD_UNIT_TESTS : Build unit tests

SFCGAL_BUILD_REGRESS_TESTS : Build regress tests

SFCGAL_BUILD_BENCH : Build bench tests

SFCGAL_BUILD_EXAMPLES : Build examples

SFCGAL_BUILD_OSG : Build components depending on OpenSceneGraph

SFCGAL_BUILD_VIEWER : Build basic viewer


* Dependencies location (environment variables)

GMP_DIR : GMP location

MPFR_DIR : MPFR location

CGAL_DIR : CGAL location

BOOST_ROOT : Boost location


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
* a 3D viewer, based on OpenSceneGraph and QT



Troubleshouting
--------------------

* Build failed with MinGW and boost 1.46.1 (problem with boost thread fixed in boost 1.47.0)





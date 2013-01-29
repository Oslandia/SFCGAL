SFCGAL
======

SFCGAL is a C++ wrapper library around [CGAL](http://www.cgal.org) with the aim of supporting ISO 19107 and OGC Simple Features for 3D operations.

It depends on CGAL 4.1

Compilation
-----------

* Use cmake
* make / make install

CGAL uses a lot of templated constructions. The whole building process may then take a while.

Test & usage
------------

In addition to the main SFCGAL library, the following optional components are part of the building process:
* a unit test suite, see the test/unit directory
* a regression test suite, see the test/regress directory
* a benchmark suite, see the test/bench directory
* a 3D viewer, based on OpenSceneGraph and QT

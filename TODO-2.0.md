# SFCGAL 2.0 - work in progress

## Code sprint 2016 conclusions about 1.*

* To get better performances, we should reduce abstraction on CGAL type
* New CGAL features in 4.7 (compared to 4.3) should lead to code removal
* OGC conformance in C API, not in C++ computational parts
    * M coordinate : Later using decoration
    * Avoid the use of hierarchies to reduce memory fragmentation due to dynamic allocation of each geometry components

## Key principles

* SFCGAL should become :
    * An IO library to convert serialized GIS geometries to/from CGAL data structures 
    * A set of helpers (iterator, visitors, etc.) to invoke CGAL algorithm on this data structures on 2DZ/3D
* Typedef OGC types relying on standard containers and CGAL types (instead of subclassing a Geometry class)
* Code removal
* Modern C++ code based on C++11 features


## Refactoring method

* [x] Restore types as typedef on CGAL structures with a variant to manage dynamism on geometry
* [ ] Restore IO for tests
    * [x] WKT
    * [ ] WKB (hexa)
* [ ] Restore progressively algorithm (Port sources from /src-legacy and /test-legacy to /src and /test with the new style)
* [ ] Restore the garden test

Finally :
 
* [ ] Restore CGAL API minimizing changes

## Supported geometry types

* Geometry (variant type for primitive and collection types)
* Primitive types
    * [x] Point
    * [x] LineString
    * [x] Triangle
    * [x] Polygon (with holes)
    * [x] Solid
* Collection
    * [x] MultiPoint
    * [x] MultiLineString
    * [x] TriangulatedSurface
    * [x] MultiPolygon
    * [x] PolyhedralSurface (considered as a MultiPolygon)
    * [x] GeometryCollection

Note that :
* A LineString is implicitly a LinearRing when it is a ring in a Polygon (with startPoint = endPoint).
* A MultiPolygon is implicity a closed PolyhedralSurface with correct orientation when it is a shell in a solid.


# Features to add in 2.0

## IO

- [ ] HEXA-WKB reader/writer

## Algorithms

- [ ] Alpha shapes 2D/3D
- [ ] Hole filling (usefull to transform dirty buildings to Solid)




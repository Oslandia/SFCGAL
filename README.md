# SFCGAL 2.0 - work in progress

## Code sprint 2016 conclusions about 1.*

* To get better performances, we should reduce abstraction on CGAL type
* New CGAL features in 4.7 (compared to 4.3) reduce
* 3D First : We should only manipulate 3D components as it's preferable to ignore Z than copy or cast points
* OGC conformance in C API, not in C++ computational parts
    * M coordinate : Later using decoration
    * Avoid the use of hierarchies to reduce memory fragmentation due to dynamic allocation of each geometry components

## Key principles

* SFCGAL should become :
    * An IO library to convert serialized GIS geometries to/from CGAL data structures 
    * A set of helpers (iterator, visitors, etc.) to invoke CGAL algorithm on this data structures on 2DZ/3D
* Typedef OGC types (instead of subclassing a Geometry class)
* Code removal
* Modern C++ code based on C++11 features


## Refactoring method

* [ ] Restore types as typedef on CGAL structures with a variant to manage dynamism on geometry
* [ ] Restore io from WKT and add HexaWKB reader
* [ ] Restore progressively algorithm (with tests and benchmarks)
* [ ] Restore the garden test

Finally :
 
* [ ] Restore CGAL API minimizing changes

## Supported geometry types

* Geometry (variant type for primitive and collection types)
* Primitive types
    * [x] Point
    * [x] LineString
    * [x] Triangle
    * [x] Polygon (with holes)
    * [ ] Solid
* Collection
    * [x] MultiPoint
    * [x] MultiLineString
    * [x] TriangulatedSurface
    * [x] MultiPolygon
    * [x] GeometryCollection

Note that :
* All geometry types are stored in 3D. Z is simply ignored when working in 2D.
* A LineString is implicitly a LinearRing when it is a ring in a Polygon (with startPoint = endPoint).
* A MultiPolygon is implicity a closed PolyhedralSurface with correct orientation when it is a shell in a solid.

# Features

### IO format

- [ ] WKT
- [ ] HEXA-WKB

### Convex hull

- [ ] convexHull
- [ ] convexHull3D

### Constraint delaunay triangulation

- [x] triangulate2DZ( g ) : 2DZ triangulation of points sets with constraints
- [ ] triangulatePolygons( g : Geometry ) : Triangulate polygons in there planes

TODO add all CAPI features.


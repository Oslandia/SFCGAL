# SFCGAL 2.0 - work in progress

## Code sprint 2016 conclusions about 1.*

* To get better performances, we should reduce abstraction on CGAL types
* New CGAL features in 4.7 (compared to 4.3) should lead to code removal
* OGC conformance in C API, not in C++ computational parts
    * ~~M coordinate : Later using decoration~~
    * Avoid the use of hierarchies to reduce memory fragmentation due to dynamic allocation of each geometry components (GeometryCollection will be the single type storing pointer to geometries)

## Key principles

* SFCGAL should become :
    * An IO library to convert serialized GIS geometries to/from CGAL data structures 
    * A set of helpers (iterator, visitors, etc.) to invoke CGAL algorithm on this data structures on 2DZ/3D
* 3D first : Store CGAL::Point_3 and tag 2D/3D nature (set3D/is3D) on geometries
* ~~Typedef OGC types relying on standard containers and CGAL types (instead of subclassing a Geometry class)~~ (would lead to a too deep refactoring, particularly to avoid changes in existing tests). Instead, the new design rely on STL containers.
* Code removal based on the use of CGAL new features since 4.3 (polygon mesh processing for example)
* Modern C++ code based on C++11 features

## Geometry types

* [x] Geometry<K> (base type with parametric kernel)
* [x] Primitive types
    * [x] Point<K>
    * [x] LineString<K>
    * [x] Triangle<K>
    * [x] Polygon<K> (with holes)
    * [x] Solid<K>
* [x] Collection of primitive geometry types
    * [x] MultiPoint<K>
    * [x] MultiLineString<K>
    * [x] TriangulatedSurface<K>
    * [x] MultiPolygon<K>
    * [x] PolyhedralSurface<K>
* [x] GeometryCollection<K> : collection of Geometry<K> (dynamic geometry type)

Note that a LineString is implicitly a LinearRing when it is a ring in a Polygon (with startPoint = endPoint).

Also, some types that doesn't belong to the Geometry hierarchy are defined :

* Mesh
    * [x] SurfaceMesh<K> (CGAL::SurfaceMesh<CGAL::Point_3<K>>)
    * [ ] MarkedPolyhedron<K>
* Graph (might be removed)
    * [ ] GeometryGraph
    * [ ] SurfaceGraph

## Refactoring method

(Note that sources from masters had been move to src-legacy, test-legacy, example-legacy)

* [x] Restore geometry types relying on CGAL types and STL containers

* [x] Restore IO
    * [x] Read WKT
    * [x] Write WKT
    * [x] Convert SFCGAL::Geometry to osg::Geometry (rely on polygon triangulation)

* [ ] Restore progressively features (Port sources from /src-legacy and /test-legacy to /src and /test with the new style)

* [ ] Restore benchs

* [ ] Restore the garden test

Finally :
 
* [ ] Restore CGAL C API minimizing changes

# Features (roadmap)

* [x] Find plane/normal for point set (used to triangulate 3D polygon)
* [x] Test if a geometry is planar with a given tolerance
* [x] Triangulate polygon 3D
* [x] Convert geometries to triangulated Mesh (CGAL::SurfaceMesh/CGAL::Polyhedron)
* [x] Tesselate geometries (convert surfaces to triangulated surfaces)
* [x] Test if Mesh is connected
* [x] Test if Mesh is closed
* [ ] Enveloppe 2D/3D (bounding box)
* [ ] Generators
    * [ ] Disc
    * [ ] Hoch fractal
    * [ ] Sierpinski fractal
    * [ ] Building (rely on straight skeleton)
* [ ] Force 2D/3D
* [ ] Length 2D/3D
* [ ] Area 2D/3D
* [ ] Volume
* [ ] Distance 2D/3D
* [ ] Extrude
* [ ] Translate
* [ ] Boundary 
* [ ] Homogenize geometry collection
* [ ] convexHull 2D/3D
* [ ] Boolean operations 2D/3D
    * [ ] covers 2D/3D
    * [ ] difference 2D/3D
    * [ ] intersection 2D/3D
    * [ ] union 2D/3D
* [ ] Validity checking 2D/3D
* [ ] Minkowski sum and offset
* [ ] Surface orientation 2D/3D
* [ ] Straight skeleton


# Support features (detail)

* [x] SFCGAL::apply_visitor : Static visitor that simplify implementation on dynamic geometry type 
* [x] Collect points from a geometry (usefull to find plane for a geometry)
* [x] Collect points and constraints from a geometry (mainly for constraints triangulation purpose)

* [x] VertexArrayBuilder (build a vector of CGAL::Point_3 to index geometry points)
* [x] TriangulatedMeshBuilder : Create a CGAL Mesh (SurfaceMesh/Polyhedron) with a consistent orientation from polygons

* [ ] Transform : Visit geometry points to apply a transform

* [ ] GeometrySet (a collection of CGAL primitive usefull for boolean ops)
* [ ] Graph view for geometries and isHalfEdge test (should this be restored?)


# Features to add in 2.0

## IO

- [ ] HEXA-WKB reader/writer

## Algorithms

- [ ] Alpha shapes 2D/3D 
- [ ] Hole filling (usefull to transform dirty buildings to Solid)




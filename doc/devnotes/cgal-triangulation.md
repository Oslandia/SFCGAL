# 2D delaunay triangulation with CGAL

Note that :

* Projection traits allows a triangulation on a 2D view of 3D points
* The default Constrained_Delaunay_triangulation_2 will crash if there are constraint intersections
* Epeck computation is only required to deal with this constraint intersections
* Compution is about 10 time slower with point by point insertion!

## Default triangulation data structure

The Triangulation Data Structure (TDS) 

```
typedef CGAL::Triangulation_vertex_base_2<Gt> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<Gt> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
```


## Triangulation in XY plane

```
std::vector< Epick::Point_3 > points ;
std::vector< std::pair< std::size_t, std::size_t > > constraints ;

typedef CGAL::Projection_traits_xy_3<Epeck>  Gt;
typedef CGAL::Constrained_Delaunay_triangulation_2<Gt> CDT ;

CDT cdt ;
cdt.insert_constraints(
    points.begin(), points.end(),
    constraints.begin(), constraints.end()
);
```


## Triangulation in arbitrary plane

see CGAL example Polygon_mesh_processing/triangulate_faces.h

```
typedef CGAL::Triangulation_2_filtered_projection_traits_3<Traits>   Gt;
typedef CGAL::Constrained_Delaunay_triangulation_2<Gt>
Gt gt(normal);
CDT cdt(gt);
```



## Triangulation in plane which allows constraints intersections

```
std::vector< Epeck::Point_3 > points ;
std::vector< std::pair< std::size_t, std::size_t > > constraints ;
detail::collectPointsAndConstraints(g,points,constraints);

typedef CGAL::Projection_traits_xy_3<Epeck>  Gt;
typedef CGAL::Triangulation_vertex_base_2<Gt> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<Gt> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Constrained_Delaunay_triangulation_2<Gt, TDS, CGAL::Exact_predicates_tag> CDT ;
typedef CDT::Finite_faces_iterator   Finite_faces_iterator ;

CDT cdt ;
cdt.insert_constraints(
    points.begin(), points.end(),
    constraints.begin(), constraints.end()
);
```


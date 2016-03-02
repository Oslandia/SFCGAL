#ifndef POLYGON_INSIDE_POLYHEDRON_H
#define POLYGON_INSIDE_POLYHEDRON_H

//
// Compatibility layer for Point_inside_polyhderon_3 that becomes Side_of_triangle_mesh in CGAL 4.7

#if CGAL_VERSION_NR < 1040701000 // version 4.7

#include <CGAL/Point_inside_polyhedron_3.h>
#define Point_inside_polyhedron CGAL::Point_inside_polyhedron_3

#else

#include <CGAL/Side_of_triangle_mesh.h>
#define Point_inside_polyhedron CGAL::Side_of_triangle_mesh

#endif

#endif

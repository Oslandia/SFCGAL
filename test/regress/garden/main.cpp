/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/chrono.hpp>

#include <SFCGAL/Exception.h>

#include <SFCGAL/all.h>

#include <SFCGAL/io/wkt.h>

using namespace SFCGAL ;

namespace po = boost::program_options ;

/*
 * Monkey testing of the SFCGAL pulic api
 */
int main( int argc, char* argv[] ){
	/*
	 * declare options
	 */
	po::options_description desc("garden test options : ");
	desc.add_options()
	    ("help", "produce help message")
	    ("progress", "display progress")
	    ("verbose",  "verbose mode")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl ;
	    return 0;
	}

	bool verbose  = vm.count("verbose") != 0 ;
	bool progress = vm.count("progress") != 0 ;


	//boost::timer timer ;
	boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();



    bool success = true;


	boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Functions called from lwgeom_sfcgal.c and corresponding C++ api calls
 *
sfcgal_geometry_collection_add_geometry(ret_geom, g) 
    GeometryCollection::addGeometry(geom)
sfcgal_geometry_collection_create()
    GeometryCollection::GeometryCollection()
sfcgal_geometry_collection_geometry_n(geom, i)
    GeometryCollection::geometryN(i)
sfcgal_geometry_collection_num_geometries(geom)
    GeometryCollection::numGeometries()
sfcgal_geometry_delete(converted)
    GeometryCollection::~GeometryCollection()
sfcgal_geometry_is_3d(geom)
    GeometryCollection::is3D()
sfcgal_geometry_is_empty(geom)
    GeometryCollection::isEmpty()
sfcgal_linestring_add_point(line,
    LineString::addPoint()
sfcgal_linestring_create()
    LineString::LineString()
sfcgal_linestring_num_points(geom)
    LineString::numPoints()
sfcgal_linestring_point_n(geom, i)
    LineString::pointN(i)
sfcgal_multi_linestring_create()
    MultiLineString::MultiLineString()
sfcgal_multi_point_create()
    MultiPoint::MultiPoint()
sfcgal_multi_polygon_create()
    MultiPolygon::MultiPolygon()
sfcgal_point_create()
    Point::Point()
sfcgal_point_create_from_xy(point.x, point.y)
    Point::Point(x,y)
sfcgal_point_create_from_xyz(point.x, point.y, point.z)
    Point::Point(x,y,z)
sfcgal_point_x(pt)
    Point::x()
sfcgal_point_y(pt)
    Point::y()
sfcgal_point_z(pt)
    Point::z()
sfcgal_polygon_add_interior_ring(ret_geom, ring)
    Polygon::addRing( ring )
sfcgal_polygon_create()
    Polygon::Polygon()
sfcgal_polygon_create_from_exterior_ring(exterior_ring)
    Polygon::Polygon(exterior_ring)
sfcgal_polygon_exterior_ring(geom)
    Polygon::exteriorRing()    
sfcgal_polygon_interior_ring_n(geom, i)
    Polygon::interiorRingN(i)
sfcgal_polygon_num_interior_rings(geom)
    Polygon::numInteriorRings()
sfcgal_polyhedral_surface_add_polygon(ret_geom, poly)
    PolyhedralSurface::addPolygon(poly)    
sfcgal_polyhedral_surface_create()
    PolyhedralSurface::PolyhedralSurface()
sfcgal_polyhedral_surface_num_polygons(geom)
    PolyhedralSurface::numPolygons()
sfcgal_polyhedral_surface_polygon_n( geom, i )
    PolyhedralSurface::polygonN(i)
sfcgal_solid_create_from_exterior_shell(shell)
    Solid::Solid(shell)
sfcgal_solid_num_shells(geom)
    Solid::numShells()
sfcgal_solid_shell_n(geom, i)
    Solid::shellN(i)
sfcgal_triangle_create()
    Triangle::Triangle()
sfcgal_triangle_set_vertex_from_xy (triangle, i, point.x, point.y)
    Triangle::vertex(i)
    Point(x,y)
sfcgal_triangle_set_vertex_from_xyz(triangle, 0, point.x, point.y, point.z)
    Triangle::vertex(i)
    Point(x,y,z)
sfcgal_triangle_vertex(geom, (i%3))
    Triangle::vertex(i) const
sfcgal_triangulated_surface_add_triangle(ret_geom, triangle)
    TriangulatedSurface::addTriangle(triangle)
sfcgal_triangulated_surface_create()
    TriangulatedSurface::TriangulatedSurface()
sfcgal_triangulated_surface_num_triangles(geom)
    TriangulatedSurface::numTriangles()
sfcgal_triangulated_surface_triangle_n(geom, i)
    TriangulatedSurface::triangleN(i)
sfcgal_version()
    Version()
*/

/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/PreparedGeometry.h>

#include <SFCGAL/capi/sfcgal_c.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/ewkt.h>
#include <SFCGAL/io/Serialization.h>

#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/convexHull.h>
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/distance3d.h>
#include <SFCGAL/algorithm/plane.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/algorithm/offset.h>
#include <SFCGAL/algorithm/straightSkeleton.h>

#include <SFCGAL/transform/ForceZOrderPoints.h>
#include <SFCGAL/transform/RoundTransform.h>

//
// Note about sfcgal_geometry_t pointers: they are basically void* pointers that represent
// pointers to a SFCGAL::Geometry.
// In order to support multiple inheritance:
// every input or output sfcgal_geometry_t* is a pointer to the *base class* SFCGAL::Geometry.
// If a function wants to return a sub-class, it must be up casted before returned (static_cast)
// If a function wants to use a sub-class from a parameter, it must also be down casted.
// For instance, static_cast<SFCGAL::Point*>(reinterpret_cast<SFCGAL::Geometry*>(p))
//
// SFCGAL::PreparedGeometry has no vtable and can thus be manipuled through reinterpret_cast without
// problem

template <class T>
inline T* down_cast( sfcgal_geometry_t* p )
{
	return static_cast<T*>(reinterpret_cast<SFCGAL::Geometry*>(p));
}

template <class T>
inline const T* down_const_cast( const sfcgal_geometry_t* p )
{
	return static_cast<const T*>(reinterpret_cast<const SFCGAL::Geometry*>(p));
}

static sfcgal_error_handler_t __sfcgal_warning_handler;
static sfcgal_error_handler_t __sfcgal_error_handler;

#define SFCGAL_WARNING __sfcgal_warning_handler
#define SFCGAL_ERROR __sfcgal_error_handler

extern "C" void sfcgal_set_error_handlers( sfcgal_error_handler_t warning_handler, sfcgal_error_handler_t error_handler )
{
	__sfcgal_warning_handler = warning_handler;
	__sfcgal_error_handler = error_handler;
}

static sfcgal_alloc_handler_t __sfcgal_alloc_handler;
static sfcgal_free_handler_t __sfcgal_free_handler;

extern "C" void sfcgal_set_alloc_handlers( sfcgal_alloc_handler_t alloc_handler, sfcgal_free_handler_t free_handler )
{
	__sfcgal_alloc_handler = alloc_handler;
	__sfcgal_free_handler = free_handler;
}

extern "C" void sfcgal_init()
{
	__sfcgal_warning_handler = printf;
	__sfcgal_error_handler = printf;
	__sfcgal_alloc_handler = malloc;
	__sfcgal_free_handler = free;
}

extern "C" sfcgal_geometry_type_t sfcgal_geometry_type_id( const sfcgal_geometry_t* geom )
{
	return (sfcgal_geometry_type_t)reinterpret_cast<const SFCGAL::Geometry*>(geom)->geometryTypeId();
}

extern "C" int sfcgal_geometry_is_3d( const sfcgal_geometry_t* geom )
{
	return (int)reinterpret_cast<const SFCGAL::Geometry*>(geom)->is3D();
}

extern "C" int sfcgal_geometry_is_empty( const sfcgal_geometry_t* geom )
{
	return (int)reinterpret_cast<const SFCGAL::Geometry*>(geom)->isEmpty();
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_clone( const sfcgal_geometry_t* geom )
{
	return reinterpret_cast<const SFCGAL::Geometry*>(geom)->clone();
}

extern "C" void sfcgal_geometry_delete( sfcgal_geometry_t* geom )
{
	delete reinterpret_cast<SFCGAL::Geometry*>(geom);
}

extern "C" void sfcgal_geometry_as_text( const sfcgal_geometry_t* pgeom, char** buffer, size_t* len )
{
	std::string wkt = reinterpret_cast<const SFCGAL::Geometry*>( pgeom )->asText();
	*buffer = (char*)__sfcgal_alloc_handler( wkt.size() + 1 );
	*len = wkt.size();
	strncpy( *buffer, wkt.c_str(), *len );
}

/**
 * Point
 */
extern "C" sfcgal_geometry_t* sfcgal_point_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Point());
}

extern "C" sfcgal_geometry_t* sfcgal_point_create_from_xy( double x, double y )
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Point( x, y ));
}
extern "C" sfcgal_geometry_t* sfcgal_point_create_from_xyz( double x, double y, double z )
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Point( x, y, z ));
}

extern "C" double sfcgal_point_x( const sfcgal_geometry_t* geom )
{
	return CGAL::to_double(down_const_cast<SFCGAL::Point>(geom)->x());
}

extern "C" double sfcgal_point_y( const sfcgal_geometry_t* geom )
{
	return CGAL::to_double(down_const_cast<SFCGAL::Point>(geom)->y());
}

extern "C" double sfcgal_point_z( const sfcgal_geometry_t* geom )
{
	return CGAL::to_double(down_const_cast<SFCGAL::Point>(geom)->z());
}

/**
 * LineString
 */
extern "C" sfcgal_geometry_t* sfcgal_linestring_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::LineString());
}

extern "C" size_t sfcgal_linestring_num_points( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::LineString>(geom)->numPoints();
}

extern "C" const sfcgal_geometry_t* sfcgal_linestring_point_n( const sfcgal_geometry_t* geom, size_t i )
{
	return static_cast<const SFCGAL::Geometry*>(&(down_const_cast<SFCGAL::LineString>(geom)->pointN( i )));
}

extern "C" void sfcgal_linestring_add_point( sfcgal_geometry_t* geom, sfcgal_geometry_t* point )
{
	down_cast<SFCGAL::LineString>(geom)->addPoint( down_cast<SFCGAL::Point>(point) );
}

/**
 * Triangle
 */
extern "C" sfcgal_geometry_t* sfcgal_triangle_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Triangle());
}

extern "C" sfcgal_geometry_t* sfcgal_triangle_create_from_points( const sfcgal_geometry_t* pa,
								  const sfcgal_geometry_t* pb,
								  const sfcgal_geometry_t* pc)
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Triangle( *down_const_cast<SFCGAL::Point>(pa),
								    *down_const_cast<SFCGAL::Point>(pb),
								    *down_const_cast<SFCGAL::Point>(pc)));
}


extern "C" const sfcgal_geometry_t* sfcgal_triangle_vertex( const sfcgal_geometry_t* geom, int i )
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::Triangle>(geom)->vertex(i));
}

extern "C" void sfcgal_triangle_set_vertex( sfcgal_geometry_t* geom, int i, const sfcgal_geometry_t* point )
{
	down_cast<SFCGAL::Triangle>(geom)->vertex( i ) = *down_const_cast<const SFCGAL::Point>(point);
}

/**
 * Polygon
 */
extern "C" sfcgal_geometry_t* sfcgal_polygon_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Polygon());
}

extern "C" sfcgal_geometry_t* sfcgal_polygon_create_from_exterior_ring( sfcgal_geometry_t* ring )
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Polygon( down_cast<SFCGAL::LineString>(ring) ));
}

extern "C" const sfcgal_geometry_t* sfcgal_polygon_exterior_ring( const sfcgal_geometry_t* geom )
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::Polygon>( geom )->exteriorRing());
}

extern "C" size_t sfcgal_polygon_num_interior_rings( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::Polygon>( geom )->numInteriorRings();
}

extern "C" const sfcgal_geometry_t* sfcgal_polygon_interior_ring_n( const sfcgal_geometry_t* geom, size_t i)
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::Polygon>( geom )->interiorRingN( i ));
}

extern "C" void sfcgal_polygon_add_interior_ring( sfcgal_geometry_t* geom, sfcgal_geometry_t* ring )
{
	down_cast<SFCGAL::Polygon>( geom )->addRing( down_cast<SFCGAL::LineString>(ring) );
}

/**
 * Geometry collection
 */

extern "C" sfcgal_geometry_t* sfcgal_geometry_collection_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::GeometryCollection());
}

extern "C" size_t sfcgal_geometry_collection_num_geometries( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::GeometryCollection>( geom )->numGeometries();
}

extern "C" const sfcgal_geometry_t* sfcgal_geometry_collection_geometry_n( const sfcgal_geometry_t* geom, size_t i )
{
	const SFCGAL::GeometryCollection* g = down_const_cast<SFCGAL::GeometryCollection>( geom );
	return static_cast<const SFCGAL::Geometry*>(&g->geometryN( i ));
}

extern "C" void sfcgal_geometry_collection_add_geometry( sfcgal_geometry_t* geom, sfcgal_geometry_t* ngeom )
{
	down_cast<SFCGAL::GeometryCollection>( geom )->addGeometry( reinterpret_cast<SFCGAL::Geometry*>(ngeom) );
}

/**
 * Multi-*
 */
extern "C" sfcgal_geometry_t* sfcgal_multi_point_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::MultiPoint());
}

extern "C" sfcgal_geometry_t* sfcgal_multi_linestring_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::MultiLineString());
}

extern "C" sfcgal_geometry_t* sfcgal_multi_polygon_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::MultiPolygon());
}

/**
 * Polyhedral surface
 */

extern "C" sfcgal_geometry_t* sfcgal_polyhedral_surface_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::PolyhedralSurface());
}

extern "C" size_t sfcgal_polyhedral_surface_num_polygons( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::PolyhedralSurface>( geom )->numPolygons();
}

extern "C" const sfcgal_geometry_t* sfcgal_polyhedral_surface_polygon_n( const sfcgal_geometry_t* geom, size_t i )
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::PolyhedralSurface>( geom )->polygonN( i ));
}

extern "C" void sfcgal_polyhedral_surface_add_polygon( sfcgal_geometry_t* geom, sfcgal_geometry_t* poly )
{
	return down_cast<SFCGAL::PolyhedralSurface>( geom )->addPolygon( down_cast<SFCGAL::Polygon>(poly) );
}

/**
 * Triangulated surface
 */

extern "C" sfcgal_geometry_t* sfcgal_triangulated_surface_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::TriangulatedSurface());
}

extern "C" size_t sfcgal_triangulated_surface_num_triangles( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::TriangulatedSurface>( geom )->numTriangles();
}

extern "C" const sfcgal_geometry_t* sfcgal_triangulated_surface_triangle_n( const sfcgal_geometry_t* geom, size_t i )
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::TriangulatedSurface>( geom )->triangleN( i ));
}

extern "C" void sfcgal_triangulated_surface_add_triangle( sfcgal_geometry_t* geom, sfcgal_geometry_t* triangle )
{
	down_cast<SFCGAL::TriangulatedSurface>( geom )->addTriangle( down_cast<SFCGAL::Triangle>(triangle) );
}

/**
 * Solid
 */

extern "C" sfcgal_geometry_t* sfcgal_solid_create()
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Solid());
}

extern "C" sfcgal_geometry_t* sfcgal_solid_create_from_exterior_shell( sfcgal_geometry_t* shell )
{
	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Solid( down_cast<SFCGAL::PolyhedralSurface>(shell) ));
}

extern "C" size_t sfcgal_solid_num_shells( const sfcgal_geometry_t* geom )
{
	return down_const_cast<SFCGAL::Solid>( geom )->numShells();
}

extern "C" const sfcgal_geometry_t* sfcgal_solid_shell_n( const sfcgal_geometry_t* geom, size_t i )
{
	return static_cast<const SFCGAL::Geometry*>(&down_const_cast<SFCGAL::Solid>( geom )->shellN( i ));
}

extern "C" void sfcgal_solid_add_interior_shell( sfcgal_geometry_t* geom , sfcgal_geometry_t* shell )
{
	down_cast<SFCGAL::Solid>( geom )->addInteriorShell( down_cast<SFCGAL::PolyhedralSurface>( shell ) );
}

extern "C" sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create()
{
	return new SFCGAL::PreparedGeometry();
}

extern "C" sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create_from_geometry( sfcgal_geometry_t* geom, srid_t srid )
{
	return new SFCGAL::PreparedGeometry( reinterpret_cast<SFCGAL::Geometry*>(geom), srid );
}

extern "C" void sfcgal_prepared_geometry_delete( sfcgal_prepared_geometry_t* pgeom )
{
	delete reinterpret_cast<SFCGAL::PreparedGeometry*>( pgeom );
}

extern "C" const sfcgal_geometry_t* sfcgal_prepared_geometry_geometry( const sfcgal_prepared_geometry_t* pgeom )
{
	return &reinterpret_cast<const SFCGAL::PreparedGeometry*>(pgeom)->geometry();
}

extern "C" void sfcgal_prepared_geometry_set_geometry( sfcgal_prepared_geometry_t* pgeom, sfcgal_geometry_t * geom )
{
	reinterpret_cast<SFCGAL::PreparedGeometry*>(pgeom)->resetGeometry( reinterpret_cast<SFCGAL::Geometry*>(geom) );
}

extern "C" srid_t sfcgal_prepared_geometry_srid( const sfcgal_prepared_geometry_t* pgeom )
{
	return reinterpret_cast<const SFCGAL::PreparedGeometry*>( pgeom )->SRID();
}

extern "C" void sfcgal_prepared_geometry_set_srid( sfcgal_prepared_geometry_t* pgeom, srid_t srid )
{
	reinterpret_cast<SFCGAL::PreparedGeometry*>( pgeom )->SRID() = srid;
}

extern "C" void sfcgal_prepared_geometry_as_ewkt( const sfcgal_prepared_geometry_t* pgeom, int num_decimals, char** buffer, size_t* len )
{
	std::string ewkt = reinterpret_cast<const SFCGAL::PreparedGeometry*>( pgeom )->asEWKT();
	*buffer = (char*)__sfcgal_alloc_handler( ewkt.size() + 1 );
	*len = ewkt.size();
	strncpy( *buffer, ewkt.c_str(), *len );
}

extern "C" sfcgal_geometry_t* sfcgal_io_read_wkt( const char* str, size_t len )
{
	std::auto_ptr<SFCGAL::Geometry> g;
	try {
		g = SFCGAL::io::readWkt( str, len );
	}
	catch ( std::exception& e )
	{
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return g.release();
}

extern "C" void sfcgal_io_write_binary_prepared( const sfcgal_prepared_geometry_t* geom, char** buffer, size_t* len )
{
	const SFCGAL::PreparedGeometry *g = reinterpret_cast<const SFCGAL::PreparedGeometry*>(geom);
	std::string str = SFCGAL::io::writeBinaryPrepared( *g );
	*buffer = (char*)__sfcgal_alloc_handler( str.size() + 1 );
	*len = str.size();
	memcpy( *buffer, str.c_str(), *len );
}

extern "C" sfcgal_prepared_geometry_t* sfcgal_io_read_binary_prepared( const char* str, size_t len )
{
	std::string sstr( str, len );
	std::auto_ptr<SFCGAL::PreparedGeometry> g;
	try
	{
		g = SFCGAL::io::readBinaryPrepared( sstr );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During read_binary_prepared" );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return g.release();
}

extern "C" sfcgal_prepared_geometry_t* sfcgal_io_read_ewkt( const char* str, size_t len )
{
	std::auto_ptr<SFCGAL::PreparedGeometry> g;
	try {
		g = SFCGAL::io::readEwkt( str, len );
	}
	catch ( std::exception& e )
	{
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	SFCGAL::PreparedGeometry* pg = g.release();
	return pg;
}

// Functions that take two geometries and return a scalar
//
// name: C function name
// ret_type: C function return type
// sfcgal_function: C++ SFCGAL method to call
// cpp_type: C++ return type (might be different than ret_type)
// fail_value: returned value on failure
#define SFCGAL_GEOMETRY_FUNCTION_BINARY_SCALAR( name, sfcgal_function, ret_type, cpp_type, fail_value ) \
	extern "C" ret_type sfcgal_geometry_##name( const sfcgal_geometry_t* ga, const sfcgal_geometry_t* gb ) \
	{								\
		cpp_type r;							\
		try							\
		{							\
			r = sfcgal_function( *(const SFCGAL::Geometry*)(ga), *(const SFCGAL::Geometry*)(gb) ); \
		}							\
		catch ( std::exception& e )				\
		{							\
			SFCGAL_WARNING( "During " #name "(A,B) :" ); \
			SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() ); \
			SFCGAL_WARNING( "   and B: %s", ((const SFCGAL::Geometry*)(gb))->asText().c_str() ); \
			SFCGAL_ERROR( "%s", e.what() );	\
			return fail_value;					\
		}							\
		return r;					\
	}

#define SFCGAL_GEOMETRY_FUNCTION_BINARY_PREDICATE( name, sfcgal_function ) \
	SFCGAL_GEOMETRY_FUNCTION_BINARY_SCALAR( name, sfcgal_function, int, bool, -1 )

SFCGAL_GEOMETRY_FUNCTION_BINARY_PREDICATE( intersects, SFCGAL::algorithm::intersects )
SFCGAL_GEOMETRY_FUNCTION_BINARY_PREDICATE( intersects_3d, SFCGAL::algorithm::intersects3D )

#define SFCGAL_GEOMETRY_FUNCTION_BINARY_MEASURE( name, sfcgal_function ) \
	SFCGAL_GEOMETRY_FUNCTION_BINARY_SCALAR( name, sfcgal_function, double, double, -1.0 )

SFCGAL_GEOMETRY_FUNCTION_BINARY_MEASURE( distance, SFCGAL::algorithm::distance )
SFCGAL_GEOMETRY_FUNCTION_BINARY_MEASURE( distance_3d, SFCGAL::algorithm::distance3D )


#define SFCGAL_GEOMETRY_FUNCTION_BINARY_CONSTRUCTION( name, sfcgal_function ) \
	extern "C" sfcgal_geometry_t* sfcgal_geometry_##name( const sfcgal_geometry_t* ga, const sfcgal_geometry_t* gb ) \
	{								\
		std::auto_ptr<SFCGAL::Geometry> result;			\
		try							\
		{							\
			result = sfcgal_function( *(const SFCGAL::Geometry*)(ga), *(const SFCGAL::Geometry*)(gb) ); \
		}							\
		catch ( std::exception& e )				\
		{							\
			SFCGAL_WARNING( "During " #name "(A,B) :" ); \
			SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() ); \
			SFCGAL_WARNING( "   and B: %s", ((const SFCGAL::Geometry*)(gb))->asText().c_str() ); \
			SFCGAL_ERROR( "%s", e.what() );	\
			return 0;					\
		}							\
		return result.release();				\
	}

SFCGAL_GEOMETRY_FUNCTION_BINARY_CONSTRUCTION( intersection, SFCGAL::algorithm::intersection )
SFCGAL_GEOMETRY_FUNCTION_BINARY_CONSTRUCTION( intersection_3d, SFCGAL::algorithm::intersection3D )

#define SFCGAL_GEOMETRY_FUNCTION_UNARY_CONSTRUCTION( name, sfcgal_function ) \
	extern "C" sfcgal_geometry_t* sfcgal_geometry_##name( const sfcgal_geometry_t* ga ) \
	{								\
		std::auto_ptr<SFCGAL::Geometry> result;			\
		try							\
		{							\
			result = sfcgal_function( *(const SFCGAL::Geometry*)(ga) ); \
		}							\
		catch ( std::exception& e )				\
		{							\
			SFCGAL_WARNING( "During " #name "(A) :" ); \
			SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() ); \
			SFCGAL_ERROR( "%s", e.what() );	\
			return 0;					\
		}							\
		return result.release();				\
	}

SFCGAL_GEOMETRY_FUNCTION_UNARY_CONSTRUCTION( convexhull, SFCGAL::algorithm::convexHull )
SFCGAL_GEOMETRY_FUNCTION_UNARY_CONSTRUCTION( convexhull_3d, SFCGAL::algorithm::convexHull3D )
SFCGAL_GEOMETRY_FUNCTION_UNARY_CONSTRUCTION( straight_skeleton, SFCGAL::algorithm::straightSkeleton )

#define SFCGAL_GEOMETRY_FUNCTION_UNARY_MEASURE( name, sfcgal_function ) \
	extern "C" double sfcgal_geometry_##name( const sfcgal_geometry_t* ga ) \
	{								\
		double r;						\
		try							\
		{							\
			r = sfcgal_function( *(const SFCGAL::Geometry*)(ga) ); \
		}							\
		catch ( std::exception& e )				\
		{							\
			SFCGAL_WARNING( "During " #name "(A,B) :" ); \
			SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() ); \
			SFCGAL_ERROR( "%s", e.what() );	\
			return -1.0;					\
		}							\
		return r;						\
	}

SFCGAL_GEOMETRY_FUNCTION_UNARY_MEASURE( area, SFCGAL::algorithm::area )
SFCGAL_GEOMETRY_FUNCTION_UNARY_MEASURE( area_3d, SFCGAL::algorithm::area3D )

extern "C" int sfcgal_geometry_has_plane( const sfcgal_geometry_t* ga ) 
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	if ( g->geometryTypeId() != SFCGAL::TYPE_POLYGON ) {
		SFCGAL_ERROR( "has_plane() only applies to polygons" );
		return -1;
	}

	bool r;
	try
	{
		r = SFCGAL::algorithm::hasPlane3D< SFCGAL::Kernel >( g->as< const SFCGAL::Polygon >() );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During has_plane(A) :" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return -1.0;
	}
	return r ? 1 : 0;
}

extern "C" int sfcgal_geometry_pointing_up( const sfcgal_geometry_t* ga ) 
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	if ( g->geometryTypeId() != SFCGAL::TYPE_POLYGON ) {
		SFCGAL_ERROR( "pointing_up() only applies to polygons" );
		return -1;
	}

	bool r;
	try
	{
		r = g->as<const SFCGAL::Polygon>().isCounterClockWiseOriented();
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During pointing_up(A) :" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return -1.0;
	}
	return r ? 1 : 0;
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_make_solid( const sfcgal_geometry_t* ga ) 
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	if ( g->geometryTypeId() != SFCGAL::TYPE_POLYHEDRALSURFACE ) {
		SFCGAL_ERROR( "make_solid() only applies to polyhedral surfaces" );
		return 0;
	}

	return static_cast<SFCGAL::Geometry*>(new SFCGAL::Solid( g->as<const SFCGAL::PolyhedralSurface>() ));
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_force_z_up( const sfcgal_geometry_t* ga ) 
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	SFCGAL::Geometry* gb = g->clone();
	SFCGAL::transform::ForceZOrderPoints forceZ;
	try
	{
		gb->accept( forceZ );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During force_z_up(A) :" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return gb;
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_triangulate( const sfcgal_geometry_t* ga )
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	SFCGAL::TriangulatedSurface* surf = new SFCGAL::TriangulatedSurface;
	try
	{
		SFCGAL::algorithm::triangulate( *g, *surf );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During triangulate(A) :" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return static_cast<SFCGAL::Geometry*>(surf);
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_triangulate_2d( const sfcgal_geometry_t* ga )
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	SFCGAL::TriangulatedSurface* surf = new SFCGAL::TriangulatedSurface;
	try
	{
		SFCGAL::algorithm::triangulate2D( *g, *surf );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During triangulate_2d(A) :" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return static_cast<SFCGAL::Geometry*>(surf);
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_extrude( const sfcgal_geometry_t* ga, double x, double y, double z )
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	SFCGAL::Geometry* gb = g->clone();
	SFCGAL::transform::ForceZOrderPoints forceZ;
	std::auto_ptr<SFCGAL::Geometry> result;
	try
	{
		gb->accept( forceZ );
		result = SFCGAL::algorithm::extrude( *gb, x, y, z );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During extrude(A, %g, %g, %g) :", x, y, z );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return result.release();
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_copy( const sfcgal_geometry_t* ga )
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	return g->clone();
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_round( const sfcgal_geometry_t* ga, int scale )
{
	const SFCGAL::Geometry* g = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	SFCGAL::Geometry* gb = g->clone();
	//	SFCGAL_WARNING( "geom: %s %s", gb->asText().c_str(), typeid(g).name() );

	SFCGAL::transform::RoundTransform roundT( scale );
	try
	{
		gb->accept( roundT );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During round(A):" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	//	SFCGAL_WARNING( "processed geom: %s", gb->asText().c_str() );
	return gb;
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_minkowski_sum( const sfcgal_geometry_t* ga, const sfcgal_geometry_t* gb )
{
	const SFCGAL::Geometry* g1 = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	const SFCGAL::Geometry* g2 = reinterpret_cast<const SFCGAL::Geometry*>(gb);
	if ( g2->geometryTypeId() != SFCGAL::TYPE_POLYGON ) {
		SFCGAL_ERROR( "minkowski_sum(): the second argument must be a polygon" );
		return 0;
	}

	std::auto_ptr<SFCGAL::MultiPolygon> sum;
	try
	{
		sum = SFCGAL::algorithm::minkowskiSum( *g1, g2->as<const SFCGAL::Polygon>() );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During minkowski_sum(A,B):" );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_WARNING( "   and B: %s", ((const SFCGAL::Geometry*)(gb))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return sum.release();
}

extern "C" sfcgal_geometry_t* sfcgal_geometry_offset_polygon( const sfcgal_geometry_t* ga, double offset )
{
	const SFCGAL::Geometry* g1 = reinterpret_cast<const SFCGAL::Geometry*>(ga);
	std::auto_ptr<SFCGAL::MultiPolygon> mp;
	try
	{
		mp = SFCGAL::algorithm::offset( *g1, offset );
	}
	catch ( std::exception& e )
	{
		SFCGAL_WARNING( "During offset(A,%g):", offset );
		SFCGAL_WARNING( "  with A: %s", ((const SFCGAL::Geometry*)(ga))->asText().c_str() );
		SFCGAL_ERROR( "%s", e.what() );
		return 0;
	}
	return mp.release();
}


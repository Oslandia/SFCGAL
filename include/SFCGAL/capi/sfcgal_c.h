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
#ifndef _SFCGAL_CAPI_H_
#define _SFCGAL_CAPI_H_
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * Minimal C API for SFCGAL
 *
 */

/*--------------------------------------------------------------------------------------*
 *
 * Support for SFCGAL::Geometry class hierarchy
 *
 *--------------------------------------------------------------------------------------*/
 
/* sfcgal_geometry_t is an opaque pointer type */
typedef void sfcgal_geometry_t;

typedef enum {
	//     TYPE_GEOMETRY            = 0, //abstract
	SFCGAL_TYPE_POINT               = 1,
	SFCGAL_TYPE_LINESTRING          = 2,
	SFCGAL_TYPE_POLYGON             = 3,
	SFCGAL_TYPE_MULTIPOINT          = 4,
	SFCGAL_TYPE_MULTILINESTRING     = 5,
	SFCGAL_TYPE_MULTIPOLYGON        = 6,
	SFCGAL_TYPE_GEOMETRYCOLLECTION  = 7,
	//     TYPE_CIRCULARSTRING      = 8,
	//     TYPE_COMPOUNDCURVE       = 9,
	//     TYPE_CURVEPOLYGON        = 10,
	//     TYPE_MULTICURVE          = 11, //abstract
	//     TYPE_MULTISURFACE        = 12, //abstract
	//     TYPE_CURVE               = 13, //abstract
	//     TYPE_SURFACE             = 14, //abstract
	SFCGAL_TYPE_POLYHEDRALSURFACE   = 15,
	SFCGAL_TYPE_TRIANGULATEDSURFACE = 16,
	
	//-- not official codes
	SFCGAL_TYPE_TRIANGLE            = 100, //17 in Wikipedia???
	SFCGAL_TYPE_SOLID               = 101,
	SFCGAL_TYPE_MULTISOLID          = 102
} sfcgal_geometry_type_t ;

sfcgal_geometry_type_t    sfcgal_geometry_type_id( const sfcgal_geometry_t* );
int                       sfcgal_geometry_is_3d( const sfcgal_geometry_t* );
int                       sfcgal_geometry_is_empty( const sfcgal_geometry_t* );
sfcgal_geometry_t*        sfcgal_geometry_clone( const sfcgal_geometry_t* );

void                      sfcgal_geometry_delete( sfcgal_geometry_t* );

/* buffer is allocated and must be freed by the caller */
void                      sfcgal_geometry_as_text( const sfcgal_geometry_t*, char** buffer, size_t* len );

/**
 * Point
 */
sfcgal_geometry_t*        sfcgal_point_create();
sfcgal_geometry_t*        sfcgal_point_create_from_xy( double x, double y );
sfcgal_geometry_t*        sfcgal_point_create_from_xyz( double x, double y, double z );

double                    sfcgal_point_x( const sfcgal_geometry_t* );
double                    sfcgal_point_y( const sfcgal_geometry_t* );
double                    sfcgal_point_z( const sfcgal_geometry_t* );

/**
 * LineString
 */
sfcgal_geometry_t*        sfcgal_linestring_create();

size_t                    sfcgal_linestring_num_points( const sfcgal_geometry_t* );
/* return a const reference to a point */
const sfcgal_geometry_t*  sfcgal_linestring_point_n( const sfcgal_geometry_t*, size_t i );
/* add a point, takes ownership */
void                      sfcgal_linestring_add_point( sfcgal_geometry_t*, sfcgal_geometry_t* );

/**
 * Triangle
 */
sfcgal_geometry_t*        sfcgal_triangle_create();
/** copy points passed in arguments */
sfcgal_geometry_t*        sfcgal_triangle_create_from_points( const sfcgal_geometry_t*,
							      const sfcgal_geometry_t*,
							      const sfcgal_geometry_t*);

/* return a const reference */
const sfcgal_geometry_t*  sfcgal_triangle_vertex( const sfcgal_geometry_t*, int i );
/* copy vertex */
void                      sfcgal_triangle_set_vertex( sfcgal_geometry_t*, int i, const sfcgal_geometry_t* );
void                      sfcgal_triangle_set_vertex_from_xy( sfcgal_geometry_t*, int i, double x, double y );
void                      sfcgal_triangle_set_vertex_from_xyz( sfcgal_geometry_t*, int i, double x, double y, double z );

/**
 * Polygon
 */
sfcgal_geometry_t*        sfcgal_polygon_create();
/* takes ownership of the exterior ring */
sfcgal_geometry_t*        sfcgal_polygon_create_from_exterior_ring( sfcgal_geometry_t* );

const sfcgal_geometry_t*  sfcgal_polygon_exterior_ring( const sfcgal_geometry_t* );

size_t                    sfcgal_polygon_num_interior_rings( const sfcgal_geometry_t* );
const sfcgal_geometry_t*  sfcgal_polygon_interior_ring_n( const sfcgal_geometry_t*, size_t );

/* takes ownership */
void                      sfcgal_polygon_add_interior_ring( sfcgal_geometry_t*, sfcgal_geometry_t* );

/**
 * Geometry collection
 */

sfcgal_geometry_t*        sfcgal_geometry_collection_create();

size_t                    sfcgal_geometry_collection_num_geometries( const sfcgal_geometry_t* );
const sfcgal_geometry_t*  sfcgal_geometry_collection_geometry_n( const sfcgal_geometry_t*, size_t i );

/* takes ownership */
void                      sfcgal_geometry_collection_add_geometry( sfcgal_geometry_t*, sfcgal_geometry_t* );

/**
 * Multi-*
 */
sfcgal_geometry_t*        sfcgal_multi_point_create();
sfcgal_geometry_t*        sfcgal_multi_linestring_create();
sfcgal_geometry_t*        sfcgal_multi_polygon_create();

/**
 * Polyhedral surface
 */

sfcgal_geometry_t*        sfcgal_polyhedral_surface_create();

size_t                    sfcgal_polyhedral_surface_num_polygons( const sfcgal_geometry_t* );
const sfcgal_geometry_t*  sfcgal_polyhedral_surface_polygon_n( const sfcgal_geometry_t*, size_t i );

/* takes ownership */
void                      sfcgal_polyhedral_surface_add_polygon( sfcgal_geometry_t*, sfcgal_geometry_t* );

/**
 * Triangulated surface
 */

sfcgal_geometry_t*        sfcgal_triangulated_surface_create();

size_t                    sfcgal_triangulated_surface_num_triangles( const sfcgal_geometry_t* );
const sfcgal_geometry_t*  sfcgal_triangulated_surface_triangle_n( const sfcgal_geometry_t*, size_t i );

/* takes ownership */
void                      sfcgal_triangulated_surface_add_triangle( sfcgal_geometry_t*, sfcgal_geometry_t* );

/**
 * Solid
 */

sfcgal_geometry_t*        sfcgal_solid_create();
/* takes ownership of the given exterior shell */
sfcgal_geometry_t*        sfcgal_solid_create_from_exterior_shell( sfcgal_geometry_t* );

size_t                    sfcgal_solid_num_shells( const sfcgal_geometry_t* );
const sfcgal_geometry_t*  sfcgal_solid_shell_n( const sfcgal_geometry_t*, size_t i );

/* takes ownership */
void                      sfcgal_solid_add_shell( sfcgal_geometry_t*, sfcgal_geometry_t* );

	
/*--------------------------------------------------------------------------------------*
 *
 * Support for SFCGAL::PreparedGeometry
 *
 *--------------------------------------------------------------------------------------*/

typedef void sfcgal_prepared_geometry_t;
typedef uint32_t srid_t;

sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create();
/* takes ownership of the given geometry */
sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create_from_geometry( sfcgal_geometry_t*, srid_t srid );

/* will also delete the underlying geometry */
void                        sfcgal_prepared_geometry_delete( sfcgal_prepared_geometry_t* );

const sfcgal_geometry_t*    sfcgal_prepared_geometry_geometry( const sfcgal_prepared_geometry_t* );
/* takes ownership of the given geometry */
void                        sfcgal_prepared_geometry_set_geometry( sfcgal_prepared_geometry_t*, sfcgal_geometry_t * );

srid_t                      sfcgal_prepared_geometry_srid( const sfcgal_prepared_geometry_t* );
void                        sfcgal_prepared_geometry_set_srid( sfcgal_prepared_geometry_t*, srid_t );

/* returned string must be deleted by the caller */
void                        sfcgal_prepared_geometry_as_ewkt( const sfcgal_prepared_geometry_t*, int num_decimals, char **buffer, size_t* len );

/*--------------------------------------------------------------------------------------*
 *
 * I/O functions
 *
 *--------------------------------------------------------------------------------------*/

/**
 * io::readWKT
 */
sfcgal_geometry_t*          sfcgal_io_read_wkt( const char*, size_t len );
sfcgal_prepared_geometry_t* sfcgal_io_read_ewkt( const char*, size_t len );

/**
 * Serialization
 */
/* allocates into char**, must be freed by the caller */
void                        sfcgal_io_write_binary_prepared( const sfcgal_prepared_geometry_t*, char**, size_t* );
sfcgal_prepared_geometry_t* sfcgal_io_read_binary_prepared( const char*, size_t l );

/*--------------------------------------------------------------------------------------*
 *
 * Spatial processing
 *
 *--------------------------------------------------------------------------------------*/

int                         sfcgal_geometry_intersects( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
int                         sfcgal_geometry_intersects_3d( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_intersection( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_intersection_3d( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_convexhull( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_convexhull_3d( const sfcgal_geometry_t* );
double                      sfcgal_geometry_area( const sfcgal_geometry_t* );
double                      sfcgal_geometry_area_3d( const sfcgal_geometry_t* );
int                         sfcgal_geometry_has_plane( const sfcgal_geometry_t* );
int                         sfcgal_geometry_pointing_up( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_tesselate( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_triangulate_2dz( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_extrude( const sfcgal_geometry_t*, double, double, double );
sfcgal_geometry_t*          sfcgal_geometry_make_solid( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_force_z_up( const sfcgal_geometry_t* );
double                      sfcgal_geometry_distance( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
double                      sfcgal_geometry_distance_3d( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_copy( const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_round( const sfcgal_geometry_t*, int );
sfcgal_geometry_t*          sfcgal_geometry_minkowski_sum( const sfcgal_geometry_t*, const sfcgal_geometry_t* );
sfcgal_geometry_t*          sfcgal_geometry_offset_polygon( const sfcgal_geometry_t*, double radius );
sfcgal_geometry_t*          sfcgal_geometry_straight_skeleton( const sfcgal_geometry_t* );

/*--------------------------------------------------------------------------------------*
 *
 * Error handling
 *
 *--------------------------------------------------------------------------------------*/

/**
 * Warning and error handlers
 */

typedef int ( *sfcgal_error_handler_t ) ( const char *, ... );

void sfcgal_set_error_handlers( sfcgal_error_handler_t, sfcgal_error_handler_t );

/*--------------------------------------------------------------------------------------*
 *
 * Memory allocation
 *
 *--------------------------------------------------------------------------------------*/

typedef void* ( *sfcgal_alloc_handler_t ) (size_t);
typedef void ( *sfcgal_free_handler_t ) (void*);

void sfcgal_set_alloc_handlers( sfcgal_alloc_handler_t, sfcgal_free_handler_t );

/*--------------------------------------------------------------------------------------*
 *
 * Init
 *
 *--------------------------------------------------------------------------------------*/

void sfcgal_init();

#ifdef __cplusplus
}
#endif
#endif

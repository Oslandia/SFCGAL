/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_CAPI_H_
#define _SFCGAL_CAPI_H_

#include <SFCGAL/config.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO : return of errors ! => error handler

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

/**
 * sfcgal_geometry_t is an opaque pointer type that is used to represent a pointer to SFCGAL::Geometry
 * @ingroup capi
 */
typedef void sfcgal_geometry_t;

/**
 * Geometric types
 * @ingroup capi
 */
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

/**
 * Set the geometry validation mode
 * @ingroup capi
 * @note obsolete
 */
SFCGAL_C_API void                      sfcgal_set_geometry_validation( int enabled );

/**
 * Returns the type of a given geometry
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_type_t    sfcgal_geometry_type_id( const sfcgal_geometry_t* );

/**
 * Tests if the given geometry is valid or not
 * @ingroup capi
 */
SFCGAL_C_API int                       sfcgal_geometry_is_valid( const sfcgal_geometry_t* );

/**
 * Tests if the given geometry is valid or not
 * And return details in case of invalidity
 * @param geom the input geometry
 * @param invalidity_reason input/output parameter. If non null, a null-terminated string could be allocated and contain reason of the invalidity
 * @param invalidity_location input/output parameter. If non null, a geometry could be allocated and contain the location of the invalidity
 * @ingroup capi
 */
SFCGAL_C_API int                       sfcgal_geometry_is_valid_detail( const sfcgal_geometry_t* geom, char** invalidity_reason, sfcgal_geometry_t** invalidity_location );

/**
 * Tests if the given geometry is 3D or not
 * @ingroup capi
 */
SFCGAL_C_API int                       sfcgal_geometry_is_3d( const sfcgal_geometry_t* );

/**
 * Tests if the given geometry is empty or not
 * @ingroup capi
 */
SFCGAL_C_API int                       sfcgal_geometry_is_empty( const sfcgal_geometry_t* );

/**
 * Returns a deep clone of the given geometry
 * @post returns a pointer to an allocated geometry that must be deallocated by @ref sfcgal_geometry_delete
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_geometry_clone( const sfcgal_geometry_t* );

/**
 * Deletes a given geometry
 * @pre the given pointer must have been previously allocated by a creation function
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_geometry_delete( sfcgal_geometry_t* );

/**
 * Returns a WKT representation of the given geometry using CGAL exact integer fractions as coordinate values
 * @post buffer is returned allocated and must be freed by the caller
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_geometry_as_text( const sfcgal_geometry_t*, char** buffer, size_t* len );

/**
 * Returns a WKT representation of the given geometry using floating point coordinate values.
 * Floating point precision can be set via the numDecimals parameter.
 * Setting numDecimals to -1 yields the same result as sfcgal_geometry_as_text.
 * @post buffer is returned allocated and must be freed by the caller
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_geometry_as_text_decim( const sfcgal_geometry_t*, int numDecimals, char** buffer, size_t* len );

/**
 * Creates an empty point
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_point_create();

/**
 * Creates a point from two X and Y coordinates
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_point_create_from_xy( double x, double y );

/**
 * Creates a point from three X, Y and Z coordinates
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_point_create_from_xyz( double x, double y, double z );

/**
 * Returns the X coordinate of the given Point
 * @pre the given geometry must be a Point
 * @pre the given point must not be empty
 * @ingroup capi
 */
SFCGAL_C_API double                    sfcgal_point_x( const sfcgal_geometry_t* );

/**
 * Returns the Y coordinate of the given Point
 * @pre the given geometry must be a Point
 * @pre the given point must not be empty
 * @ingroup capi
 */
SFCGAL_C_API double                    sfcgal_point_y( const sfcgal_geometry_t* );

/**
 * Returns the Z coordinate of the given Point
 * @pre the given geometry must be a Point
 * @pre the given point must not be empty
 * @post the Z coordinate can value NaN if the given point is 2D only
 * @ingroup capi
 */
SFCGAL_C_API double                    sfcgal_point_z( const sfcgal_geometry_t* );

/**
 * Creates an empty LineString
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_linestring_create();

/**
 * Returns the number of points of the given LineString
 * @pre linestring must be a LineString
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_linestring_num_points( const sfcgal_geometry_t* linestring );

/**
 * Returns the ith point of a given LineString
 * @param i is the point index in the LineString
 * @pre linestring must be a LineString
 * @pre i >= and i < sfcgal_linestring_num_points
 * @post the returned Point is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_linestring_point_n( const sfcgal_geometry_t* linestring, size_t i );

/**
 * Adds a point to a LineString
 * @param linestring is the LineString where the Point has to be added to
 * @param point is the Point to add to the given LineString
 * @pre i >= and i < sfcgal_linestring_num_points
 * @post the ownership of Point is taken by the function
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_linestring_add_point( sfcgal_geometry_t* linestring, sfcgal_geometry_t* point );

/**
 * Creates an empty Triangle
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_triangle_create();

/**
 * Creates a Triangle from three given Point
 * @pre pta must be a Triangle
 * @pre ptb must be a Triangle
 * @pre ptc must be a Triangle
 * @post the ownership of the three points are not taken. The caller is still responsible of their deallocation
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_triangle_create_from_points( const sfcgal_geometry_t* pta,
        const sfcgal_geometry_t* ptb,
        const sfcgal_geometry_t* ptc );

/**
 * Returns one the Triangle's vertex as a Point
 * @pre triangle must be a Triangle
 * @pre i >= 0 and i < 3
 * @post returns a pointer to one of the vertices as a Point. This pointer is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_triangle_vertex( const sfcgal_geometry_t* triangle, int i );

/**
 * Sets one vertex of a Triangle
 * @pre triangle must be a Triangle
 * @pre vertex must be a Point
 * @post returns a pointer to one of the vertices as a Point. This pointer is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_triangle_set_vertex( sfcgal_geometry_t* triangle, int i, const sfcgal_geometry_t* vertex );

/**
 * Sets one vertex of a Triangle from two coordinates
 * @pre triangle must be a Triangle
 * @pre i >= 0 and i < 3
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_triangle_set_vertex_from_xy( sfcgal_geometry_t* triangle, int i, double x, double y );

/**
 * Sets one vertex of a Triangle from three coordinates
 * @pre triangle must be a Triangle
 * @pre i >= 0 and i < 3
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_triangle_set_vertex_from_xyz( sfcgal_geometry_t* triangle, int i, double x, double y, double z );

/**
 * Creates an empty Polygon
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_polygon_create();

/**
 * Creates an empty Polygon from an extrior ring
 * @pre ring must be a LineString
 * @post the ownership of the given ring is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_polygon_create_from_exterior_ring( sfcgal_geometry_t* ring );

/**
 * Returns the exterior ring of a given Polygon
 * @pre polygon must be a Polygon
 * @pre polygon must not be empty
 * @post the returned ring is a LineString, is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_polygon_exterior_ring( const sfcgal_geometry_t* polygon );

/**
 * Returns the number of interior rings of a given Polygon
 * @pre polygon must be a Polygon
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_polygon_num_interior_rings( const sfcgal_geometry_t* polygon );

/**
 * Returns the ith interior ring of a given Polygon
 * @pre polygon must be a Polygon
 * @pre i >= 0 and i < sfcgal_polygon_num_interior_rings
 * @post the returned ring is a LineString, is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_polygon_interior_ring_n( const sfcgal_geometry_t* polygon, size_t i );

/**
 * Adds an interior ring to a given Polygon
 * @pre polygon must be a Polygon
 * @pre ring must be a LineString
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_polygon_add_interior_ring( sfcgal_geometry_t* polygon, sfcgal_geometry_t* ring );

/**
 * Creates an empty  GeometryCollection
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_geometry_collection_create();

/**
 * Returns the number of geometries of a given GeometryCollection
 * @pre collection is a GeometryCollection
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_geometry_collection_num_geometries( const sfcgal_geometry_t* collection );

/**
 * Returns the ith geometry of a GeometryCollection
 * @pre collection is a GeometryCollection
 * @pre i >= 0 and i < sfcgal_geometry_collection_num_geometries
 * @post the returned Geometry is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_geometry_collection_geometry_n( const sfcgal_geometry_t* collection , size_t i );

/**
 * Adds a Geometry to a given GeometryCollection
 * @pre collection must be a GeometryCollection
 * @post the ownership of the given geometry is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_geometry_collection_add_geometry( sfcgal_geometry_t* collection, sfcgal_geometry_t* geometry );

/**
 * Creates an empty MultiPoint
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_multi_point_create();

/**
 * Creates an empty MultiLineString
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_multi_linestring_create();

/**
 * Creates an empty MultiPolygon
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_multi_polygon_create();

/**
 * Creates an empty PolyhedralSurface
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_polyhedral_surface_create();

/**
 * Returns the number of polygons of a given PolyhedralSurface
 * @pre polyhedral must be a PolyhedralSurface
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_polyhedral_surface_num_polygons( const sfcgal_geometry_t* polyhedral );

/**
 * Returns the ith polygon of a given PolyhedralSurface
 * @pre polyhedral must be a PolyhedralSurface
 * @pre i >= 0 and i < sfcgal_polyhedral_surface_num_polygons(polyhedral)
 * @post the returned Polygon is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_polyhedral_surface_polygon_n( const sfcgal_geometry_t* polyhedral, size_t i );

/**
 * Adds a Polygon to a given PolyhedralSurface
 * @pre polyhedral must be a PolyhedralSurface
 * @pre polygon must be a Polygon
 * @post the ownership of the Polygon is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_polyhedral_surface_add_polygon( sfcgal_geometry_t* polyhedral, sfcgal_geometry_t* polygon );

/**
 * Creates an empty TriangulatedSurface
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_triangulated_surface_create();

/**
 * Returns the number of triangles of a given TriangulatedSurface
 * @pre tin must be a TriangulatedSurface
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_triangulated_surface_num_triangles( const sfcgal_geometry_t* tin );

/**
 * Returns the ith Triangle of a given TriangulatedSurface
 * @pre tin must be a TriangulatedSurface
 * @pre i >= 0 and i < sfcgal_triangulated_surface_num_triangles( tin )
 * @post the returned Triangle is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_triangulated_surface_triangle_n( const sfcgal_geometry_t* tin, size_t i );

/**
 * Adds a Triangle to a given TriangulatedSurface
 * @pre tin must be a TriangulatedSurface
 * @pre triangle must be a Triangle
 * @post the ownership of the Triangle is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_triangulated_surface_add_triangle( sfcgal_geometry_t* tin, sfcgal_geometry_t* triangle );

/**
 * Creates an empty Solid
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_solid_create();

/**
 * Creates a Solid from an exterior shell
 * @pre ring must be a PolyhedralSurface
 * @post the ownership of the given shell is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*        sfcgal_solid_create_from_exterior_shell( sfcgal_geometry_t* shell );

/**
 * Returns the number of shells of a given Solid
 * @pre solid must be a Solid
 * @ingroup capi
 */
SFCGAL_C_API size_t                    sfcgal_solid_num_shells( const sfcgal_geometry_t* solid );

/**
 * Returns the ith shell of a given Solid
 * @pre solid must be a Solid
 * @pre i >= 0 and i < sfcgal_solid_num_shells( tin )
 * @post the returned PolyhedralSurface is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*  sfcgal_solid_shell_n( const sfcgal_geometry_t* solid, size_t i );

/**
 * Adds a shell to a given Solid
 * @pre solid must be a Solid
 * @pre shell must be a PolyhedralSurface
 * @post the ownership of the shell is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API void                      sfcgal_solid_add_shell( sfcgal_geometry_t* solid, sfcgal_geometry_t* shell );


/**
 * Gets the validity flag of the geometry.
 */
int sfcgal_geometry_has_validity_flag( const sfcgal_geometry_t* geom );

/**
 * Sets the validity flag of the geometry.
 * FIXME We better have geometry constructors to directly build valid geometries
 */
void sfcgal_geometry_force_valid( sfcgal_geometry_t* geom, int valid );

/*--------------------------------------------------------------------------------------*
 *
 * Support for SFCGAL::PreparedGeometry
 *
 *--------------------------------------------------------------------------------------*/

/**
 * Opaque type that represents the C++ type SFCGAL::PreparedGeometry
 * @ingroup capi
 */
typedef void sfcgal_prepared_geometry_t;

typedef uint32_t srid_t;

/**
 * Creates an empty PreparedGeometry
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create();

/**
 * Creates a PreparedGeometry from a Geometry and an SRID
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_prepared_geometry_t* sfcgal_prepared_geometry_create_from_geometry( sfcgal_geometry_t* geometry, srid_t srid );

/**
 * Deletes a given PreparedGeometry
 * @pre prepared must be a PreparedGeometry
 * @post the underlying Geometry linked to the given PreparedGeometry is also deleted
 * @ingroup capi
 */
SFCGAL_C_API void                        sfcgal_prepared_geometry_delete( sfcgal_prepared_geometry_t* prepared );

/**
 * Returns the Geometry associated with a given PreparedGeometry
 * @pre prepared must be a PreparedGeometry
 * @post the returned Geometry is not writable and must not be deallocated by the caller
 * @ingroup capi
 */
SFCGAL_C_API const sfcgal_geometry_t*    sfcgal_prepared_geometry_geometry( const sfcgal_prepared_geometry_t* prepared );

/**
 * Sets the Geometry associated with the given PreparedGeometry
 * @pre prepared must be a PreparedGeometry
 * @post the ownership of the given geometry is taken. The caller is not responsible anymore of its deallocation
 * @ingroup capi
 */
SFCGAL_C_API void                        sfcgal_prepared_geometry_set_geometry( sfcgal_prepared_geometry_t* prepared, sfcgal_geometry_t* geometry );

/**
 * Returns SRID associated with a given PreparedGeometry
 * @pre prepared must be a PreparedGeometry
 * @ingroup capi
 */
SFCGAL_C_API srid_t                      sfcgal_prepared_geometry_srid( const sfcgal_prepared_geometry_t* prepared );

/**
 * Sets SRID associated with a given PreparedGeometry
 * @pre prepared must be a PreparedGeometry
 * @ingroup capi
 */
SFCGAL_C_API void                        sfcgal_prepared_geometry_set_srid( sfcgal_prepared_geometry_t* prepared, srid_t );

/**
 * Returns an EWKT representation of the given PreparedGeometry
 * @param num_decimals number of decimals. -2 for a variable number of decimals. -1 for an exact representation
 * @post buffer is returned allocated and must be freed by the caller
 * @ingroup capi
 */
SFCGAL_C_API void                        sfcgal_prepared_geometry_as_ewkt( const sfcgal_prepared_geometry_t* prepared, int num_decimals, char** buffer, size_t* len );

/*--------------------------------------------------------------------------------------*
 *
 * I/O functions
 *
 *--------------------------------------------------------------------------------------*/

/**
 * io::readWKT
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_io_read_wkt( const char*, size_t len );
SFCGAL_C_API sfcgal_prepared_geometry_t* sfcgal_io_read_ewkt( const char*, size_t len );

/**
 * Serialization
 */
/* allocates into char**, must be freed by the caller */
SFCGAL_C_API void                        sfcgal_io_write_binary_prepared( const sfcgal_prepared_geometry_t*, char**, size_t* );
SFCGAL_C_API sfcgal_prepared_geometry_t* sfcgal_io_read_binary_prepared( const char*, size_t l );

/*--------------------------------------------------------------------------------------*
 *
 * Spatial processing
 *
 *--------------------------------------------------------------------------------------*/

/**
 * Tests the intersection of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @ingroup capi
 */
SFCGAL_C_API int                         sfcgal_geometry_intersects( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Tests the 3D intersection of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @ingroup capi
 */
SFCGAL_C_API int                         sfcgal_geometry_intersects_3d( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the intersection of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_intersection( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the 3D intersection of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_intersection_3d( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the difference of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_difference( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the 3D difference of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_difference_3d( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the union of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_union( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the 3D union of geom1 and geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_union_3d( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the convex hull of geom
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_convexhull( const sfcgal_geometry_t* geom );

/**
 * Returns the 3D convex hull of geom
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_convexhull_3d( const sfcgal_geometry_t* geom );

/**
 * Returns the volume of geom (must be a volume)
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API double                      sfcgal_geometry_volume( const sfcgal_geometry_t* geom );

/**
 * Returns the area of geom
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API double                      sfcgal_geometry_area( const sfcgal_geometry_t* geom );

/**
 * Returns the 3D area of geom
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API double                      sfcgal_geometry_area_3d( const sfcgal_geometry_t* geom );

/**
 * Tests if the given Geometry is planar
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API int                         sfcgal_geometry_is_planar( const sfcgal_geometry_t* geom );

/**
 * Returns the orientation of the given Polygon
 * -1 for a counter clockwise orientation
 * 1 for a clockwise orientation
 * 0 for an invalid or undetermined orientation
 * @pre geom is a Polygon
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API int                         sfcgal_geometry_orientation( const sfcgal_geometry_t* geom );

/**
 * Returns a tesselation of the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_tesselate( const sfcgal_geometry_t* geom );

/**
 * Returns a triangulation of the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_triangulate_2dz( const sfcgal_geometry_t* geom );

/**
 * Returns an extrusion of the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_extrude( const sfcgal_geometry_t* geom, double ex, double ey, double ez );

/**
 * Convert a PolyhedralSurface to a Solid
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup detail
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_make_solid( const sfcgal_geometry_t* geom );

/**
 * Force a Left Handed Rule on the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_force_lhr( const sfcgal_geometry_t* geom );

/**
 * Force a Right Handed Rule on the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_force_rhr( const sfcgal_geometry_t* geom );

/**
 * Computes the distance of the two given Geometry objects
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @ingroup capi
 */
SFCGAL_C_API double                      sfcgal_geometry_distance( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Computes the 3D distance of the two given Geometry objects
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @ingroup capi
 */
SFCGAL_C_API double                      sfcgal_geometry_distance_3d( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Round coordinates of the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_round( const sfcgal_geometry_t* geom, int r );

/**
 * Returns the minkowski sum geom1 + geom2
 * @pre isValid(geom1) == true
 * @pre isValid(geom2) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_minkowski_sum( const sfcgal_geometry_t* geom1, const sfcgal_geometry_t* geom2 );

/**
 * Returns the offset polygon of the given Geometry.
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_offset_polygon( const sfcgal_geometry_t* geom, double radius );

/**
 * Returns the straight skeleton of the given Geometry
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_straight_skeleton( const sfcgal_geometry_t* geom );

/**
 * Returns the straight skeleton of the given Geometry with the distance to the border as M coordinate
 * @pre isValid(geom) == true
 * @post isValid(return) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_straight_skeleton_distance_in_m( const sfcgal_geometry_t* geom );

/**
 * Returns the approximate medial axis for the given Polygon
 * Approximate medial axis is based on straight skeleton
 * @pre isValid(geom) == true
 * @ingroup capi
 */
SFCGAL_C_API sfcgal_geometry_t*          sfcgal_geometry_approximate_medial_axis( const sfcgal_geometry_t* geom );

/*--------------------------------------------------------------------------------------*
 *
 * Error handling
 *
 *--------------------------------------------------------------------------------------*/

/**
 * Warning and error handlers
 * @ingroup capi
 */
typedef int ( *sfcgal_error_handler_t ) ( const char*, ... );

/**
 * Sets the error handlers. These callbacks are called on warning or error
 * @param warning_handler is the printf-styled callback function that will be called when a function raises a warning.
 * The default behaviour is to call printf.
 * @param error_handler is the printf-style callback function that will be called when a function generates an error.
 * The default behaviour is to call printf.
 * @ingroup capi
 */
SFCGAL_C_API void sfcgal_set_error_handlers( sfcgal_error_handler_t warning_handler, sfcgal_error_handler_t error_handler );

/*--------------------------------------------------------------------------------------*
 *
 * Memory allocation
 *
 *--------------------------------------------------------------------------------------*/

typedef void* ( *sfcgal_alloc_handler_t ) ( size_t );
typedef void ( *sfcgal_free_handler_t ) ( void* );

/**
 * Sets the error handlers. These callbacks are called on warning or error
 * @param malloc_handler is the function to call for memory allocation. The default behaviour is to call malloc()
 * @param free_handler is the function to call for memory deallocation. The default behaviour is to call free()
 * @ingroup capi
 */
SFCGAL_C_API void sfcgal_set_alloc_handlers( sfcgal_alloc_handler_t malloc_handler, sfcgal_free_handler_t free_handler );

/*--------------------------------------------------------------------------------------*
 *
 * Init
 *
 *--------------------------------------------------------------------------------------*/

/**
 * This function must be called before all the other one.
 * @ingroup capi
 */
SFCGAL_C_API void sfcgal_init();

/**
 * Get version
 * @ingroup capi
 */
SFCGAL_C_API const char* sfcgal_version();

#ifdef __cplusplus
}
#endif
#endif

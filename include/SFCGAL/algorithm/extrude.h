#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * extrude a Geometry with a direction
	 * @todo GeometryCollection, PolyhedralSurface, TriangulatedSurface (require boundary)
	 */
	std::auto_ptr< Geometry > extrude( const Geometry & g, double dx, double dy, double dz ) ;

	void       translate( Geometry & g, double dx, double dy, double dz ) ;

	LineString *          extrude( const Point & g, double dx, double dy, double dz ) ;
	PolyhedralSurface *   extrude( const LineString & g, double dx, double dy, double dz ) ;
	Solid *               extrude( const Polygon & g, double dx, double dy, double dz ) ;
	Solid *               extrude( const Triangle & g, double dx, double dy, double dz ) ;

	MultiLineString *     extrude( const MultiPoint & g, double dx, double dy, double dz ) ;
	PolyhedralSurface *   extrude( const MultiLineString & g, double dx, double dy, double dz ) ;
	MultiSolid *          extrude( const MultiPolygon & g, double dx, double dy, double dz ) ;

	/**
	 * @warning suppose that the TriangulatedSurface is connected
	 * @todo take orientation in account
	 */
	Solid *               extrude( const TriangulatedSurface & g, double dx, double dy, double dz ) ;
	/**
	 * @warning doesn't take orientation in account
	 * @todo take orientation in account
	 */
	Solid *               extrude( const PolyhedralSurface & g, double dx, double dy, double dz ) ;

	/**
	 * extrude each geometry in a GeometryCollection
	 */
	GeometryCollection*   extrude( const GeometryCollection & g, double dx, double dy, double dz ) ;

}//algorithm
}//SFCGAL

#endif

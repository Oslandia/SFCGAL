#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * extrude a Geometry with a direction
	 * @todo GeometryCollection, PolyhedralSurface, TriangulatedSurface (require boundary)
	 */
	Geometry * extrude( const Geometry & g, double dx, double dy, double dz ) ;

	void       translate( Geometry & g, double dx, double dy, double dz ) ;

	LineString *          extrude( const Point & g, double dx, double dy, double dz ) ;
	PolyhedralSurface *   extrude( const LineString & g, double dx, double dy, double dz ) ;
	Solid *               extrude( const Polygon & g, double dx, double dy, double dz ) ;
	Solid *               extrude( const Triangle & g, double dx, double dy, double dz ) ;

	MultiLineString *     extrude( const MultiPoint & g, double dx, double dy, double dz ) ;
	PolyhedralSurface *   extrude( const MultiLineString & g, double dx, double dy, double dz ) ;
	MultiSolid *          extrude( const MultiPolygon & g, double dx, double dy, double dz ) ;

}//algorithm
}//SFCGAL

#endif

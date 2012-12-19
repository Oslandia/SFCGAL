#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * extrude a Geometry with a direction
	 * @todo GeometryCollection, PolyhedralSurface, TriangulatedSurface (require boundary)
	 */
	std::auto_ptr< Geometry > extrude( const Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

	void       translate( Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

	LineString *          extrude( const Point & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	PolyhedralSurface *   extrude( const LineString & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	Solid *               extrude( const Polygon & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	Solid *               extrude( const Triangle & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

	MultiLineString *     extrude( const MultiPoint & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	PolyhedralSurface *   extrude( const MultiLineString & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	MultiSolid *          extrude( const MultiPolygon & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

	/**
	 * @warning suppose that the TriangulatedSurface is connected
	 * @todo take orientation in account
	 */
	Solid *               extrude( const TriangulatedSurface & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;
	/**
	 * @warning doesn't take orientation in account
	 * @todo take orientation in account
	 */
	Solid *               extrude( const PolyhedralSurface & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

	/**
	 * extrude each geometry in a GeometryCollection
	 */
	GeometryCollection*   extrude( const GeometryCollection & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

}//algorithm
}//SFCGAL

#endif

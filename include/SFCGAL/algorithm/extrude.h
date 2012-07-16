#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	Geometry * extrude( const Geometry & g, double dx, double dy, double dz ) ;

	void       translate( Geometry & g, double dx, double dy, double dz ) ;

	LineString *          extrude( const Point & g, double dx, double dy, double dz ) ;
	PolyhedralSurface *   extrude( const LineString & g, double dx, double dy, double dz ) ;

	/**
	 * @todo manage top/bottom orientation according to the extrusion direction and the polygon
	 * normal. (0,0,-1) would lead to bad orientation.
	 */
	Solid *               extrude( const Polygon & g, double dx, double dy, double dz ) ;

}//algorithm
}//SFCGAL

#endif
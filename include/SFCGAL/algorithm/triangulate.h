#ifndef _SFCGAL_ALGORITHM_TRIANGULATE_H_
#define _SFCGAL_ALGORITHM_TRIANGULATE_H_

namespace SFCGAL {
	class Polygon ;
	class MultiPolygon ;
	class TriangulatedSurface ;
}

namespace SFCGAL {
namespace algorithm {

	/**
	 * Convert a 2D/3D polygon to a TriangulatedSurface.
	 * @todo Project points according to the normal
	 * @warning suppose no null segments!
	 */
	void triangulate( const Polygon & polygon, TriangulatedSurface & triangulatedSurface ) ;

	/**
	 * Convert a 2D/3D MultiPolygon to a TriangulatedSurface. Each polygon is triangulated invidualy
	 * @warning suppose no null segments!
	 */
	void triangulate( const MultiPolygon & polygon, TriangulatedSurface & triangulatedSurface ) ;

}//algorithm
}//algorithm



#endif


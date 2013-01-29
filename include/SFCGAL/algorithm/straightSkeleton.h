#ifndef _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_
#define _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_

#include <memory>

namespace SFCGAL {
	class Geometry ;
	class Polygon ;
	class MultiPolygon ;
	class MultiLineString ;
}

namespace SFCGAL {
namespace algorithm {
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g ) ;
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const Polygon& g ) ;
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const MultiPolygon& g ) ;

}//namespace algorithm
}//namespace SFCGAL


#endif

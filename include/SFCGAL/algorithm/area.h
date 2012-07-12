#ifndef _SFCGAL_ALGORITHM_AREA_H_
#define _SFCGAL_ALGORITHM_AREA_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns 3D area for a Geometry
	 */
	double area3D( const Geometry & g ) ;

	/**
	 * Returns 2D area for a Geometry
	 */
	double area2D( const Geometry & g ) ;

}//algorithm
}//SFCGAL


#endif

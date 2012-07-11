#ifndef _SFCGAL_ALGORITHM_ORIENTATION_H_
#define _SFCGAL_ALGORITHM_ORIENTATION_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Try to bring consistency in the polygon orientation
	 */
	void makeConsistentOrientation( TriangulatedSurface & geometry ) ;

}//algorithm
}//SFCGAL


#endif

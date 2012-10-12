#ifndef _SFCGAL_DETAIL_COORDINATESTORAGE_H_
#define _SFCGAL_DETAIL_COORDINATESTORAGE_H_

#include <boost/variant.hpp>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
namespace detail {


	typedef struct {} Empty ;

	/**
	 * Coordinate storage based on the Kernel. This allows to keep
	 * an Epeck representation of Coordinate throw geometry processing.
	 */
	typedef boost::variant< Empty, Kernel::Point_2, Kernel::Point_3 > CoordinateStorage ;

	/**
	 * test if coordinateStorage is empty
	 */
	bool isEmpty( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * test if coordinateStorage is 3D
	 */
	bool is3D( const CoordinateStorage & coordinateStorage ) ;

	/**
	 * Convert coordinate storage to Point_2
	 */
	Kernel::Point_2 toPoint_2( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * Convert coordinate storage to Point_3
	 */
	Kernel::Point_3 toPoint_3( const CoordinateStorage & coordinateStorage ) ;


}//namespace detail
}//namespace SFCGAL

#endif

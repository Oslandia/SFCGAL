#ifndef _SFCGAL_DETAIL_COORDINATESTORAGE_H_
#define _SFCGAL_DETAIL_COORDINATESTORAGE_H_

#include <boost/variant.hpp>

#include <SFCGAL/Kernel.h>

#include <CGAL/Vector_2.h>
#include <CGAL/Vector_3.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>

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
	 * X accessor, NaN if not defined
	 */
	Kernel::FT x( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * Y accessor, NaN if not defined
	 */
	Kernel::FT y( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * Z accessor, NaN if not defined
	 */
	Kernel::FT z( const CoordinateStorage & coordinateStorage ) ;

	/**
	 * Convert coordinate storage to Point_2
	 */
	Kernel::Point_2 toPoint_2( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * Convert coordinate storage to Point_3
	 */
	Kernel::Point_3 toPoint_3( const CoordinateStorage & coordinateStorage ) ;


	/**
	 * Convert coordinate storage to Vector_2
	 */
	Kernel::Vector_2 toVector_2( const CoordinateStorage & coordinateStorage ) ;
	/**
	 * Convert coordinate storage to Vector_3
	 */
	Kernel::Vector_3 toVector_3( const CoordinateStorage & coordinateStorage ) ;

}//namespace detail
}//namespace SFCGAL

#endif

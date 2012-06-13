#ifndef _SFCGAL_KERNEL_H_
#define _SFCGAL_KERNEL_H_

#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>


namespace SFCGAL {
	/**
	 * Defines the default Kernel used by SFCGAL
	 * @todo allow to choose between differents Kernels
	 */
	typedef CGAL::Cartesian< double > Kernel ;

	/**
	 * Defines a 2D mathematical vector
	 */
	typedef Kernel::Vector_2          Vector_2 ;
	/**
	 * Defines a 3D mathematical vector
	 */
	typedef Kernel::Vector_3          Vector_3 ;

	/**
	 * Defines the Point_2 in the default Kernel
	 */
	typedef Kernel::Point_2           Point_2 ;
	/**
	 * Defines the Point_3 in the default Kernel
	 */
	typedef Kernel::Point_3           Point_3 ;

	/**
	 * Defines a Segment_2 in the default Kernel
	 */
	typedef CGAL::Segment_2< Kernel > Segment_2 ;
	/**
	 * Defines a Segment_3 in the default Kernel
	 */
	typedef CGAL::Segment_3< Kernel > Segment_3 ;

	/**
	 * A polygon defined by an exterior ring
	 */
	typedef CGAL::Polygon_2< Kernel >            Polygon_2 ;
	/**
	 * A polygon defined by an exterior ring and interior rings
	 */
	typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;
}//SFCGAL



#endif

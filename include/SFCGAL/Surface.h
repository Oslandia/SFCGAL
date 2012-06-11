#ifndef _SFCGAL_SURFACE_H_
#define _SFCGAL_SURFACE_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

	/**
	 * Abstract Surface class
	 *
	 * @todo template < size_t N >?
	 */
	class Surface : public Geometry {
	public:
		/**
		 * destructor
		 */
		virtual ~Surface() ;

		//-- SFCGAL::Geometry
		virtual int            dimension() const ;

		/**
		 * [OGC/SFS]"The area of this Surface, as measured in the spatial reference system of this Surface"
		 */
		//virtual double area() const = 0 ;
		/**
		 * [OGC/SFS]"The mathematical centroid for this Surface as a Point. The result in not guaranteed to
		 * be on this Surface"
		 */
		//virtual Point centroid() const = 0 ;
		/**
		 * [OGC/SFS]"A Point guaranteed to be on this Surface"
		 * @warning empty point is isEmpty()
		 */
		//virtual Point pointOnSurface() const = 0 ;
	protected:
		/**
		 * no default constructor
		 */
		Surface() ;
		/**
		 * no copy constructor
		 */
		Surface( Surface const& other ) ;
	};


}

#endif

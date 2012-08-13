#ifndef _SFCGAL_ENVELOPE_H_
#define _SFCGAL_ENVELOPE_H_

#include <boost/assert.hpp>
#include <SFCGAL/detail/Interval.h>

#include <SFCGAL/Coordinate.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>

namespace SFCGAL {

	/**
	 * Represents a bounding box
	 */
	class Envelope {
	public:
		/**
		 * default constructor (empty bounding box)
		 */
		Envelope() ;
		/**
		 * 2D box constructor with min,max values
		 */
		Envelope(
			const double & xmin, const double & xmax,
			const double & ymin, const double & ymax
		) ;
		/**
		 * 3D box constructor with min,max values
		 */
		Envelope(
			const double & xmin, const double & xmax,
			const double & ymin, const double & ymax,
			const double & zmin, const double & zmax
		) ;
		/**
		 * default constructor (empty bounding box)
		 */
		Envelope( const Coordinate & p ) ;
		/**
		 * default constructor (empty bounding box)
		 */
		Envelope( const Coordinate & p1, const Coordinate & p2 ) ;
		/**
		* copy constructor
		*/
		Envelope( const Envelope& other ) ;
		/**
		* assign operator
		*/
		Envelope& operator = ( const Envelope& other ) ;
		/**
		 * destructor
		 */
		~Envelope() ;

		/**
		 * indicates if the bounding box is empty
		 */
		bool isEmpty() const ;
		/**
		 * indicates if the bounding box has a 3D component
		 */
		bool is3D() const ;

		/**
		 * expand the box to include coordinate
		 */
		void expandToInclude( const Coordinate & coordinate ) ;


		inline const double& xMin() const { return _bounds[0].lower(); }
		inline const double& yMin() const { return _bounds[1].lower(); }
		inline const double& zMin() const { return _bounds[2].lower(); }

		inline const double& xMax() const { return _bounds[0].upper(); }
		inline const double& yMax() const { return _bounds[1].upper(); }
		inline const double& zMax() const { return _bounds[2].upper(); }


		/**
		 * returns the n-th bound
		 */
		inline detail::Interval & boundsN( const size_t & n ) {
			BOOST_ASSERT( n < 3 );
			return _bounds[n] ;
		}
		/**
		 * returns the n-th bound
		 */
		inline const detail::Interval & boundsN( const size_t & n ) const {
			BOOST_ASSERT( n < 3 );
			return _bounds[n] ;
		}


		/**
		 * Convenience function. Convert to CGAL::BBox_2
		 */
		inline CGAL::Bbox_2 toBbox_2() {
			BOOST_ASSERT( ! isEmpty() );

			return CGAL::Bbox_2(
				_bounds[0].lower(), _bounds[1].lower(),
				_bounds[0].upper(), _bounds[1].upper()
			);
		}

		/**
		 * Convenience function. Convert to CGAL::BBox_3
		 */
		inline CGAL::Bbox_3 toBbox_3() {
			BOOST_ASSERT( is3D() );

			return CGAL::Bbox_3(
					_bounds[0].lower(), _bounds[1].lower(), _bounds[2].lower(),
					_bounds[0].upper(), _bounds[1].upper(), _bounds[2].upper()
			);
		}



	private:
		/**
		 * bounds of the interval ((xmin,xmax),(ymin,ymax),(zmin,zmax))
		 */
		detail::Interval _bounds[3] ;
	};


}//SFCGAL

#endif

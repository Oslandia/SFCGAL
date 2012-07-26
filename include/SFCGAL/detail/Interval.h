#ifndef _SFCGAL_DETAIL_INTERVAL_H_
#define _SFCGAL_DETAIL_INTERVAL_H_

namespace SFCGAL {
namespace detail {

	/**
	 * Represents an interval
	 */
	class Interval {
	public:
		/**
		 * default constructor (empty interval)
		 */
		Interval() ;
		/**
		 * collapsed interval constructor
		 */
		Interval( const double & value ) ;
		/**
		 * constructor with two values
		 */
		Interval( const double & v1, const double & v2 ) ;
		/**
		 * copy constructor
		 */
		Interval( const Interval & other ) ;
		/**
		 * assign operator
		 */
		Interval& operator = ( const Interval & other ) ;
		/**
		 * destructor
		 */
		~Interval() ;

		/**
		 * indicates if the interval is empty
		 */
		bool isEmpty() const ;


		/**
		 * returns the lower value
		 */
		inline const double & lower() const { return _lower; }
		/**
		 * returns the upper value
		 */
		inline const double & upper() const { return _upper; }
		/**
		 * returns the with of the interval
		 */
		inline double         width() const { return _upper - _lower ; }

		/**
		 * expand the interval
		 *
		 * @warning no effect if isEmpty()
		 */
		void                  expandBy( const double & d ) ;
		/**
		 * expand the interval to include an other interval.
		 *
		 * @warning no effect if other.isEmpty()
		 */
		void                  expandToInclude( const Interval & other ) ;
		/**
		 * expand the interval to include a value
		 *
		 * @warning no effect if value is NaN
		 */
		void                  expandToInclude( const double & value ) ;


		/**
		 * test if this intersects other
		 */
		bool                  intersects( const Interval & other ) const  ;


		/**
		 * compare two intervals
		 * @warning true for empty intervals
		 */
		bool operator == ( const Interval& other ) const ;
		/**
		 * compare two intervals
		 * @warning false for empty intervals
		 */
		bool operator != ( const Interval& other ) const ;

	private:
		double _lower ;
		double _upper ;
	};

}//detail
}//SFCGAL

#endif

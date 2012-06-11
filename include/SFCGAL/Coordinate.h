#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <sstream>

#include <boost/numeric/ublas/vector.hpp>

namespace SFCGAL {

	/**
	 * mathematical vector supporting based on boost::ublas representing
	 * Point coordinates.
	 */
	typedef boost::numeric::ublas::vector< double > Coordinate ;

	/**
	 * Write coordinates as text
	 */
	void writeText( std::ostream & s, const Coordinate & coordinate ) ;

}//SFCGAL




#endif

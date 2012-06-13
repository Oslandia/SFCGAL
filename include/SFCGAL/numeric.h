#ifndef _SFCGAL_NUMERIC_H_
#define _SFCGAL_NUMERIC_H_

#include <limits>

namespace SFCGAL {
	/**
	 * shortcut to get NaN for double
	 */
	inline double NaN() { return std::numeric_limits< double >::quiet_NaN(); }
	/**
	 * shortcut to test NaN for double
	 */
	inline bool   isNaN( const double & value ){ return value != value ; }


}//SFCGAL

#endif

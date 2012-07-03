#ifndef _SFCGAL_DETAIL_COMPLEXCOMPARATOR_H_
#define _SFCGAL_DETAIL_COMPLEXCOMPARATOR_H_

#include <complex>

namespace SFCGAL {
namespace detail {

	/**
	 * lexicographic order on complex
	 */
	struct ComplexComparator {
		template < typename T >
		inline bool operator () ( const std::complex< T > & a, const std::complex< T > & b ){
			return ( a.real() < b.real() ) || ( a.real() == b.real() && a.imag() < b.imag() );
		}
	};


}//detail
}//SFCGAL


#endif

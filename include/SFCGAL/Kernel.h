#ifndef _SFCGAL_KERNEL_H_
#define _SFCGAL_KERNEL_H_

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <boost/variant.hpp>

namespace SFCGAL {

	/**
	 * default Kernel
	 */
	typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel ;

}//namespace SFCGAL

#endif

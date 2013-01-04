#ifndef _SFCGAL_KERNEL_H_
#define _SFCGAL_KERNEL_H_

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

namespace SFCGAL {

	/**
	 * default Kernel
	 */
	typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel ;


	/**
	 * Quotient type
	 */
	typedef CGAL::Gmpq QT;

}//namespace SFCGAL

#endif

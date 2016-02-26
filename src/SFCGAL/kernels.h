#ifndef _SFCGAL_KERNELS_H_
#define _SFCGAL_KERNELS_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian_converter.h>

namespace SFCGAL {
    
    typedef CGAL::Exact_predicates_exact_constructions_kernel   Epeck ;
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Epick ;

}

#endif

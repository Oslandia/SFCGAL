#ifndef _SFCGAL_DETAIL_COLLECTPOINTSANDCONSTRAINTS_H_
#define _SFCGAL_DETAIL_COLLECTPOINTSANDCONSTRAINTS_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/detail/PointsAndConstraintsVisitor.h>

namespace SFCGAL {
namespace detail {
    
    /**
     * Collect Points and Constraints from a geometry.
     *
     * @warning this methods does not ensure unicity
     */
    template < typename K >
    void collectPointsAndConstraints(
        const Geometry<K> & geometry,
        std::vector< Point<K> > & points,
        std::vector< std::pair< size_t, size_t > > & constraints
    ){
        detail::PointsAndConstraintsVisitor<K> visitor(points,constraints);
        boost::apply_visitor(visitor,geometry);
    }
        
} // detail
} // SFCGAL

#endif

#ifndef _SFCGAL_DETAIL_POLYGONSETTOMULTIPOLYGON_H_
#define _SFCGAL_DETAIL_POLYGONSETTOMULTIPOLYGON_H_

#include <CGAL/Polygon_set_2.h>
#include <SFCGAL/MultiPolygon.h>

namespace SFCGAL {
namespace detail {

/**
 * @brief convert a CGAL::Polygon_set_2 to a MultiPolygon
 */
std::auto_ptr< MultiPolygon > polygonSetToMultiPolygon( const CGAL::Polygon_set_2< Kernel > & polygonSet ) ;

} // namespace detail
} // namespace SFCGAL


#endif

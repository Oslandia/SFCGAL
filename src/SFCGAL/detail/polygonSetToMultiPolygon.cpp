#include <SFCGAL/detail/polygonSetToMultiPolygon.h>

#include <CGAL/Polygon_with_holes_2.h>

#include <list>

namespace SFCGAL {
namespace detail {

///
///
///
std::auto_ptr< MultiPolygon > polygonSetToMultiPolygon( const CGAL::Polygon_set_2< Kernel > & polygonSet )
{
	typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;

	std::list<Polygon_with_holes_2> res;
	polygonSet.polygons_with_holes( std::back_inserter(res) ) ;

	std::auto_ptr< MultiPolygon > result( new MultiPolygon );
	for ( std::list<Polygon_with_holes_2>::const_iterator it = res.begin(); it != res.end(); ++it ){
		result->addGeometry( new Polygon( *it ) );
	}
	return result ;
}

} // namespace detail
} // namespace SFCGAL

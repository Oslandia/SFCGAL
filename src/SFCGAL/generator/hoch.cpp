#include <SFCGAL/generator/hoch.h>

#include <SFCGAL/all.h>

namespace SFCGAL {
namespace generator {

std::vector< Kernel::Vector_2 > _hoch( const std::vector< Kernel::Vector_2 > & points ){
	std::vector< Kernel::Vector_2 > result ;
	result.reserve( points.size() * 2 ) ;
	size_t numPoints = points.size() ;
	for ( size_t i = 0; i < numPoints; i++ ){
		const Kernel::Vector_2& a = points[ i ] ;
		const Kernel::Vector_2& b = points[ (i + 1) % numPoints ] ;

		Kernel::Vector_2 ab = b - a ;
		Kernel::Vector_2 normal( -ab.y(), ab.x() );

		result.push_back( a );
		result.push_back( a + ab / 3 );
		result.push_back( a + ab / 2 + normal * sqrt(3.0) / 6.0 );
		result.push_back( a + (ab * 2)/3 );
	}
	return result ;
}

///
///
///
std::auto_ptr< Polygon > hoch( const unsigned int & order )
{
	std::vector< Kernel::Vector_2 > points ;
	points.push_back( Kernel::Vector_2( 1.0, sqrt(3.0) ) );
	points.push_back( Kernel::Vector_2( 2.0, 0.0 ) );
	points.push_back( Kernel::Vector_2( 0.0, 0.0 ) );


	for ( unsigned int k = 0 ; k < order; k++ ){
		points = _hoch( points );
	}

	std::auto_ptr< Polygon > result( new Polygon() ) ;
	std::auto_ptr< LineString > ring( new LineString() ) ;
	for ( std::vector< Kernel::Vector_2 >::const_iterator it = points.begin(); it != points.end(); ++it ){
		ring->addPoint( new Point( it->x(), it->y() ) ) ;
	}
	ring->addPoint( new Point( points.front().x(), points.front().y() ) ) ;

	result->setExteriorRing( ring.release() );

	return result ;
}

} // namespace generator
} // namespace SFCGAL


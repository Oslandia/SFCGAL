#include <SFCGAL/generator/disc.h>
#include <SFCGAL/all.h>

namespace SFCGAL {
namespace generator {

///
///
///
std::auto_ptr< Polygon > disc(
	const Point& center,
	const double & radius,
	const unsigned int & nQuadrantSegments
)
{
	BOOST_ASSERT( nQuadrantSegments > 1 );

	std::auto_ptr< LineString > exteriorRing( new LineString() ) ;

	double dTheta = M_PI_4 / nQuadrantSegments ;
	for ( size_t i = 0; i < nQuadrantSegments * 4; i++ ){
		Kernel::Vector_2 p = center.toVector_2() + Kernel::Vector_2( cos(i*dTheta), sin(i*dTheta) ) ;
		exteriorRing->addPoint( new Point( p.x(), p.y() ) ) ;
	}
	exteriorRing->addPoint( exteriorRing->startPoint() ) ;

	return std::auto_ptr< Polygon >( new Polygon( exteriorRing.release() ) );
}

} // namespace generator
} // namespace SFCGAL


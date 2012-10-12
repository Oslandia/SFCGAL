#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/transform/AffineTransform2.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_transform_AffineTransform2Test )

BOOST_AUTO_TEST_CASE( simpleTranslate )
{
	LineString g( Point(0.0,0.0), Point(1.0,1.0) ) ;

	CGAL::Aff_transformation_2< Kernel > affine(
			CGAL::TRANSLATION,
			CGAL::Vector_2< Kernel >( 1.0,2.0 )
	);

	transform::AffineTransform2< Kernel > transform( affine ) ;
	g.accept(transform );

	BOOST_CHECK_EQUAL( g.pointN(0).asText(5), "POINT(1.00000 2.00000)" );
	BOOST_CHECK_EQUAL( g.pointN(1).asText(5), "POINT(2.00000 3.00000)" );
}



BOOST_AUTO_TEST_SUITE_END()





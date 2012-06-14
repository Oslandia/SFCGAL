#include <boost/test/unit_test.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>

#include <cmath>

#include <CGAL/Aff_transformation_3.h>

typedef CGAL::Cartesian< double > Kernel ;
typedef Kernel::Vector_2          Vector_2 ;
typedef Kernel::Vector_3          Vector_3 ;
typedef Kernel::Point_2           Point_2 ;
typedef Kernel::Point_3           Point_3 ;


// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( CGAL_AffineTransform3Test )

/*
 * simple translate
 */
BOOST_AUTO_TEST_CASE( testTranslate )
{
	Vector_3 translate(4.0,5.0,6.0);
	CGAL::Aff_transformation_3< Kernel > affine( CGAL::TRANSLATION, translate );

	/*
	 * @warning : translate is not applied on Vector_3!
	 */
	Point_3 in(1.0,2.0,3.0);
	Point_3 out = affine( in ) ;
	BOOST_CHECK_EQUAL( out.x(), 5.0 );
	BOOST_CHECK_EQUAL( out.y(), 7.0 );
	BOOST_CHECK_EQUAL( out.z(), 9.0 );
}

/*
 * rotate around Z (PI/2), translate( 3.0, 4.0, 5.0 )
 */
BOOST_AUTO_TEST_CASE( testRotateAndTranslate )
{
	/*
	 * @warning : translate is not applied on Vector_3!
	 */

	CGAL::Aff_transformation_3< Kernel > affine(
		0.0, -1.0, 0.0, 3.0,
		1.0,  0.0, 0.0, 4.0,
		0.0,  0.0, 1.0, 5.0/*,
		0.0,  0.0, 0.0, 1.0*/
	);
	Point_3 in(1.0,1.0,3.0);
	Point_3 out = affine( in ) ;

	BOOST_CHECK_EQUAL( out.x(), 2.0 ); //-1.0 + 3.0
	BOOST_CHECK_EQUAL( out.y(), 5.0 ); //1.0 + 4.0
	BOOST_CHECK_EQUAL( out.z(), 8.0 ); //3.0 + 5.0
}




BOOST_AUTO_TEST_SUITE_END()


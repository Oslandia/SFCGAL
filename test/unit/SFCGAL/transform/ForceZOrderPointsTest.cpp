#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/transform/ForceZOrderPoints.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_transform_ForceZOrderPointsTest )

BOOST_AUTO_TEST_CASE( simple )
{
	std::auto_ptr<Geometry> g1 = io::readWkt( "POLYGON((647732.480570211 6859892.27342207,647751.917554836 6859905.96306653,647758.849473995 6859910.84527601,647729.333625251 6859953.05055238,647692.408472298 6859926.97152605,647722.038426689 6859884.91892326,647732.480570211 6859892.27342207))" );

	std::cout << "ForceZOrderPoints" << std::endl;
	std::cout << g1->asText(3) << std::endl;

	transform::ForceZOrderPoints forceZ;
	g1->accept( forceZ );
	std::cout << g1->asText(3) << std::endl;
	g1->accept( forceZ );
	std::cout << g1->asText(3) << std::endl;
}



BOOST_AUTO_TEST_SUITE_END()





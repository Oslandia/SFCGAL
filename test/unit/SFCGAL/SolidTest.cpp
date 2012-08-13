#include <CGAL/Cartesian.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Cartesian< double >  Kernel ;
typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;

#include <SFCGAL/Solid.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_SolidTest )

BOOST_AUTO_TEST_CASE( solidConversionTest )
{
	// the unit cube where half of a cube has been substracted
	std::string gstr = "SOLID("
		// exterior shell
		"("
		"((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))," // front face
		"((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0))," // right face
		"((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))," // top face
		"((0 0 1,0 1 1,0 1 0,0 0 0,0 0 1))," // left face
		"((1 0 1,1 1 1,0 1 1,0 0 1,1 0 1))," // back face
		"((1 0 0,1 0 1,0 0 1,0 0 0,1 0 0))" // bottom face
		"),"
		// interior shell, a.k.a. a hole
		"("
		"((0 0 0,0 0.5 0,0.5 0.5 0,0.5 0 0,0 0 0))," // front face
		"((0.5 0 0,0.5 0.5 0,0.5 0.5 0.5,0.5 0 0.5,0.5 0 0))," // right face
		"((0 0.5 0,0 0.5 0.5,0.5 0.5 0.5,0.5 0.5 0,0 0.5 0))," // top face
		"((0 0 0.5,0 0.5 0.5,0 0.5 0,0 0 0,0 0 0.5))," // left face
		"((0.5 0 0.5,0.5 0.5 0.5,0 0.5 0.5,0 0 0.5,0.5 0 0.5))," // back face
		"((0.5 0 0,0.5 0 0.5,0 0 0.5,0 0 0,0.5 0 0))" // bottom face
		")"
		")";

	std::auto_ptr<Geometry> g( io::readWkt( gstr ));

	const Solid& solid = static_cast<const Solid&>( *g );
	CGAL::Nef_polyhedron_3<ExactKernel> nef( solid.toNef_polyhedron_3<ExactKernel>());
	// Here a Nef_polyhedron merges adjacent triangles
	BOOST_CHECK_EQUAL( nef.number_of_facets(), 9U );
}

BOOST_AUTO_TEST_SUITE_END()

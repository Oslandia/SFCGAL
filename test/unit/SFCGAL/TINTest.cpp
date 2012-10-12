#include <SFCGAL/Kernel.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_TINTest )

BOOST_AUTO_TEST_CASE( polyhedronConversionTest )
{
	// two unit squares sharing a common edge (1,0)-(1,1)
	std::string gstr = "POLYHEDRALSURFACE(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),"
		"((1 0 0,1 1 0,2 1 0,2 0 0,1 0 0)))";
	// the following surface would generate an exception, since the two polygons have opposite orientations
	// "POLYHEDRALSURFACE(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),((2 0 0,2 1 0,1 1 0,1 0 0,2 0 0)))";
	std::auto_ptr<Geometry> g( io::readWkt( gstr ));

	TriangulatedSurface tri;
	algorithm::triangulate( *g, tri );

	std::auto_ptr<CGAL::Polyhedron_3<Kernel> > poly( tri.toPolyhedron_3<Kernel, CGAL::Polyhedron_3<Kernel> >() );
	// we check the two squares share a common edge
	BOOST_CHECK_EQUAL( poly->size_of_facets(), 4U );
	BOOST_CHECK_EQUAL( poly->size_of_vertices(), 6U );
}

BOOST_AUTO_TEST_SUITE_END()





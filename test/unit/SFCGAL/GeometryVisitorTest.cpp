#include <boost/test/unit_test.hpp>

#include <cmath>

#include <SFCGAL/all.h>
#include <SFCGAL/GeometryVisitor.h>


using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

/**
 * get type from Geometry
 */
class DemoVisitorGetType : public ConstGeometryVisitor {
public:

	virtual void visit( const Point & g ) {
		type = "Point";
	}
	virtual void visit( const LineString & g ) {
		type = "LineString";
	}
	virtual void visit( const Polygon & g ) {
		type = "Polygon";
	}
	virtual void visit( const Triangle & g ) {
		type = "Triangle";
	}
	virtual void visit( const Solid & g ) {
		type = "Solid";
	}

	/**
	 * invoke visit(Point) for each point
	 */
	virtual void visit( const MultiPoint & g ) {
		type = "MultiPoint";
	}
	/**
	 * invoke visit(LineString) for each lineString
	 */
	virtual void visit( const MultiLineString & g ){
		type = "MultiLineString";
	}
	/**
	 * invoke visit(Polygon) for each polygon
	 */
	virtual void visit( const MultiPolygon & g ){
		type = "MultiPolygon";
	}

	/**
	 * invoke g.accept(geometry) for each sub geometry
	 */
	virtual void visit( const GeometryCollection & g ){
		type = "GeometryCollection";
	}

	/**
	 * invoke g.accept(polygon) for each polygon
	 */
	virtual void visit( const PolyhedralSurface & g ) {
		type = "PolyhedralSurface";
	}
	/**
	 * invoke g.accept(triangle) for each triangle
	 */
	virtual void visit( const TriangulatedSurface & g ) {
		type = "TriangulatedSurface";
	}

public:
	std::string type ;
};


template < typename T >
std::string getTypeWithVisitor(){
	std::auto_ptr< Geometry > geometry( new T() );
	DemoVisitorGetType visitor;
	geometry->accept( visitor );
	return visitor.type ;
}



/*
 * base checks (mainly for compilation issues)
 */
BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryVisitorTest )

BOOST_AUTO_TEST_CASE( testVisitPoint ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< Point >(), "Point" );
}
BOOST_AUTO_TEST_CASE( testVisitLineString ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< LineString >(), "LineString" );
}
BOOST_AUTO_TEST_CASE( testVisitPolygon ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< Polygon >(), "Polygon" );
}
BOOST_AUTO_TEST_CASE( testVisitTriangle ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< Triangle >(), "Triangle" );
}

BOOST_AUTO_TEST_CASE( testVisitMultiPoint ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< MultiPoint >(), "MultiPoint" );
}
BOOST_AUTO_TEST_CASE( testVisitMultiLineString ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< MultiLineString >(), "MultiLineString" );
}
BOOST_AUTO_TEST_CASE( testVisitMultiPolygon ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< MultiPolygon >(), "MultiPolygon" );
}

BOOST_AUTO_TEST_CASE( testVisitGeometryCollection ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< GeometryCollection >(), "GeometryCollection" );
}

BOOST_AUTO_TEST_CASE( testVisitTriangulatedSurface ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< TriangulatedSurface >(), "TriangulatedSurface" );
}

BOOST_AUTO_TEST_CASE( testVisitPolyhedralSurface ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< PolyhedralSurface >(), "PolyhedralSurface" );
}

BOOST_AUTO_TEST_CASE( testVisitSolid ){
	BOOST_CHECK_EQUAL( getTypeWithVisitor< Solid >(), "Solid" );
}


BOOST_AUTO_TEST_SUITE_END()


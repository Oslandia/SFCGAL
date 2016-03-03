/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>

#include <SFCGAL/Geometry.h>


using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

/**
 * get type from Geometry
 */
template < typename K >
class DemoVisitorGetType : public boost::static_visitor<> {
public:
    
    void operator () ( const Point<K>& ) {
        type = "Point";
    }
    
    void operator () ( const LineString<K>& ) {
        type = "LineString";
    }
    
    void operator () ( const Polygon<K>& ) {
        type = "Polygon";
    }
    
    void operator () ( const Triangle<K>& ) {
        type = "Triangle";
    }

    void operator () ( const Solid<K>& ) {
        type = "Solid";
    }
    
    void operator () ( const MultiPoint<K>& ) {
        type = "MultiPoint";
    }
    
    void operator () ( const MultiLineString<K>& ) {
        type = "MultiLineString";
    }
    
    void operator () ( const MultiPolygon<K>& ) {
        type = "MultiPolygon";
    }
    
    void operator () ( const MultiSolid<K>& ) {
        type = "MultiSolid";
    }

    void operator () ( const GeometryCollection<K>& ) {
        type = "GeometryCollection";
    }
    /*
    void operator () ( const PolyhedralSurface<K>& ) {
        type = "PolyhedralSurface";
    }
    */
    
    void operator () ( const TriangulatedSurface<K>& ) {
        type = "TriangulatedSurface";
    }


public:
    std::string type ;
};


template < typename K >
std::string getTypeWithVisitor(const Geometry<K>& g)
{
    DemoVisitorGetType<K> visitor;
    boost::apply_visitor(visitor,g);
    return visitor.type ;
}



/*
 * base checks (mainly for compilation issues)
 */
BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryVisitorTest )

BOOST_AUTO_TEST_CASE( testVisitPoint )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(Point<Epeck>()), "Point" );
}
BOOST_AUTO_TEST_CASE( testVisitLineString )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(LineString<Epeck>()), "LineString" );
}
BOOST_AUTO_TEST_CASE( testVisitPolygon )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(Polygon<Epeck>()), "Polygon" );
}
BOOST_AUTO_TEST_CASE( testVisitTriangle )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(Triangle<Epeck>()), "Triangle" );
}

BOOST_AUTO_TEST_CASE( testVisitMultiPoint )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(MultiPoint<Epeck>()), "MultiPoint" );
}
BOOST_AUTO_TEST_CASE( testVisitMultiLineString )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(MultiLineString<Epeck>()), "MultiLineString" );
}
BOOST_AUTO_TEST_CASE( testVisitMultiPolygon )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(MultiPolygon<Epeck>()), "MultiPolygon" );
}
BOOST_AUTO_TEST_CASE( testVisitMultiSolid )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(MultiSolid<Epeck>()), "MultiSolid" );
}
BOOST_AUTO_TEST_CASE( testVisitGeometryCollection )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(GeometryCollection<Epeck>()), "GeometryCollection" );
}

BOOST_AUTO_TEST_CASE( testVisitTriangulatedSurface )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(TriangulatedSurface<Epeck>()), "TriangulatedSurface" );
}

/*
BOOST_AUTO_TEST_CASE( testVisitPolyhedralSurface )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(PolyhedralSurface<Epeck>()), "PolyhedralSurface" );
}
*/

BOOST_AUTO_TEST_CASE( testVisitSolid )
{
    BOOST_CHECK_EQUAL( getTypeWithVisitor<Epeck>(Solid<Epeck>()), "Solid" );
}


BOOST_AUTO_TEST_SUITE_END()


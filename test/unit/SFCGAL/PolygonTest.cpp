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

#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/io/wkt.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_PolygonTest )

//Polygon() ;
BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    Polygon<Epeck> g;
    BOOST_CHECK( g.isEmpty() );
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
}

//Polygon( const std::vector< LineString > & rings ) ;
//Polygon( const LineString & exteriorRing ) ;
BOOST_AUTO_TEST_CASE( exteriorRingConstructor )
{
    LineString<Epeck> exteriorRing ;
    exteriorRing.push_back( Point<Epeck>( 0.0,0.0 ) );
    exteriorRing.push_back( Point<Epeck>( 1.0,0.0 ) );
    exteriorRing.push_back( Point<Epeck>( 1.0,1.0 ) );
    exteriorRing.push_back( Point<Epeck>( 0.0,1.0 ) );
    exteriorRing.push_back( Point<Epeck>( 0.0,0.0 ) );

    Polygon<Epeck> g;
    g.setExteriorRing( exteriorRing );
    BOOST_CHECK( ! g.isEmpty() );
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
    BOOST_CHECK_EQUAL( g.exteriorRing().numPoints(), 5U );
}
BOOST_AUTO_TEST_CASE( exteriorRingConstructor3D )
{
    Polygon<Epeck> g;
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0,2.0 ) );

    BOOST_CHECK( ! g.isEmpty() );
    BOOST_CHECK( g.is3D() );
    BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
}


//Polygon( LineString * exteriorRing ) ;
//Polygon( const Triangle & triangle ) ;
/*
BOOST_AUTO_TEST_CASE( testConstructorTriangle )
{
    Polygon<Epeck> g(
        Triangle<Epeck>(
            Point<Epeck>( 0.0,0.0 ),
            Point<Epeck>( 0.0,1.0 ),
            Point<Epeck>( 1.0,1.0 )
        )
    );
    BOOST_CHECK_EQUAL( g.numRings(), 1U );
    BOOST_CHECK_EQUAL( g.exteriorRing().numPoints(), 4U );
}
*/
//Polygon( Polygon const& other ) ;

//Polygon( const CGAL::Polygon_2< Kernel >& other );
//Polygon( const CGAL::Polygon_with_holes_2< Kernel >& poly );

//Polygon& operator = ( const Polygon & other ) ;
//~Polygon() ;


//bool isCounterClockWiseOriented() const;

//void reverse() ;
BOOST_AUTO_TEST_CASE( testReverse )
{
/*
    Polygon<Epeck> g(
        Triangle<Epeck>(
            Point( 0.0,0.0 ),
            Point( 1.0,0.0 ),
            Point( 1.0,1.0 )
        )
    );
    BOOST_CHECK( g.isCounterClockWiseOriented() );
    g.reverse();
    BOOST_CHECK( ! g.isCounterClockWiseOriented() );
*/
}
//TODO same with holes

//inline const LineString &    exteriorRing() const
//inline LineString &          exteriorRing()
//inline void  setExteriorRing( const LineString& ring )
//inline void  setExteriorRing( LineString* ring )
//inline bool                  hasInteriorRings() const
//inline size_t                numInteriorRings() const
//inline const LineString &    interiorRingN( const size_t & n ) const
//inline LineString &          interiorRingN( const size_t & n )
//inline size_t  numRings() const
//inline const LineString &    ringN( const size_t & n ) const
//inline LineString &          ringN( const size_t & n )
//inline void            addInteriorRing( const LineString & ls )
//inline void            addInteriorRing( LineString* ls )
//inline void            addRing( const LineString & ls )
//inline void            addRing( LineString* ls )
//inline iterator       begin() { return _rings.begin() ; }
//inline const_iterator begin() const { return _rings.begin() ; }
//inline iterator       end() { return _rings.end() ; }
//inline const_iterator end() const { return _rings.end() ; }



//CGAL::Polygon_2<Kernel> toPolygon_2( bool fixOrientation = true ) const;
//CGAL::Polygon_with_holes_2<Kernel> toPolygon_with_holes_2( bool fixOrientation = true ) const;



//-- Geometry

//virtual Geometry *   Geometry::clone() const = 0 ;
/*
BOOST_AUTO_TEST_CASE( testClone )
{
    LineString<Epeck> exteriorRing ;
    exteriorRing.push_back( Point( 0.0,0.0 ) );
    exteriorRing.push_back( Point( 1.0,0.0 ) );
    exteriorRing.push_back( Point( 1.0,1.0 ) );
    exteriorRing.push_back( Point( 0.0,1.0 ) );
    exteriorRing.push_back( Point( 0.0,0.0 ) );

    Polygon<Epeck> g( exteriorRing );
    std::auto_ptr< Polygon > copy( g.clone() );

    BOOST_CHECK( ! copy->isEmpty() );
    BOOST_CHECK( ! copy->is3D() );
    BOOST_CHECK_EQUAL( copy->numInteriorRings(), 0U );
    BOOST_CHECK_EQUAL( copy->exteriorRing().numPoints(), 5U );
}
*/

//virtual Geometry*    Geometry::boundary() const ;
BOOST_AUTO_TEST_CASE( testBoundaryEmpty )
{
    /*
    std::auto_ptr< Geometry > boundary( Polygon().boundary() );
    BOOST_CHECK( boundary->isEmpty() );
    BOOST_CHECK( boundary->is< GeometryCollection >() );
    */
}
BOOST_AUTO_TEST_CASE( testBoundaryWithoutHoles )
{
    /*
    std::string wkt( "POLYGON((0 0,0 1,1 1,0 0))" ) ;
    std::auto_ptr< Geometry > boundary( io::readWkt( wkt )->boundary() );
    BOOST_CHECK( ! boundary->isEmpty() );
    BOOST_CHECK_EQUAL( boundary->asText( 0 ), "LINESTRING(0 0,0 1,1 1,0 0)" );
    */
}
BOOST_AUTO_TEST_CASE( testBoundaryWithHoles )
{
    /*
    std::string wkt( "POLYGON((0 0,0 5,5 5,0 5,0 0),(1 1,2 1,2 2,1 1))" ) ;
    std::auto_ptr< Geometry > boundary( io::readWkt( wkt )->boundary() );
    BOOST_CHECK( ! boundary->isEmpty() );
    BOOST_CHECK_EQUAL( boundary->asText( 0 ), "MULTILINESTRING((0 0,0 5,5 5,0 5,0 0),(1 1,2 1,2 2,1 1))" );
    */
}

//Envelope             Geometry::envelope() const ;

/*

//std::string          Geometry::asText( const int & numDecimals = -1 ) const ;
BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    Polygon<Epeck> g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "POLYGON EMPTY" );
}
BOOST_AUTO_TEST_CASE( asText2d )
{
    Polygon<Epeck> g;
    g.exteriorRing().push_back( Point( 0.0,0.0 ) );
    g.exteriorRing().push_back( Point( 1.0,0.0 ) );
    g.exteriorRing().push_back( Point( 1.0,1.0 ) );
    g.exteriorRing().push_back( Point( 0.0,1.0 ) );
    g.exteriorRing().push_back( Point( 0.0,0.0 ) );

    BOOST_CHECK_EQUAL( g.asText( 1 ), "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
    Polygon<Epeck> g;
    g.exteriorRing().push_back( Point( 0.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point( 1.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point( 1.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point( 0.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point( 0.0,0.0,2.0 ) );

    BOOST_CHECK_EQUAL( g.asText( 1 ), "POLYGON((0.0 0.0 2.0,1.0 0.0 2.0,1.0 1.0 2.0,0.0 1.0 2.0,0.0 0.0 2.0))" );
}

//virtual std::string  Geometry::geometryType() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryType )
{
    Polygon<Epeck> g;
    BOOST_CHECK_EQUAL( g.geometryType(), "Polygon" );
}
//virtual GeometryType Geometry::geometryTypeId() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryTypeId )
{
    Polygon<Epeck> g;
    BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_POLYGON );
}
//virtual int          Geometry::dimension() const = 0 ;
BOOST_AUTO_TEST_CASE( testDimension )
{
    Polygon<Epeck> g;
    BOOST_CHECK_EQUAL( g.dimension(), 2 );
}
//virtual int          Geometry::coordinateDimension() const = 0 ;
//virtual bool         Geometry::isEmpty() const = 0 ;
//virtual bool         Geometry::is3D() const = 0 ;
//virtual bool         Geometry::isMeasured() const = 0 ;
//virtual bool         Geometry::isSimple() const = 0 ;

//template < typename Derived > inline bool Geometry::is() const
BOOST_AUTO_TEST_CASE( isPolygon )
{
    Polygon g;
    BOOST_CHECK( g.is< Polygon >() );
}
//template < typename Derived > inline const Derived &  Geometry::as() const
//template < typename Derived > inline Derived &        Geometry::as()

*/


BOOST_AUTO_TEST_SUITE_END()





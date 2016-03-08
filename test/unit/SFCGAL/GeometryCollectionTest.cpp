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

#include <exception>

#include <SFCGAL/types.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryCollectionTest )

//GeometryCollection() ;
BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    GeometryCollection<Epeck> g ;
    BOOST_CHECK( g.isEmpty() );
}

//GeometryCollection( GeometryCollection const& other ) ;
//GeometryCollection& operator = ( const GeometryCollection & other ) ;
//virtual ~GeometryCollection() ;

//virtual size_t              numGeometries() const ;
//virtual const Geometry  &  geometryN( size_t const& n ) const ;
//virtual Geometry &          geometryN( size_t const& n ) ;
//void                      addGeometry( Geometry * geometry ) ;
//void                      addGeometry( Geometry const& geometry ) ;
BOOST_AUTO_TEST_CASE( testAccessors )
{
    GeometryCollection<Epeck> g ;

    g.push_back( new Point<Epeck>( 2.0,3.0 ) ) ;
    BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
    g.push_back( new LineString<Epeck>{ 
        Point<Epeck>( 0.0,0.0 ),
        Point<Epeck>( 1.0,1.0 ) 
    } ) ;
    BOOST_CHECK_EQUAL( g.numGeometries(), 2U );
    g.push_back( new Triangle<Epeck>{
        Point<Epeck>( 0.0,0.0 ),
        Point<Epeck>( 1.0,0.0 ),
        Point<Epeck>( 1.0,1.0 ) 
    } ) ;
    BOOST_CHECK_EQUAL( g.numGeometries(), 3U );

//    BOOST_CHECK_EQUAL( g.geometryN( 0 ).asText( 0 ), "POINT(2 3)"  ) ;
//    BOOST_CHECK_EQUAL( g.geometryN( 1 ).asText( 0 ), "LINESTRING(0 0,1 1)"  ) ;
//    BOOST_CHECK_EQUAL( g.geometryN( 2 ).asText( 0 ), "TRIANGLE((0 0,1 0,1 1,0 0))"  ) ;
}


//-- iterators

//inline iterator       begin()
//inline const_iterator begin() const
//inline iterator       end()
//inline const_iterator end() const
BOOST_AUTO_TEST_CASE( testIterators )
{
    GeometryCollection<Epeck> g ;
    g.push_back( new Point<Epeck>( 0.0,0.0 ) );
    g.push_back( new Point<Epeck>( 1.0,1.0 ) );

    GeometryCollection<Epeck>::const_iterator it = g.begin() ;

    BOOST_CHECK( it->as<Point<Epeck>>() == Point<Epeck>( 0.0,0.0 ) );
    ++it ;
    BOOST_CHECK( it->as<Point<Epeck>>() == Point<Epeck>( 1.0,1.0 ) );
    ++it ;
    BOOST_CHECK( it == g.end() );
}


//-- Geometry

//virtual Geometry *   Geometry::clone() const = 0 ;
//virtual Geometry*    Geometry::boundary() const ;
//Envelope             Geometry::envelope() const ;

//std::string          Geometry::asText( const int & numDecimals = -1 ) const ;
/*
BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    GeometryCollection g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "GEOMETRYCOLLECTION EMPTY" );
}
BOOST_AUTO_TEST_CASE( asText2d )
{
    GeometryCollection g;
    g.addGeometry( Point( 2.0,3.0 ) );
    g.addGeometry( Triangle( Point( 0.0,0.0 ), Point( 1.0,0.0 ), Point( 1.0,1.0 ) ) );
    BOOST_CHECK_EQUAL( g.asText( 1 ), "GEOMETRYCOLLECTION(POINT(2.0 3.0),TRIANGLE((0.0 0.0,1.0 0.0,1.0 1.0,0.0 0.0)))" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
    GeometryCollection g;
    g.addGeometry( Point( 2.0,3.0,5.0 ) );
    g.addGeometry( Triangle( Point( 0.0,0.0,6.0 ), Point( 1.0,0.0,6.0 ), Point( 1.0,1.0,6.0 ) ) );
    BOOST_CHECK_EQUAL( g.asText( 1 ), "GEOMETRYCOLLECTION(POINT(2.0 3.0 5.0),TRIANGLE((0.0 0.0 6.0,1.0 0.0 6.0,1.0 1.0 6.0,0.0 0.0 6.0)))" );
}
*/

//virtual std::string  Geometry::geometryType() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryType )
{
    GeometryCollection<Epeck> g;
    BOOST_CHECK_EQUAL( g.geometryType(), "GeometryCollection" );
}
//virtual GeometryType Geometry::geometryTypeId() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryTypeId )
{
    GeometryCollection<Epeck> g;
    BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_GEOMETRYCOLLECTION );
}

//virtual int          Geometry::dimension() const = 0 ;
//virtual int          Geometry::coordinateDimension() const = 0 ;
//virtual bool         Geometry::isEmpty() const = 0 ;
//virtual bool         Geometry::is3D() const = 0 ;
//virtual bool         Geometry::isMeasured() const = 0 ;
//virtual bool         Geometry::isSimple() const = 0 ;

//template < typename Derived > inline bool Geometry::is() const
BOOST_AUTO_TEST_CASE( testIsGeometryCollection )
{
    BOOST_CHECK( GeometryCollection<Epeck>().is< GeometryCollection<Epeck> >() );
    
    // change in 2.0
    BOOST_CHECK( ! MultiPoint<Epeck>().is< GeometryCollection<Epeck> >() );
    BOOST_CHECK( ! MultiLineString<Epeck>().is< GeometryCollection<Epeck> >() );
    BOOST_CHECK( ! MultiPolygon<Epeck>().is< GeometryCollection<Epeck> >() );
    BOOST_CHECK( ! MultiSolid<Epeck>().is< GeometryCollection<Epeck> >() );
}

//template < typename Derived > inline const Derived &  Geometry::as() const
//template < typename Derived > inline Derived &        Geometry::as()



BOOST_AUTO_TEST_SUITE_END()





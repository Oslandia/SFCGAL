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
#include <SFCGAL/Kernel.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Exception.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_PointTest )


//Point() ;
BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    Point g;
    BOOST_CHECK( g.isEmpty() ) ;
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK( ! g.isMeasured() );

    // no more access to double
    BOOST_CHECK_THROW( g.x(), Exception );
    BOOST_CHECK_THROW( g.y(), Exception );
    BOOST_CHECK_THROW( g.z(), Exception );
    BOOST_CHECK( isNaN( g.m() ) );
}

//Point( const Coordinate & coordinate ) ;

//Point( const Kernel::FT & x, const Kernel::FT & y ) ;
BOOST_AUTO_TEST_CASE( xyConstructor )
{
    Point g( 2.0,3.0 );
    BOOST_CHECK( ! g.isEmpty() ) ;
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK_EQUAL( g.x(), 2.0 );
    BOOST_CHECK_EQUAL( g.y(), 3.0 );
    BOOST_CHECK_EQUAL( g.z(), 0 );
}

//Point( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ) ;
//Point( const double & x, const double & y, const double & z = NaN() ) ;
BOOST_AUTO_TEST_CASE( xyzConstructor )
{
    Point g( 2.0,3.0,4.0 );
    BOOST_CHECK( ! g.isEmpty() ) ;
    BOOST_CHECK( g.is3D() );
    BOOST_CHECK_EQUAL( g.x(), 2.0 );
    BOOST_CHECK_EQUAL( g.y(), 3.0 );
    BOOST_CHECK_EQUAL( g.z(), 4.0 );
}
//Point( const Kernel::Point_2 & other ) ;
//Point( const Kernel::Point_3 & other ) ;
//Point( const Point & other ) ;
//Point& operator = ( const Point & other ) ;
//~Point() ;


//-- tested in Coordinate
//inline Kernel::RT x() const { return _coordinate.x() ; }
//inline Kernel::RT y() const { return _coordinate.y() ; }
//inline Kernel::RT z() const { return _coordinate.z() ; }

//inline double    m() const { return _m ; }
//inline void      setM( const double & m ) { _m = m ; }
BOOST_AUTO_TEST_CASE( testGetSetM )
{
    Point p( 3.0,4.0 );
    BOOST_CHECK( ! p.isMeasured() );
    BOOST_CHECK( isNaN( p.m() ) );
    p.setM( 5.0 );
    BOOST_CHECK_EQUAL( p.m(), 5.0 );
}

//bool operator < ( const Point & other ) const ;
//bool operator == ( const Point & other ) const ;
//bool operator != ( const Point & other ) const ;


//inline Kernel::Vector_2 toVector_2() const
//inline Kernel::Vector_3 toVector_3() const
//inline Kernel::Point_2 toPoint_2() const
//inline Kernel::Point_3 toPoint_3() const
BOOST_AUTO_TEST_CASE( emptyToVector_2 )
{
    Point g ;
    CGAL::Vector_2< Kernel > p = g.toVector_2();
    BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 0.0 );
    BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 0.0 );
}
BOOST_AUTO_TEST_CASE( xyToVector_2 )
{
    Point g( 3.0, 4.0 );
    CGAL::Vector_2< Kernel > p = g.toVector_2();
    BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 3.0 );
    BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 4.0 );
}
BOOST_AUTO_TEST_CASE( xyToVector_3 )
{
    Point g( 3.0, 4.0 );
    CGAL::Vector_3< Kernel > p = g.toVector_3();

    BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 3.0 );
    BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 4.0 );
    BOOST_CHECK_EQUAL( CGAL::to_double( p.z() ), 0.0 );
}


//template <int D> typename TypeForDimension<D>::Point toPoint_d() const;
//inline Coordinate &       coordinate() { return _coordinate; }
//inline const Coordinate & coordinate() const { return _coordinate; }


//-- SFCGAL::Geometry

//virtual Geometry *   Geometry::clone() const = 0 ;
BOOST_AUTO_TEST_CASE( testClone )
{
    Point p( 3.0,4.0 );
    std::auto_ptr< Geometry > copy( p.clone() );
    BOOST_REQUIRE( copy->is< Point >() );
    BOOST_CHECK_EQUAL( copy->as< Point >().x(), 3.0 );
    BOOST_CHECK_EQUAL( copy->as< Point >().y(), 4.0 );
}

//virtual Geometry*    Geometry::boundary() const ;
BOOST_AUTO_TEST_CASE( testBoundary )
{
    Point p( 3.0,4.0 );
    std::auto_ptr< Geometry > boundary( p.boundary() );
    BOOST_CHECK( boundary->isEmpty() );
    BOOST_CHECK( boundary->is< GeometryCollection >() );
}

//Envelope             Geometry::envelope() const ;
BOOST_AUTO_TEST_CASE( testEnvelope_empty )
{
    BOOST_CHECK( Point().envelope().isEmpty() );
}
BOOST_AUTO_TEST_CASE( testEnvelope_2D )
{
    Point g( 3.0,4.0 );
    Envelope box = g.envelope() ;
    BOOST_CHECK( ! box.isEmpty() );
    BOOST_CHECK( ! box.is3D() );

    BOOST_CHECK_EQUAL( box.xMin(), 3.0 );
    BOOST_CHECK_EQUAL( box.xMax(), 3.0 );
    BOOST_CHECK_EQUAL( box.yMin(), 4.0 );
    BOOST_CHECK_EQUAL( box.yMax(), 4.0 );
}
BOOST_AUTO_TEST_CASE( testEnvelope_3D )
{
    Point g( 3.0,4.0,5.0 );
    Envelope box = g.envelope() ;
    BOOST_CHECK( ! box.isEmpty() );
    BOOST_CHECK( box.is3D() );

    BOOST_CHECK_EQUAL( box.xMin(), 3.0 );
    BOOST_CHECK_EQUAL( box.xMax(), 3.0 );
    BOOST_CHECK_EQUAL( box.yMin(), 4.0 );
    BOOST_CHECK_EQUAL( box.yMax(), 4.0 );
    BOOST_CHECK_EQUAL( box.zMin(), 5.0 );
    BOOST_CHECK_EQUAL( box.zMax(), 5.0 );
}


//std::string          Geometry::asText( const int & numDecimals = -1 ) const ;
BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    Point g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "POINT EMPTY" );
}
BOOST_AUTO_TEST_CASE( asText2d )
{
    Point g( 2.0,3.0 );
    BOOST_CHECK_EQUAL( g.asText( 3 ), "POINT(2.000 3.000)" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
    Point g( 2.0,3.0,4.0 );
    BOOST_CHECK_EQUAL( g.asText( 3 ), "POINT(2.000 3.000 4.000)" );
}

//virtual std::string  Geometry::geometryType() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryType )
{
    Point g;
    BOOST_CHECK_EQUAL( g.geometryType(), "Point" );
}
//virtual GeometryType Geometry::geometryTypeId() const = 0 ;
BOOST_AUTO_TEST_CASE( testGeometryTypeId )
{
    Point g;
    BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_POINT );
}

//virtual int          Geometry::dimension() const = 0 ;
BOOST_AUTO_TEST_CASE( testDimension )
{
    Point g;
    BOOST_CHECK_EQUAL( g.dimension(), 0 );
}

//virtual int          Geometry::coordinateDimension() const = 0 ;
BOOST_AUTO_TEST_CASE( testCoordinateDimension )
{
    BOOST_CHECK_EQUAL( Point().coordinateDimension(), 0 );
    BOOST_CHECK_EQUAL( Point( 2.0,3.0 ).coordinateDimension(), 2 );
    BOOST_CHECK_EQUAL( Point( 2.0,3.0,4.0 ).coordinateDimension(), 3 );
}
//virtual bool         Geometry::isEmpty() const = 0 ;
BOOST_AUTO_TEST_CASE( testIsEmpty )
{
    BOOST_CHECK( Point().isEmpty() );
    BOOST_CHECK( ! Point( 2.0,3.0 ).isEmpty() );
}
//virtual bool         Geometry::is3D() const = 0 ;
BOOST_AUTO_TEST_CASE( testIs3D )
{
    BOOST_CHECK( ! Point().is3D() );
    BOOST_CHECK( ! Point( 2.0,3.0 ).is3D() );
    BOOST_CHECK( Point( 2.0,3.0,4.0 ).is3D() );
}
//virtual bool         Geometry::isMeasured() const = 0 ;
BOOST_AUTO_TEST_CASE( testIsMeasured )
{
    BOOST_CHECK( ! Point().isMeasured() );
    BOOST_CHECK( ! Point( 2.0,3.0 ).isMeasured() );
    BOOST_CHECK( ! Point( 2.0,3.0,4.0 ).isMeasured() );
    BOOST_CHECK( Point( 2.0,3.0,4.0,5.0 ).isMeasured() );
}

//TODO
//virtual bool         Geometry::isSimple() const = 0 ;

//template < typename Derived > inline bool Geometry::is() const
BOOST_AUTO_TEST_CASE( isPoint )
{
    Point g;
    BOOST_CHECK( g.is< Point >() );
}
//template < typename Derived > inline const Derived &  Geometry::as() const
//template < typename Derived > inline Derived &        Geometry::as()
BOOST_AUTO_TEST_CASE( asPoint )
{
    std::auto_ptr< Geometry > g( new Point() );
    BOOST_CHECK( g->as< Point >().isEmpty() );
}



BOOST_AUTO_TEST_SUITE_END()





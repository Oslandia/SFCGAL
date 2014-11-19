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

#include <SFCGAL/Solid.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_SolidTest )

//Solid() ;
//Solid( const PolyhedralSurface & exteriorShell ) ;
//Solid( PolyhedralSurface * exteriorShell ) ;
//Solid( const std::vector< PolyhedralSurface > & shells ) ;
//Solid( Solid const& other ) ;
//Solid& operator = ( const Solid & other ) ;
//~Solid() ;

//inline const PolyhedralSurface &    exteriorShell() const { return _shells[0] ; }
//inline PolyhedralSurface &          exteriorShell() { return _shells[0] ; }
//inline size_t                       numInteriorShells() const { return _shells.size() - 1 ; }
//inline const PolyhedralSurface  &   interiorShellN( size_t const& n ) const { return _shells[n+1]; }
//inline PolyhedralSurface &          interiorShellN( size_t const& n ) { return _shells[n+1]; }
//inline void                         addInteriorShell( const PolyhedralSurface & shell )
//inline void                         addInteriorShell( PolyhedralSurface * shell )
//inline size_t  numShells() const {
//inline const PolyhedralSurface &  shellN( const size_t & n ) const {
//inline PolyhedralSurface &        shellN( const size_t & n ) {

//-- iterators

//inline iterator       begin() { return _shells.begin() ; }
//inline const_iterator begin() const { return _shells.begin() ; }
//inline iterator       end() { return _shells.end() ; }
//inline const_iterator end() const { return _shells.end() ; }


//-- helpers

//template < typename K > CGAL::Nef_polyhedron_3<K> toNef_polyhedron_3() const



//virtual Geometry *   Geometry::clone() const = 0 ;
//virtual Geometry*    Geometry::boundary() const ;
//Envelope             Geometry::envelope() const ;
//std::string          Geometry::asText( const int & numDecimals = -1 ) const ;
//virtual std::string  Geometry::geometryType() const = 0 ;
//virtual GeometryType Geometry::geometryTypeId() const = 0 ;
//virtual int          Geometry::dimension() const = 0 ;
//virtual int          Geometry::coordinateDimension() const = 0 ;
//virtual bool         Geometry::isEmpty() const = 0 ;
//virtual bool         Geometry::is3D() const = 0 ;
//virtual bool         Geometry::isMeasured() const = 0 ;
//virtual bool         Geometry::isSimple() const = 0 ;
//template < typename Derived > inline bool Geometry::is() const
//template < typename Derived > inline const Derived &  Geometry::as() const
//template < typename Derived > inline Derived &        Geometry::as()



//-- other tests

BOOST_AUTO_TEST_CASE( solidReadTest )
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

    std::auto_ptr<Geometry> g( io::readWkt( gstr ) );
    BOOST_CHECK_EQUAL( g->as< Solid >().numShells(),2U );
}

BOOST_AUTO_TEST_SUITE_END()

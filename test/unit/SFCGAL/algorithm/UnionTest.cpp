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
#include <SFCGAL/Exception.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/union.h>
#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/detail/tools/Registry.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>

#include <boost/test/unit_test.hpp>

#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << " debug: " 

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_UnionTest )

BOOST_AUTO_TEST_CASE( PointPoint )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTIPOINT(0 0,0 1)" ) );
    }
}

BOOST_AUTO_TEST_CASE( TrianglePoint )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(.1 .1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
}

BOOST_AUTO_TEST_CASE( TriangleTriangle )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
}

BOOST_AUTO_TEST_SUITE_END()


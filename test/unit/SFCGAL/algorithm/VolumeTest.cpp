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

#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>

#include <boost/test/unit_test.hpp>

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_VolumeTest )


BOOST_AUTO_TEST_CASE( cubeVolume )
{
    std::unique_ptr<Geometry> s = io::readWkt( "SOLID((((0 0 0,0 0 1,0 1 1,0 1 0,0 0 0)),\
                                                     ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),\
                                                     ((0 0 0,1 0 0,1 0 1,0 0 1,0 0 0)),\
                                                     ((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0)),\
                                                     ((0 0 1,1 0 1,1 1 1,0 1 1,0 0 1)),\
                                                     ((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))))" );
    BOOST_CHECK_EQUAL( algorithm::volume( *s ), 1 );
}

BOOST_AUTO_TEST_CASE( cubeWithHoleVolume )
{
    std::unique_ptr<Geometry> s = io::readWkt(
                                    "SOLID((((0 0 0,0 0 1,0 1 1,0 1 0,0 0 0)),\
                ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),\
                ((0 0 0,1 0 0,1 0 1,0 0 1,0 0 0)),\
                ((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0)),\
                ((0 0 1,1 0 1,1 1 1,0 1 1,0 0 1)),\
                ((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))),\
               (((.2 .2 .2,.2 .8 .2,.2 .8 .8,.2 .2 .8,.2 .2 .2)),\
                ((.2 .2 .2,.8 .2 .2,.8 .8 .2,.2 .8 .2,.2 .2 .2)),\
                ((.2 .2 .2,.2 .2 .8,.8 .2 .8,.8 .2 .2,.2 .2 .2)),\
                ((.8 .2 .2,.8 .2 .8,.8 .8 .8,.8 .8 .2,.8 .2 .2)),\
                ((.2 .2 .8,.2 .8 .8,.8 .8 .8,.8 .2 .8,.2 .2 .8)),\
                ((.2 .8 .2,.8 .8 .2,.8 .8 .8,.2 .8 .8,.2 .8 .2))))" );
    const Kernel::FT c( .6 );
    const Kernel::FT ref =  1 - c*c*c;
    BOOST_CHECK_EQUAL( CGAL::to_double( algorithm::volume( s->as<Solid>(), algorithm::NoValidityCheck() ) ), CGAL::to_double( ref ) );
}

BOOST_AUTO_TEST_CASE( invertedCubeVolume )
{
    std::unique_ptr<Geometry> s = io::readWkt( "SOLID((((0 0 0,0 1 0,0 1 1,0 0 1,0 0 0)),\
                                                     ((0 0 0,1 0 0,1 1 0,0 1 0,0 0 0)),\
                                                     ((0 0 0,0 0 1,1 0 1,1 0 0,0 0 0)),\
                                                     ((1 0 0,1 0 1,1 1 1,1 1 0,1 0 0)),\
                                                     ((0 0 1,0 1 1,1 1 1,1 0 1,0 0 1)),\
                                                     ((0 1 0,1 1 0,1 1 1,0 1 1,0 1 0))))" );
    BOOST_CHECK_EQUAL( algorithm::volume( *s ), -1 );

}


BOOST_AUTO_TEST_SUITE_END()


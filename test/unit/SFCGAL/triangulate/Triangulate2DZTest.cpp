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

#include <SFCGAL/Exception.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_triangulate_Triangulate2DZTest )

BOOST_AUTO_TEST_CASE( testPoint )
{
    std::unique_ptr< Geometry > g( io::readWkt( "POINT(1.0 2.0 3.0)" ) ) ;
    ConstraintDelaunayTriangulation triangulation = triangulate2DZ( *g );
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 1U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 0U );
}

BOOST_AUTO_TEST_CASE( testLineString )
{
    std::unique_ptr< Geometry > g( io::readWkt( "LINESTRING(0.0 0.0,1.0 0.0,1.0 1.0,2.0 1.0)" ) ) ;
    ConstraintDelaunayTriangulation triangulation = triangulate2DZ( *g );
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 4U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 2U );
}

BOOST_AUTO_TEST_CASE( testPolygonWithHole )
{
    std::unique_ptr< Geometry > g( io::readWkt( "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0),(0.2 0.2,0.2 0.8,0.8 0.8,0.8 0.2,0.2 0.2))" ) ) ;
    ConstraintDelaunayTriangulation triangulation = triangulate2DZ( *g );
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 8U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 10U );
}


BOOST_AUTO_TEST_CASE( testMultiPoint )
{
    std::unique_ptr< Geometry > g( io::readWkt( "MULTIPOINT((1.0 2.0 3.0),(2.0 3.0 6.0),(8.0 6.0 7.0),(2.0 1.0 6.0))" ) ) ;
    ConstraintDelaunayTriangulation triangulation = triangulate2DZ( *g );
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 4U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 2U );
}

//MultiLineString
//MultiPolygon
//GeometryCollection
BOOST_AUTO_TEST_CASE( testMultiPolygon )
{
    std::unique_ptr< Geometry > g( io::readWkt( "GEOMETRYCOLLECTION(LINESTRING(-1.52451708766716 0.583952451708767,-1.5408618127786 0.361069836552749,-1.47251114413076 0.317979197622586,-1.30163447251114 0.398216939078752,-1.32095096582467 0.482912332838039,-1.08320950965825 0.598811292719168,-0.809806835066865 0.570579494799406,-0.517087667161962 0.662704309063893),POLYGON((-1.46508172362556 0.615156017830609,-1.35215453194651 0.806835066864785,-1.08320950965825 0.754829123328381,-1.10401188707281 0.630014858841011,-1.2407132243685 0.557206537890045,-1.46508172362556 0.615156017830609)),POLYGON((-1.2778603268945 0.316493313521545,-0.925705794947994 0.540861812778603,-0.557206537890045 0.37444279346211,-1.09806835066865 0.0267459138187223,-1.2927191679049 0.197622585438336,-1.2778603268945 0.316493313521545),(-0.922734026745914 0.448736998514116,-1.03566121842496 0.393759286775632,-1.0297176820208 0.329866270430907,-0.87369985141159 0.286775631500743,-0.739970282317979 0.332838038632987,-0.922734026745914 0.448736998514116),(-1.12778603268945 0.295690936106984,-1.21545319465082 0.280832095096583,-1.23476968796434 0.225854383358098,-1.14858841010401 0.184249628528975,-1.0520059435364 0.210995542347697,-1.12778603268945 0.295690936106984)),POINT(-1.22288261515602 0.438335809806835),POINT(-1.1887072808321 0.24962852897474),POINT(-1.09658246656761 0.526002971768202),POINT(-0.967310549777118 0.225854383358098),POINT(-0.936106983655275 0.472511144130758),POINT(-0.882615156017831 0.335809806835067),POINT(-0.821693907875186 0.607726597325409),POINT(-0.708766716196137 0.243684992570579),POINT(-0.643387815750372 0.471025260029718),POINT(-0.632986627043091 0.674591381872214),POINT(-0.476968796433878 0.242199108469539),POINT(-0.456166419019317 0.573551263001486),POINT(-0.349182763744428 0.386329866270431))" ) );
    ConstraintDelaunayTriangulation triangulation = triangulate2DZ( *g );
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 41U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 72U );
//	std::cout << triangulation.getTriangulatedSurface()->asText(5) << std::endl ;
}

//Solid

BOOST_AUTO_TEST_CASE( testSolid )
{

    std::unique_ptr< Geometry > g( io::readWkt( "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                                                      ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                                      ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                                      ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                                      ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                                                      ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );

    BOOST_CHECK_THROW( triangulate2DZ( *g ), GeometryInvalidityException );
}


//MultiSolid


BOOST_AUTO_TEST_SUITE_END()





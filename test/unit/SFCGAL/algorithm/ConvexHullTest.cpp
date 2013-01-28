/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/convexHull.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ConvexHullTest )

BOOST_AUTO_TEST_CASE( testPoint )
{
	Point p(1.0,2.0,3.0);
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D(p) );
	BOOST_CHECK( hull->is< Point >() );
	BOOST_CHECK_EQUAL( hull->as< Point >().x(), 1.0 ) ;
	BOOST_CHECK_EQUAL( hull->as< Point >().y(), 2.0 ) ;
	BOOST_CHECK_EQUAL( hull->as< Point >().z(), 3.0 ) ;
}

/*
 * @todo Test if points are collinear
 */
BOOST_AUTO_TEST_CASE( testLineStringCollinear )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,0.0)  );
	points.push_back( Point(1.0,1.0,1.0)  );
	points.push_back( Point(2.0,2.0,2.0)  );
	points.push_back( Point(3.0,3.0,3.0)  );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< LineString >() );
}

BOOST_AUTO_TEST_CASE( testLineStringCoplanar )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,1.0)  );
	points.push_back( Point(1.0,0.0,1.0)  );
	points.push_back( Point(1.0,1.0,1.0)  );
	points.push_back( Point(0.0,1.0,1.0)  );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< PolyhedralSurface >() );
	BOOST_CHECK_EQUAL( hull->as< PolyhedralSurface >().numPolygons(), 1U );
}




BOOST_AUTO_TEST_CASE( testTetrahedron )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,0.0) );
	points.push_back( Point(1.0,0.0,0.0) );
	points.push_back( Point(0.0,1.0,0.0) );
	points.push_back( Point(0.0,0.0,1.0) );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< PolyhedralSurface >() );
	BOOST_CHECK_EQUAL( hull->as< PolyhedralSurface >().numPolygons(), 4U );

}


BOOST_AUTO_TEST_CASE( testConvexHull2D )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0) );
	points.push_back( Point(0.5,0.5) );
	points.push_back( Point(1.0,0.0) );
	points.push_back( Point(0.0,1.0) );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull( lineString ) );
	BOOST_CHECK( hull->is<Polygon>() );
}

BOOST_AUTO_TEST_SUITE_END()


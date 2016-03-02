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
#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>

#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/detail/generator/sierpinski.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchPredicate )


double randf()
{
    return double(rand()) / RAND_MAX;
}

BOOST_AUTO_TEST_CASE( testPointCompare )
{
    std::vector<CGAL::Point_3<Kernel> > points;
    for ( size_t i = 0; i < 10000; i++ ) {
        points.push_back( CGAL::Point_3<Kernel>( randf(), randf(), randf() ) );
    }

    bench().start( "xyz" ) ;

    for ( size_t i = 0; i < points.size(); i++ ) {
        for ( size_t j = 0; j < points.size(); j++ ) {
            points[i].x() < points[j].x();
        }
    }

    bench().stop();


    bench().start( "compare" ) ;

    for ( size_t i = 0; i < points.size(); i++ ) {
        for ( size_t j = 0; j < points.size(); j++ ) {
            CGAL::compare_x<Kernel>( points[i], points[j] );
        }
    }

    bench().stop();
}

BOOST_AUTO_TEST_SUITE_END()





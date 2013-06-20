/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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
#include <SFCGAL/algorithm/tesselate.h>

#include <SFCGAL/detail/tools/Registry.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_TesselateTest )


BOOST_AUTO_TEST_CASE( testEmpty )
{
	tools::Registry & registry = tools::Registry::instance() ;

	std::vector< std::string > geometryTypes = registry.getGeometryTypes() ;
	for ( size_t i = 0; i < geometryTypes.size(); i++ ){
		std::auto_ptr< Geometry > g( registry.newGeometryByTypeName( geometryTypes[i] ) ) ;
		BOOST_TEST_MESSAGE( boost::format("tesselate(%s)") % g->asText() );
		std::auto_ptr< Geometry > result = algorithm::tesselate( *g ) ;
		BOOST_CHECK_EQUAL( result->asText(), g->asText() );
	}
}


//TODO test invariants (Point,LineString & co)
//TODO test with polygon, MultiPolygon & co



BOOST_AUTO_TEST_SUITE_END()


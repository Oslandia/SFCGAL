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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/translate.h>

#include <SFCGAL/tools/Registry.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_TranslateTest )


BOOST_AUTO_TEST_CASE( testEmpty )
{
	tools::Registry & registry = tools::Registry::instance() ;
	std::vector< std::string > typeNames = tools::Registry::instance().getGeometryTypes();
	for ( size_t i = 0; i < typeNames.size(); i++ ){
		BOOST_TEST_MESSAGE( typeNames[i] ) ;

		std::auto_ptr< Geometry > g( registry.newGeometryByTypeName( typeNames[i] ) ) ;
		BOOST_REQUIRE( g.get() != NULL ) ;
		algorithm::translate( *g, 1.0, 1.0, 1.0 );
		BOOST_CHECK( g->isEmpty() );
	}
}

//TODO complete with 2D/3D test after having renamed translate to translate3D


BOOST_AUTO_TEST_SUITE_END()


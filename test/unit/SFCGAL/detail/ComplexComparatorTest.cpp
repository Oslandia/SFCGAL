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

#include <SFCGAL/detail/ComplexComparator.h>
#include <SFCGAL/numeric.h>

using namespace SFCGAL ;
using namespace SFCGAL::detail ;

// always after CGAL
using namespace boost::unit_test ;


BOOST_AUTO_TEST_SUITE( SFCGAL_detail_ComplexComparatorTest )

BOOST_AUTO_TEST_CASE( testComparator )
{
    ComplexComparator less ;

    BOOST_CHECK( ! less( std::complex< double >( 1.0,0.0 ), std::complex< double >( 0.0,0.0 ) ) );
    BOOST_CHECK( ! less( std::complex< double >( 0.0,0.0 ), std::complex< double >( 0.0,0.0 ) ) );
    BOOST_CHECK( less( std::complex< double >( 0.0,0.0 ), std::complex< double >( 1.0,0.0 ) ) );
    BOOST_CHECK( less( std::complex< double >( 0.0,0.0 ), std::complex< double >( 0.0,1.0 ) ) );
}

BOOST_AUTO_TEST_SUITE_END()


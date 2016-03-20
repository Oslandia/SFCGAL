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

#include <SFCGAL/kernels.h>
#include <SFCGAL/detail/VertexArrayBuilder.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_detail_VertexArrayBuilderTest )

BOOST_AUTO_TEST_CASE( testUnicity3D )
{
    typedef CGAL::Point_3<Epeck> CT ;
    std::vector< CT > vertices ;
    detail::VertexArrayBuilder<CT> builder(vertices);
    
    size_t a = builder.addVertex(CT(0.0,0.0,0.0));
    BOOST_CHECK_EQUAL(a,0);
    
    size_t b = builder.addVertex(CT(1.0,0.0,0.0));
    BOOST_CHECK_EQUAL(b,1);
    
    size_t c = builder.addVertex(CT(0.0,1.0,0.0));
    BOOST_CHECK_EQUAL(c,2);
    
    size_t d = builder.addVertex(CT(0.0,0.0,0.0));
    BOOST_CHECK_EQUAL(d,0);

    BOOST_CHECK_EQUAL(vertices.size(),3);
    BOOST_CHECK_EQUAL(vertices[0],CT(0.0,0.0,0.0));
    BOOST_CHECK_EQUAL(vertices[1],CT(1.0,0.0,0.0));
    BOOST_CHECK_EQUAL(vertices[2],CT(0.0,1.0,0.0));
}


//MultiSolid


BOOST_AUTO_TEST_SUITE_END()





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

#include <SFCGAL/triangulate/triangulate2DZ.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/TriangulatedSurface.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/algorithm/isValid.h>

#include <SFCGAL/triangulate/detail/collectPointsAndConstraints.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>

namespace SFCGAL {
namespace triangulate {

///
///
///
std::auto_ptr< TriangulatedSurface > triangulate2DZ( const Geometry& g )
{
    // TODO
    // typedef CGAL::Projection_traits_xy_3<Kernel>                Projection;
    // typedef CGAL::Constrained_Delaunay_triangulation_2<Projection>  CDT;

    std::vector< Kernel::Point_2 > points ;
    std::vector< std::pair< std::size_t, std::size_t > > constraints ;
    detail::collectPointsAndConstraints(g,points,constraints);

    typedef CGAL::Constrained_Delaunay_triangulation_2< Epeck > CDT ;
    typedef CDT::Finite_faces_iterator   Finite_faces_iterator ;
    
    CDT triangulation ;
    triangulation.insert_constraints(
        points.begin(), points.end(),
        constraints.begin(), constraints.end()
    );
    
    std::auto_ptr< TriangulatedSurface > triangulatedSurface(
        new TriangulatedSurface()
    );
    triangulatedSurface->reserve( triangulation.number_of_faces() );
    for ( Finite_faces_iterator it = triangulation.finite_faces_begin(); it != triangulation.finite_faces_end(); ++it ) {
        Epeck::Point_2 a = it->vertex( 0 )->point() ;
        Epeck::Point_2 b = it->vertex( 1 )->point() ;
        Epeck::Point_2 c = it->vertex( 2 )->point() ;

        triangulatedSurface->addTriangle( new Triangle( 
            Point( a ), 
            Point( b ), 
            Point( c )
        ) );
    }
    return triangulatedSurface ;
}


}//triangulate
}//SFCGAL



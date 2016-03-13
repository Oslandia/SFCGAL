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

#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <CGAL/linear_least_squares_fitting_3.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/detail/collectPoints.h>

#include <SFCGAL/algorithm/normal.h>

namespace SFCGAL {
namespace algorithm {
    
/**
 * Find the best fitting plane for a point set.
 *
 * @return the plane
 */
template < typename K >
CGAL::Plane_3<K> plane3D( 
    const std::vector<CGAL::Point_3<K>> & points
){
    /*
     * This special case allows exact computation of normals for non degenerated triangles.
     *
     * It is also related to a bad precision in linear_least_squares_fitting_3
     * when there are only 3 input points : 
     * http://cgal-discuss.949826.n4.nabble.com/linear-least-squares-fitting-3-gives-bad-fitting-quality-for-input-of-only-3-points-td3829065.html
     */
    if ( points.size() == 3 ){
        return CGAL::Plane_3<K>(
            points[0],
            normal3D<K>(points[0],points[1],points[2])
        );
    }
    
    
    CGAL::Cartesian_converter<K,Epick> to_epick;
    CGAL::Cartesian_converter<Epick,K> to_k;
    // find best fitting plane
    CGAL::Plane_3<Epick> plane;
    typename Epick::FT quality = CGAL::linear_least_squares_fitting_3(
        boost::make_transform_iterator(points.begin(),to_epick),
        boost::make_transform_iterator(points.end(),to_epick),
        plane,
        CGAL::Dimension_tag<0>()
    );
    return to_k(plane);
}

/**
 * Find the best fitting plane for a Geometry
 */
template < typename K >
CGAL::Plane_3<K> plane3D( 
    const Geometry<K> & g
){
    // get points from the geometry
    std::vector<CGAL::Point_3<K>> points;
    detail::collectPoints(g,points);
    // find best fitting plane for the point set
    return plane3D<K>(points);
}


/**
 * Test if all points of a geometry lie in the same plane.
 * TODO toleranceAbs => K::FT maxSquaredDistance
 *
 * @warning empty geometries are considered as planar
 *
 * @ingroup detail
 */
template < typename K >
bool isPlane3D( const Geometry<K>& g, const double& toleranceAbs )
{
    // get points from the geometry
    std::vector<CGAL::Point_3<K>> points;
    detail::collectPoints(g,points);
    // TODO check if this behavior is wanted for empty geometries
    if ( points.empty() ) {
        return true;
    }
    
    // find best fitting plane
    CGAL::Plane_3<K> plane = plane3D<K>(points);
    // can't find a plane
    if ( plane.is_degenerate() ){
        return false ;
    }
    
    // check max distance to plane
    double squaredTol = toleranceAbs * toleranceAbs ;
    for ( const CGAL::Point_3<K> & p : points ){
        typename K::FT squaredDistance = CGAL::squared_distance(p, plane.projection(p)) ;
        if ( squaredDistance > squaredTol ){
            return false;
        }
    }
    return true;
}


}//algorithm
}//SFCGAL


#endif
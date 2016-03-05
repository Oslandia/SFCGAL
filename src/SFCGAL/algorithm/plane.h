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

//#include <SFCGAL/detail/ublas.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/normal.h>
#include <SFCGAL/detail/collectPoints.h>

namespace SFCGAL {
namespace algorithm {

/**
 * @brief Test if a 3D plane can be extracted from a Polygon
 * @ingroup public_api
 */
template < typename K >
bool hasPlane3D( 
    const Polygon<K>& polygon,
    CGAL::Point_3<K>& a,
    CGAL::Point_3<K>& b,
    CGAL::Point_3<K>& c 
)
{
    typedef CGAL::Point_3<K> Point_3 ;

    const LineString<K>& exteriorRing = exteriorRing(polygon) ;

    /*
     * look for 3 non collinear points
     */
    size_t  n = 0 ;

    for ( size_t i = 0; i < exteriorRing.size(); i++ ) {
        const Point_3 & p = exteriorRing[ i ] ;

        if ( n == 0 ) {
            a = p ;
            n++ ;
        }
        else if ( n == 1 && a != p ) {
            b = p ;
            n++ ;
        }
        else if ( n == 2 && ! CGAL::collinear( a, b, p ) ) {
            c = p ;
            n++ ;
            return true ;
        }
    }

    BOOST_ASSERT( n < 3 );
    return false;
}

/**
 * Test if a 3D plane can be extracted from a Polygon
 */
template < typename K >
bool hasPlane3D( const Polygon<K>& polygon )
{
    // temporary arguments
    CGAL::Point_3<K> a, b, c;
    return hasPlane3D( polygon, a, b, c );
}

/**
 * Get 3 non collinear points from a Polygon
 */
template < typename K >
void plane3D(
    const Polygon<K>& polygon,
    CGAL::Point_3<K>& a,
    CGAL::Point_3<K>& b,
    CGAL::Point_3<K>& c
)
{
    if ( ! hasPlane3D( polygon, a, b, c ) ) {
        BOOST_THROW_EXCEPTION(
            Exception(
                ( boost::format( "can't find plane for Polygon '%1%'" ) % polygon.asText( 3 ) ).str()
            )
        );
    }
}

/**
 * Returns the oriented 3D plane of a polygon (supposed to be planar).
 * @warning result is rounded to double if exact is false (avoid huge expression tree)
 */
template < typename K >
CGAL::Plane_3<K> plane3D( const Polygon<K>& polygon, bool exact = true )
{
    CGAL::Vector_3<K> nrml = normal3D<K>( polygon, exact );

    if ( !exact ) {
        const double nrm = std::sqrt( CGAL::to_double( nrml.squared_length() ) );
        nrml = CGAL::Vector_3<K>( nrml.x()/nrm, nrml.y()/nrm, nrml.z()/nrm );
    }

    return CGAL::Plane_3<K>( exteriorRing(polygon).at( 0 ), nrml );
}



/**
 * Test if all points of a geometry lie in the same plane
 
 * TODO review this algorithm (Epeck for the barycenter)
 * 
 * @ingroup detail
 */
template < typename K >
bool isPlane3D( const Geometry<K>& geom,const double& toleranceAbs )
{
    std::vector< Point<K> > points ;
    detail::collectPoints(geom,points);

    if ( points.empty() ) {
        return true;
    }

    // the present approach is to find a good plane by:
    // - computing the centroid C of the point set
    // - finding the farest point F from C
    // - finding the farest point G from (CF)
    // - we define the unit normal N to the plane from CFxCG
    // - we check that points Xi are in the plane CXi.N < tolerance
    //
    // note that we could compute the covarence matrix of the points and use SVD
    // but we would need a lib for that, and it may be overkill

    typedef CGAL::Vector_3<K> Vector_3 ;

    // centroid
    Vector_3 c( 0,0,0 );
    int numPoint = 0;

    for ( auto x = points.begin(); x != points.end(); ++x ) {
        c = c + toVector_3( *x ) ;
        ++numPoint;
    }

    BOOST_ASSERT( numPoint );
    c = c / numPoint;

    // farest point from centroid
    Vector_3 f = c ;
    typename K::FT maxDistanceSq = 0;

    for ( auto x = points.begin(); x != points.end(); ++x ) {
        const Vector_3 cx = toVector_3( *x ) - c ;
        const typename K::FT dSq = cx * cx ;

        if ( dSq > maxDistanceSq ) {
            f = toVector_3( *x ) ;
            maxDistanceSq = dSq ;
        }
    }

    if ( std::sqrt( CGAL::to_double( maxDistanceSq ) ) < toleranceAbs ) {
        // std::cout << "all points in the same location\n";
        return true;
    }

    // farest point from line
    Vector_3 g=c;
    const Vector_3 cf = f - c ; // direction of (CF)
    maxDistanceSq = 0; // watch out, we reuse the variable

    for ( auto x = points.begin(); x != points.end(); ++x ) {
        const Vector_3 cx = toVector_3( *x ) - c ;
        const Vector_3 cp = ( cx * cf ) * cf / cf.squared_length() ; // projection of x on line (CF)
        const typename K::FT dSq = ( cx - cp ).squared_length() ;

        if ( dSq > maxDistanceSq ) {
            g = toVector_3( *x ) ;
            maxDistanceSq = dSq ;
        }
    }

    if ( std::sqrt( CGAL::to_double( maxDistanceSq ) ) < toleranceAbs ) {
        // std::cout << "all points aligned\n";
        return true;
    }

    const Vector_3 n = CGAL::cross_product( cf, g - c );

    const Vector_3 nNormed = n / std::sqrt( CGAL::to_double( n.squared_length() ) );

    for ( auto x = points.begin(); x != points.end(); ++x ) {
        const Vector_3 cx = toVector_3( *x ) - c ;

        if ( std::abs( CGAL::to_double( cx * n ) ) > toleranceAbs ) {
            // std::cout << "point out of plane\n";
            return false;
        }
    }

    // std::cout << "plane general case\n";
    return true;
}


}//algorithm
}//SFCGAL


#endif

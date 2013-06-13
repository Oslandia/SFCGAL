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
#ifndef SFCGAL_INTERSECTION_ALGORITHM
#define SFCGAL_INTERSECTION_ALGORITHM

#include <SFCGAL/config.h>

#include <memory>

namespace SFCGAL {
class Geometry;
 namespace detail {
	 template <int Dim> class GeometrySet;
	 template <int Dim> class PrimitiveHandle;
 }

namespace algorithm {
	/**
	 * Intersection on 2D geometries.
	 * @pre ga and gb are valid geometries
	 * @ingroup public_api
	 */
	SFCGAL_API std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb );
	
	/**
	 * Intersection on 3D geometries. Assume z = 0 if needed
	 * @pre ga and gb are valid geometries
	 * @ingroup public_api
	 */
	SFCGAL_API std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb );
	
	/**
	 * @ingroup detail
	 */
	template <int Dim>
	void intersection( const detail::GeometrySet<Dim>& a, const detail::GeometrySet<Dim>& b, detail::GeometrySet<Dim>& );

	/**
	 * @ingroup detail
	 */
	template <int Dim>
	void intersection( const detail::PrimitiveHandle<Dim>& a, const detail::PrimitiveHandle<Dim>& b, detail::GeometrySet<Dim>& );

//    
//	/**
//     * Intersection of two line segment
//	 * @ingroup detail
//     * @return either an empty Point, a Point or a LineString
//	 */
//    template< int Dim >
//    std::auto_ptr< Geometry > intersectionLineSegments( const Point & p11, const Point & p12, const Point & p21, const Point & p22, const double toleranceAbs )
//    {
//        std::auto_ptr< Geometry > inter( new Point ); // empty point if nothing else
//
//
//        Segment_3<Kernel> s1( , Point_2<Kernel> q); 
//
//        CCGAL::Object out = intersection ( l1, l2);
//
// 
// 		typedef CGAL::Vector_3< Kernel > Vector_3 ;
//         const Vector_3 u = p12.toVector_3() - p11.toVector_3(); 
//         const Vector_3 v = p21.toVector_3() - p11.toVector_3(); 
//         const Vector_3 w = p22.toVector_3() - p11.toVector_3();
// 
//         // points must lie in the same plane
//         const Vector_3 n = CGAL::cross_product( u, v );
//         const Kernel::FT nSq = n.squared_length();
//         const Vector_3 nNormed = n / std::sqrt( CGAL::to_double( nSq ) );
//         if ( std::abs( CGAL::to_double( w * nNormed ) ) > toleranceAbs ) {
//             std::cout << "points not in the same plane\n";
//             return inter;
//         }
// 
//         // now project on the plane and solve the equation
//         const Vector_3 pv =  ( v * n );
//         const Vector_3 pw = ( w * u ) * u / u.squared_length()  ;// projection of point w on u
//         const Kernel::FT dvSq = ( v - pv ).squared_length() ;
//         const Kernel::FT dwSq = ( w - pw ).squared_length() ;
//         if ( std::sqrt( to_double( CGAL::max( dvSq, dwSq ) ) ) < toleranceAbs ) {
//             // all points are aligned
// 
//             const Vector_3 uNormed = u / 
// 
//         }
//         else {
//             // points are not aligned, we solve the linear equation
//         }
//
//        BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
//        BOOST_ASSERT( inter.get() );
//        return inter; // empty point if nothing else
//    }

}
}

#endif

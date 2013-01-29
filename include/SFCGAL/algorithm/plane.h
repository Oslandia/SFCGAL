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
#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>

//#include <SFCGAL/ublas.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/normal.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Test if a 3D plane can be extracted from a Polygon
	 */
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon,
			 CGAL::Point_3< Kernel > & a,
			 CGAL::Point_3< Kernel > & b,
			 CGAL::Point_3< Kernel > & c )
	{
		typedef CGAL::Point_3< Kernel > Point_3 ;

		const LineString & exteriorRing = polygon.exteriorRing() ;

		/*
		 * look for 3 non collinear points
		 */
		size_t  n = 0 ;
		for ( size_t i = 0; i < exteriorRing.numPoints(); i++ ){
			Point_3 p = exteriorRing.pointN(i).toPoint_3() ;
			if ( n == 0 ){
				a = p ;
				n++ ;
			}else if ( n == 1 && a != p ){
				b = p ;
				n++ ;
			}else if ( n == 2 && ! CGAL::collinear( a, b, p ) ) {
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
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon )
	{
		// temporary arguments
		CGAL::Point_3< Kernel > a, b, c;
		return hasPlane3D( polygon, a, b, c );
	}

	/**
	 * Get 3 non collinear points from a Polygon
	 */
	template < typename Kernel >
	void plane3D(
		const Polygon & polygon,
		CGAL::Point_3< Kernel > & a,
		CGAL::Point_3< Kernel > & b,
		CGAL::Point_3< Kernel > & c
	)
	{
		if ( ! hasPlane3D( polygon, a, b, c) ){
			BOOST_THROW_EXCEPTION(
				Exception(
					( boost::format("can't find plane for Polygon '%1%'") % polygon.asText(3) ).str()
				)
			);
		}
	}

	/**
	 * Returns the oriented 3D plane of a polygon (supposed to be planar).
	 * @warning result is rounded to double if exact is false (avoid huge expression tree)
	 */
	template < typename Kernel >
	CGAL::Plane_3< Kernel > plane3D( const Polygon & polygon, bool exact = true )
	{
		CGAL::Vector_3< Kernel > normal = normal3D< Kernel >( polygon, exact );
		return CGAL::Plane_3< Kernel >( polygon.exteriorRing().pointN(0).toPoint_3(), normal );
	}



}//algorithm
}//SFCGAL


#endif

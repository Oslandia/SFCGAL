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
#include <iostream>

//#include <CGAL/Cartesian.h>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/bounding_box.h>
#include <CGAL/box_intersection_d.h>

/**
 * Defines the default Kernel used by SFCGAL
 * @todo allow to choose between differents Kernels
 */
//typedef CGAL::Cartesian< double > Kernel ;
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel ;

/**
 * Defines a 2D mathematical vector
 */
typedef Kernel::Vector_2          Vector_2 ;
/**
 * Defines a 3D mathematical vector
 */
typedef Kernel::Vector_3          Vector_3 ;

/**
 * Defines the Point_2 in the default Kernel
 */
typedef Kernel::Point_2           Point_2 ;
/**
 * Defines the Point_3 in the default Kernel
 */
typedef Kernel::Point_3           Point_3 ;

typedef Kernel::Triangle_3        Triangle_3 ;

/**
 * Defines a Segment_2 in the default Kernel
 */
typedef CGAL::Segment_2< Kernel > Segment_2 ;
/**
 * Defines a Segment_3 in the default Kernel
 */
typedef CGAL::Segment_3< Kernel > Segment_3 ;

/**
 * A polygon defined by an exterior ring
 */
typedef CGAL::Polygon_2< Kernel >            Polygon_2 ;
/**
 * A polygon defined by an exterior ring and interior rings
 */
typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;


int main(){

	///------------- 2D -----------------------------------------------------------------------

	std::cout << "--- Vector_2 ----" << std::endl;
	{
		Vector_2 a( 1.0, 1.0 );
		Vector_2 b( 3.0, 4.0 );
		std::cout << "a   : " << a << std::endl;
		std::cout << "b   : " << b << std::endl;
		std::cout << "a+b : " << (a+b) << std::endl;

		std::cout << "b.squared_length() : "<< a.squared_length() << std::endl ;
	}

	std::cout << "--- Point_2 ----" << std::endl;
	{
		Point_2 a( 0.0, 0.0 );
		Point_2 b( 3.0, 4.0 );
		std::cout << "a : " << a << std::endl ;
		std::cout << "b : " << b << std::endl ;
	}

	std::cout << "--- Segment_2 ----" << std::endl;
	{
		Segment_2 ab(
			Point_2( 0.0, 0.0 ),
			Point_2( 1.0, 1.0 )
		);
		std::cout << "ab : " << ab << std::endl;
	}



	std::cout << "--- Polygon_2 ----" << std::endl;
	{
		std::vector< Point_2 > exteriorRing ;
		exteriorRing.push_back( Point_2(0.0,0.0) );
		exteriorRing.push_back( Point_2(1.0,0.0) );
		exteriorRing.push_back( Point_2(0.5,1.0) );
		// CGAL polygon are not closed duplicating last point
		//exteriorRing.push_back( Point_2(0.0,0.0) );

		Polygon_2 polygon( exteriorRing.begin(), exteriorRing.end() );

		//some basic test
		std::cout << "polygon.area()                  : " << polygon.area() << std::endl ;
		std::cout << "polygon.is_convex()             : " << polygon.is_convex() << std::endl ;
		std::cout << "polygon.is_clockwise_oriented() : " << polygon.is_clockwise_oriented() << std::endl ;
		std::cout << "polygon.bbox()                  : " << polygon.bbox() << std::endl ;
		std::cout << "polygon.is_simple()             : " << polygon.is_simple() << std::endl ;

		//polygon.bottom_vertex();
		//polygon.bounded_side();

		std::cout << polygon << std::endl ;
	}

	std::cout << "--- Polygon_with_holes_2 ----" << std::endl;
	{
		//create rings
		std::vector< Point_2 > exteriorRing ;
		exteriorRing.push_back( Point_2( 0.0, 0.0 ) );
		exteriorRing.push_back( Point_2( 1.0, 0.0 ) );
		exteriorRing.push_back( Point_2( 1.0, 1.0 ) );
		exteriorRing.push_back( Point_2( 0.0, 1.0 ) );

		std::vector< Point_2 > interiorRing ;
		interiorRing.push_back( Point_2( 0.2, 0.2 ) );
		interiorRing.push_back( Point_2( 0.8, 0.2 ) );
		interiorRing.push_back( Point_2( 0.8, 0.8 ) );
		interiorRing.push_back( Point_2( 0.2, 0.8 ) );

		// create polygon
		Polygon_with_holes_2 polygon( Polygon_2( exteriorRing.begin(), exteriorRing.end() ) );
		polygon.add_hole( Polygon_2( interiorRing.begin(), interiorRing.end() ) );

		// print wkt
		std::cout << polygon << std::endl ;
	}


	///------------- 3D -----------------------------------------------------------------------


	std::cout << "--- Point_3 ----" << std::endl;
	{
		Point_3 a( 0.0, 0.0, 0.0 );
		Point_3 b( 3.0, 4.0, 5.0 );
		std::cout << "a : " << a << std::endl ;
		std::cout << "b : " << b << std::endl ;

		std::cout << "a < b : " << ( a < b ) << std::endl;
	}
	std::cout << "--- Segment_3 ----" << std::endl;
	{
		Segment_3 ab(
			Point_3( 0.0, 0.0, 0.0 ),
			Point_3( 3.0, 4.0, 5.0 )
		);

		std::cout << ab.source() << " => " << ab.target() << std::endl;
		std::cout << ab << std::endl;
	}
	std::cout << "--- Triangle_3 ---" << std::endl;
	{
	    Triangle_3 tri( Point_3( 0.0, 0.0, 1.0),
			    Point_3( 1.0, 0.0, 1.0),
			    Point_3( 0.0, 1.0, 1.0) );
	    Point_3 p( 0.2, 0.2, 1.0 );
	    Point_3 p2( 0.2, 0.2, 0.0 );
	    std::cout << "p  : " << p << std::endl;
	    std::cout << "p2 : " << p2 << std::endl;
	    std::cout << "tri: " << tri << std::endl;
	    std::cout << "tri.has_on(p) : " << tri.has_on( p ) << std::endl;
	    std::cout << "tri.has_on(p2): " << tri.has_on( p2 ) << std::endl;
	}
	std::cout << "--- 2D Arrangements ---" << std::endl;
	{
	    typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
	    typedef Traits_2::Point_2                  APoint_2;
	    typedef Traits_2::X_monotone_curve_2       ASegment_2;
	    typedef CGAL::Arrangement_2<Traits_2>      Arrangement_2;
	    
	    Arrangement_2 arr;
	    std::vector<ASegment_2> segs;
	    segs.push_back( ASegment_2( APoint_2( 0.0, 0.0 ), APoint_2( 2.0, 0.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 2.0, 0.0 ), APoint_2( 1.0, 1.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 1.0, 1.0 ), APoint_2( 1.0, -1.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 1.0, -1.0 ), APoint_2( 0.0, 0.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 1.0, -1.0 ), APoint_2( 0.0, 0.0 ) ) );
	    CGAL::insert( arr, segs.begin(), segs.end() );

	    std::cout << "# of vertices: " << arr.number_of_vertices() << std::endl;
	    std::cout << "# of faces: " << arr.number_of_faces() << std::endl;
	    std::cout << "autointersects ? " << (arr.number_of_vertices() > 4 ? "YES" : "NO") << std::endl;

	    arr.clear();
	    segs.clear();
	    // a triangle
	    segs.push_back( ASegment_2( APoint_2( 0.0, 0.0 ), APoint_2( 1.0, 1.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 1.0, 1.0 ), APoint_2( 1.0, 0.0 ) ) );
	    segs.push_back( ASegment_2( APoint_2( 1.0, 0.0 ), APoint_2( 0.0, 0.0 ) ) );
	    // and a segment contained in the triangle
	    segs.push_back( ASegment_2( APoint_2( 0.55, 0.5 ), APoint_2( 0.6, 0.5 ) ) );
	    CGAL::insert( arr, segs.begin(), segs.end() );
	    std::cout << "# of vertices: " << arr.number_of_vertices() << std::endl;
	    std::cout << "# of faces: " << arr.number_of_faces() << std::endl;
	    Arrangement_2::Face_iterator it;
	    for ( it = arr.faces_begin(); it != arr.faces_end(); it++ ) {
		std::cout << "unbounded: " << it->is_unbounded() << " has holes: " << (it->holes_begin() != it->holes_end()) << std::endl;
	    }
	}
	return 0;
}

#include <iostream>

#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

/**
 * Defines the default Kernel used by SFCGAL
 * @todo allow to choose between differents Kernels
 */
typedef CGAL::Cartesian< double > Kernel ;

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


int main( int argc, char* argv[] ){

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


	return 0;
}

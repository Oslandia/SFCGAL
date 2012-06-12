#include <iostream>

#include <SFCGAL/Kernel.h>

using namespace SFCGAL ;

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


	return 0;
}

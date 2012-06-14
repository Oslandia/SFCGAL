#include <CGAL/Cartesian.h>
#include <CGAL/squared_distance_2.h>



//declaration Kernel simple
typedef CGAL::Cartesian< double > K ;
typedef K::Point_2 Point_2 ;


int main( int argc, char* argv[] ){
	Point_2 a( 0.0, 0.0 );
	Point_2 b( 3.0, 4.0 );
	K::Compute_squared_distance_2 squared_distance;
	std::cout << squared_distance( a, b ) << std::endl;	
	return 0;
}


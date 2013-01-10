#include <iostream>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <CGAL/point_generators_2.h>

#include "bench.h"

using namespace SFCGAL ;

typedef CGAL::Creator_uniform_2< double, Kernel::Point_2 > Creator ;


void benchMultiPointTriangulation( int n ){
	CGAL::Random_points_in_disc_2< Kernel::Point_2, Creator > g( 150.0 );

	MultiPoint multiPoint ;
	for ( int i = 0; i < n; i++ ){
		Kernel::Point_2 p = *(++g) ;
		multiPoint.addGeometry( new Point( p ) ) ;
	}

	TriangulatedSurface triangulatedSurface ;
	SFCGAL_BENCH( SFCGAL::algorithm::triangulate( multiPoint, triangulatedSurface ) ) ;
}


int main( int argc, char* argv[] ){

	SFCGAL_BENCH( benchMultiPointTriangulation(1000) ) ;
	SFCGAL_BENCH( benchMultiPointTriangulation(10000) ) ;
	SFCGAL_BENCH( benchMultiPointTriangulation(100000) ) ;

	return 0 ;
}

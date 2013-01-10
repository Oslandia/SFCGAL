#include <iostream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include "bench.h"

using namespace SFCGAL ;

void benchReadPoint( int n ){
	for ( int i = 0; i < n; i++ ){
		io::readWkt( "POINT(50000 50000)" ) ;
	}
}

void benchReadLineString( int n ){
	for ( int i = 0; i < n; i++ ){
		io::readWkt( "LINESTRING(0 0,0 1000,1000 1000)" ) ;
	}
}

void benchReadPolygon( int n ){
	for ( int i = 0; i < n; i++ ){
		io::readWkt( "POLYGON((0 0,0 1000,1000 1000,1000 0,0 0))" ) ;
	}
}


int main( int argc, char* argv[] ){
	SFCGAL_BENCH( benchReadPoint(1000) )
	SFCGAL_BENCH( benchReadPoint(10000) )
	SFCGAL_BENCH( benchReadPoint(100000) )

	SFCGAL_BENCH( benchReadLineString(1000) )
	SFCGAL_BENCH( benchReadLineString(10000) )
	SFCGAL_BENCH( benchReadLineString(100000) )

	SFCGAL_BENCH( benchReadPolygon(1000) )
	SFCGAL_BENCH( benchReadPolygon(10000) )
	SFCGAL_BENCH( benchReadPolygon(100000) )
	return 0 ;
}

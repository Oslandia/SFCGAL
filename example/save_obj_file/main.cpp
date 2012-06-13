#include <memory>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WaveFrontObj.h>

using namespace SFCGAL ;

int main() {
	std::string wkt( "MULTIPOLYGON(((17.6999999999534 21.2000000001863,0 5.70000000018626,4.19999999995343 0,10.4000000000233 4.79999999981374,22.4000000000233 13.2999999998137,17.6999999999534 21.2000000001863)))" );

	std::auto_ptr< Geometry > g( io::readWkt( wkt ) ) ;

	io::WaveFrontObj obj;
	obj.addGeometry( g->as< MultiPolygon >() ) ;
	obj.save( "triangulation3d_in_plane.obj" );

	return 0;
}

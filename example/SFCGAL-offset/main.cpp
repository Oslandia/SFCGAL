#include <SFCGAL/all.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/offset.h>

using namespace SFCGAL ;

int main(){
	std::auto_ptr< Geometry > g( io::readWkt("MULTIPOINT(0 0,5 6,3 2,7 1,4 1,3 5,2 9)") );

	for ( size_t i = 1; i <= 50; i++ ){
		std::auto_ptr< Geometry > buffer( algorithm::offset( *g, 0.2*i ) );
		std::cout << buffer->asText(5) << std::endl ;
	}

	return 0 ;
}

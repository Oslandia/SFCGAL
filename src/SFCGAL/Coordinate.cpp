#include <SFCGAL/Coordinate.h>


namespace SFCGAL {

///
///
///
void writeText( std::ostream & s, const Coordinate & coordinate )
{
	for ( size_t i = 0; i < coordinate.size(); i++ ){
		if ( i != 0 )
			s << " " ;
		s << coordinate[i] ;
	}
}


}//SFCGAL




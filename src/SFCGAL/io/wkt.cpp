#include <SFCGAL/io/wkt.h>

#include <SFCGAL/io/WktReader.h>
#include <SFCGAL/io/WktWriter.h>


namespace SFCGAL {
namespace io {

///
///
///
std::auto_ptr< Geometry > readWkt( std::istream & s )
{
	WktReader wktReader(s);
	return std::auto_ptr< Geometry >( wktReader.readGeometry() );
}

///
///
///
std::auto_ptr< Geometry > readWkt( const std::string & s )
{
	std::istringstream iss(s);
	WktReader wktReader(iss);
	return std::auto_ptr< Geometry >( wktReader.readGeometry() );
}


}//io
}//SFCGAL



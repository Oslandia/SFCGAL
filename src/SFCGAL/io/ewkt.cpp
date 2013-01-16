#include <SFCGAL/io/ewkt.h>

#include <SFCGAL/io/WktReader.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/tools/CharArrayBuffer.h>

namespace SFCGAL {
namespace io {

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( std::istream & s )
{
	WktReader wktReader(s);
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( const std::string & s )
{
	std::istringstream iss(s);
	WktReader wktReader(iss);
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( char* str, size_t len )
{
	CharArrayBuffer buf( str, str + len );
	std::istream istr( &buf );
	WktReader wktReader( istr );
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

}//io
}//SFCGAL



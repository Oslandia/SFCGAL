#ifndef _SFCGAL_IO_EWKT_H_
#define _SFCGAL_IO_EWKT_H_

#include <sstream>
#include <string>
#include <memory>

namespace SFCGAL {
	class Geometry ;
	class PreparedGeometry ;
}

namespace SFCGAL {
namespace io {
	/**
	 * Read a EWKT prepared geometry from an input stream
	 */
	std::auto_ptr< PreparedGeometry > readEwkt( std::istream & s ) ;
	/**
	 * Read a EWKT geometry from a string
	 */
	std::auto_ptr< PreparedGeometry > readEwkt( const std::string & s ) ;
	/**
	 * Read a EWKT geometry from a char*
	 */
	std::auto_ptr< PreparedGeometry > readEwkt( char*, size_t );
}
}

#endif

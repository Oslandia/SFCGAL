#ifndef _SFCGAL_IO_WKT_H_
#define _SFCGAL_IO_WKT_H_

#include <sstream>
#include <string>
#include <memory>

namespace SFCGAL {
	class Geometry ;
}

namespace SFCGAL {
namespace io {
	/**
	 * Read a WKT geometry from an input stream
	 */
	std::auto_ptr< Geometry > readWkt( std::istream & s ) ;
	/**
	 * Read a WKT geometry from a string
	 */
	std::auto_ptr< Geometry > readWkt( const std::string & s ) ;
}
}

#endif

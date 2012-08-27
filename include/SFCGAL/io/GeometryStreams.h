#ifndef _SFCGAL_GEOMETRY_STREAMS_H_
#define _SFCGAL_GEOMETRY_STREAMS_H_

#include <ostream>

namespace SFCGAL {

	class Envelope;
	class Geometry;
	
	/**
	 * Ostream operator for Envelope;
	 */
	std::ostream& operator<< (std::ostream&, const Envelope& );

	/**
	 * Ostream operator for Geometry;
	 */
	std::ostream& operator<< (std::ostream&, const Geometry& );
}

#endif

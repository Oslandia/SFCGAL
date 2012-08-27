#include <SFCGAL/io/GeometryStreams.h>

namespace SFCGAL
{

    ///
    ///
    ///
    std::ostream& operator<<( std::ostream& ostr, const Envelope& env )
    {
	return env.print( ostr );
    }
    
    ///
    ///
    ///
    std::ostream& operator<<( std::ostream& ostr, const Geometry& g )
    {
	ostr << g.asText();
	return ostr;
    }

}

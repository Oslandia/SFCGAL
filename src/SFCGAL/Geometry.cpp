#include <SFCGAL/Geometry.h>

#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/io/WktWriter.h>

#include <SFCGAL/algorithm/BoundaryVisitor.h>

namespace SFCGAL {

///
///
///
Geometry::~Geometry()
{

}

///
///
///
std::string Geometry::asText( const int & numDecimals ) const
{
	std::ostringstream oss;
	if ( numDecimals >= 0 ){
		oss << std::fixed ;
		oss.precision( numDecimals );
	}
	io::WktWriter writer( oss );
	writer.write( *this );
	return oss.str();
}

///
///
///
Geometry* Geometry::boundary() const
{
	algorithm::BoundaryVisitor visitor ;
	accept(visitor);
	return visitor.releaseBoundary() ;
}


///
///
///
Geometry::Geometry()
{

}

///
///
///
Geometry::Geometry( Geometry const& other )
{

}



}//SFCGAL


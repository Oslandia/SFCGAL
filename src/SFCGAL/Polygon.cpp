#include <SFCGAL/Polygon.h>

namespace SFCGAL {

///
///
///
Polygon::Polygon():
	Surface(),
	_rings(1,LineString())
{

}

///
///
///
Polygon::Polygon( const std::vector< LineString > & rings ):
	_rings(rings)
{
	if ( _rings.empty() ){
		_rings.resize(1,LineString());
	}
}

///
///
///
Polygon::Polygon( const LineString & exteriorRing ):
	Surface(),
	_rings(1,exteriorRing)
{

}

///
///
///
Polygon::Polygon( Polygon const& other ):
	Surface(other),
	_rings(other._rings)
{

}

///
///
///
Polygon& Polygon::operator = ( const Polygon & other )
{
	_rings = other._rings ;
	return *this ;
}

///
///
///
Polygon::~Polygon()
{

}

///
///
///
int Polygon::coordinateDimension() const
{
	return _rings[0].coordinateDimension() ;
}


///
///
///
std::string Polygon::geometryType() const
{
	return "Polygon" ;
}

///
///
///
GeometryType Polygon::geometryTypeId() const
{
	return TYPE_POLYGON ;
}

///
///
///
Polygon * Polygon::clone() const
{
	return new Polygon(*this);
}

///
///
///
bool   Polygon::isEmpty() const
{
	return exteriorRing().isEmpty() ;
}

///
///
///
bool   Polygon:: is3D() const
{
	return exteriorRing().is3D() ;
}




}//SFCGAL


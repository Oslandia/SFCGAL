#include <SFCGAL/MultiLineString.h>

namespace SFCGAL {

///
///
///
MultiLineString::MultiLineString():
	GeometryCollection()
{

}

///
///
///
MultiLineString::MultiLineString( MultiLineString const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiLineString& MultiLineString::operator = ( const MultiLineString & other )
{
	assign(other) ;
	return *this ;
}

///
///
///
MultiLineString::~MultiLineString()
{

}

///
///
///
MultiLineString *   MultiLineString::clone() const
{
	return new MultiLineString(*this);
}

///
///
///
std::string    MultiLineString::geometryType() const
{
	return "MultiLineString" ;
}

///
///
///
GeometryType   MultiLineString::geometryTypeId() const
{
	return TYPE_MULTILINESTRING ;
}

///
///
///
bool           MultiLineString::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_LINESTRING ;
}


}//SFCGAL


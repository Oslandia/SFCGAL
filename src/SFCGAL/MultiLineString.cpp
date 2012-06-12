#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/GeometryVisitor.h>

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

///
///
///
void MultiLineString::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void MultiLineString::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL


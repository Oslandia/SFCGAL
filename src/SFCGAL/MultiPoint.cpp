#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/GeometryVisitor.h>


namespace SFCGAL {

///
///
///
MultiPoint::MultiPoint():
	GeometryCollection()
{

}

///
///
///
MultiPoint::MultiPoint( MultiPoint const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiPoint& MultiPoint::operator = ( const MultiPoint & other )
{
	assign(other) ;
	return *this ;
}

///
///
///
MultiPoint::~MultiPoint()
{

}

///
///
///
MultiPoint *   MultiPoint::clone() const
{
	return new MultiPoint(*this);
}

///
///
///
std::string    MultiPoint::geometryType() const
{
	return "MultiPoint" ;
}

///
///
///
GeometryType   MultiPoint::geometryTypeId() const
{
	return TYPE_MULTIPOINT ;
}

///
///
///
bool           MultiPoint::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_POINT ;
}



///
///
///
void MultiPoint::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void MultiPoint::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL


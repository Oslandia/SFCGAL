#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
MultiSolid::MultiSolid():
	GeometryCollection()
{

}

///
///
///
MultiSolid::MultiSolid( MultiSolid const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiSolid& MultiSolid::operator = ( const MultiSolid & other )
{
	assign(other) ;
	return *this ;
}

///
///
///
MultiSolid::~MultiSolid()
{

}

///
///
///
MultiSolid *   MultiSolid::clone() const
{
	return new MultiSolid(*this);
}

///
///
///
std::string    MultiSolid::geometryType() const
{
	return "MultiSolid" ;
}

///
///
///
GeometryType   MultiSolid::geometryTypeId() const
{
	return TYPE_MULTISOLID ;
}

///
///
///
bool           MultiSolid::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_SOLID ;
}

///
///
///
void MultiSolid::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void MultiSolid::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL


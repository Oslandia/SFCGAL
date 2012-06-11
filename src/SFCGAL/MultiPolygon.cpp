#include <SFCGAL/MultiPolygon.h>

namespace SFCGAL {

///
///
///
MultiPolygon::MultiPolygon():
	GeometryCollection()
{

}

///
///
///
MultiPolygon::MultiPolygon( MultiPolygon const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiPolygon& MultiPolygon::operator = ( const MultiPolygon & other )
{
	assign(other) ;
	return *this ;
}

///
///
///
MultiPolygon::~MultiPolygon()
{

}

///
///
///
MultiPolygon *   MultiPolygon::clone() const
{
	return new MultiPolygon(*this);
}

///
///
///
std::string    MultiPolygon::geometryType() const
{
	return "MultiPolygon" ;
}

///
///
///
GeometryType   MultiPolygon::geometryTypeId() const
{
	return TYPE_MULTIPOLYGON ;
}

///
///
///
bool           MultiPolygon::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_POLYGON ;
}


}//SFCGAL


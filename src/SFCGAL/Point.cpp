#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
Point::Point():
	_coordinates(3), _is3D(false), _isEmpty(true)
{
	_coordinates[0] = 0.0;
	_coordinates[1] = 0.0;
	_coordinates[2] = 0.0;
}

///
///
///
Point::Point( const double & x, const double & y ):
	_coordinates(3), _is3D(false), _isEmpty(false)
{
	_coordinates[0] = x ;
	_coordinates[1] = y ;
	_coordinates[2] = 0.0;
}

///
///
///
Point::Point( const double & x, const double & y, const double & z ):
	_coordinates(3), _is3D(true), _isEmpty(false)
{
	_coordinates[0] = x ;
	_coordinates[1] = y ;
	_coordinates[2] = z ;
}

///
///
///
Point::Point( const Point & other ):
	_coordinates(other._coordinates), _is3D(other._is3D), _isEmpty(other._isEmpty)
{

}

///
///
///
Point& Point::operator = ( const Point & other )
{
	_coordinates = other._coordinates ;
	_is3D = other._is3D;
	_isEmpty = other._isEmpty;
	return *this ;
}

///
///
///
Point::~Point()
{

}


///
///
///
Point * Point::clone() const
{
	return new Point(*this);
}

///
///
///
std::string Point::geometryType() const
{
	return "Point";
}

///
///
///
GeometryType Point::geometryTypeId() const
{
	return TYPE_POINT ;
}

///
///
///
int Point::dimension() const
{
	return 0 ;
}

///
///
///
int Point::coordinateDimension() const
{
	return _coordinates.size();
}


///
///
///
bool Point::isEmpty() const
{
	return _isEmpty;
}

///
///
///
bool Point::is3D() const
{
	return _is3D;
}




///
///
///
bool Point::operator == (const Point & other ) const
{
	if ( _coordinates.size() != other._coordinates.size() ){
		return false ;
	}
	for ( size_t i = 0; i < _coordinates.size(); i++ ){
		if ( _coordinates[i] != other._coordinates[i] )
			return false ;
	}
	return true ;
}


///
///
///
bool Point::operator != (const Point &p ) const
{
	return !(*this == p);
}


///
///
///
void Point::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void Point::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}



}//SFCGAL


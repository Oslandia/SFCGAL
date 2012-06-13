#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>

#include <SFCGAL/Exception.h>


namespace SFCGAL {

///
///
///
Point::Point():
	_x( NaN() ),
	_y( NaN() ),
	_z( NaN() )
{
}

///
///
///
Point::Point( const double & x, const double & y, const double & z ):
	_x(x),
	_y(y),
	_z(z)
{

}


///
///
///
Point::Point( const Point & other ):
	_x(other._x),
	_y(other._y),
	_z(other._z)
{

}

///
///
///
Point& Point::operator = ( const Point & other )
{
	_x = other._x ;
	_y = other._y ;
	_z = other._z ;
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
	if ( isEmpty() ){
		return 0 ;
	}else {
		return is3D() ? 3 : 2 ;
	}
}


///
///
///
bool Point::isEmpty() const
{
	return isNaN(_x) || isNaN(_y);
}

///
///
///
bool Point::is3D() const
{
	return ! isEmpty() && ! isNaN(_z) ;
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

///
///
///
bool Point::operator < ( const Point & other ) const
{
	// no empty comparison
	if ( isEmpty() || other.isEmpty() ){
		BOOST_THROW_EXCEPTION( Exception("try to compare empty points using a < b ") );
	}

	// no mixed dimension comparison
	if ( ( is3D() && ! other.is3D() ) || ( ! is3D() && other.is3D() ) ){
		BOOST_THROW_EXCEPTION( Exception("try to compare empty points with different coordinate dimension using a < b") );
	}

	// comparison along x
	if ( _x < other._x ){
		return true ;
	}else if ( other._x < _x ){
		return false;
	}

	// comparison along y
	if ( _y < other._y ){
		return true ;
	}else if ( other._y < _y ){
		return false;
	}

	// comparison along z if possible
	if ( is3D() ){
		if ( _z < other._z ){
			return true ;
		}else if ( other._z < _z ){
			return false;
		}
	}

	// points are equals
	return false;
}

///
///
///
bool Point::operator == ( const Point & other ) const
{
	if ( isEmpty() ){
		return other.isEmpty() ;
	}
	if ( is3D() || other.is3D() ){
		return _x == other._x && _y == other._y  && _z == other._z ;
	}else{
		return _x == other._x && _y == other._y ;
	}
}

///
///
///
bool Point::operator != ( const Point & other ) const
{
	return ! ( *this == other );
}


}//SFCGAL


#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Exception.h>


namespace SFCGAL {

///
///
///
Coordinate::Coordinate():
	_x( NaN() ),
	_y( NaN() ),
	_z( NaN() )
{
}

///
///
///
Coordinate::Coordinate( const double & x, const double & y, const double & z ):
	_x(x),
	_y(y),
	_z(z)
{

}


///
///
///
Coordinate::Coordinate( const Coordinate & other ):
	_x(other._x),
	_y(other._y),
	_z(other._z)
{

}

///
///
///
Coordinate& Coordinate::operator = ( const Coordinate & other )
{
	_x = other._x ;
	_y = other._y ;
	_z = other._z ;
	return *this ;
}

///
///
///
Coordinate::~Coordinate()
{

}


///
///
///
int Coordinate::coordinateDimension() const
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
bool Coordinate::isEmpty() const
{
	return isNaN(_x) || isNaN(_y);
}

///
///
///
bool Coordinate::is3D() const
{
	return ! isEmpty() && ! isNaN(_z) ;
}

///
///
///
bool Coordinate::operator < ( const Coordinate & other ) const
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
bool Coordinate::operator == ( const Coordinate & other ) const
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
bool Coordinate::operator != ( const Coordinate & other ) const
{
	return ! ( *this == other );
}


}//SFCGAL


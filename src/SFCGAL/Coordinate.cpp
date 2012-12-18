#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Exception.h>


namespace SFCGAL {

///
///
///
Coordinate::Coordinate():
	_dimension(0),
	_x(0),
	_y(0),
	_z(0)
{
}

///
///
///
Coordinate::Coordinate( const Kernel::FT & x, const Kernel::FT & y ):
	_dimension(2),
	_x(x),
	_y(y),
	_z(0)
{

}

///
///
///
Coordinate::Coordinate( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ):
	_dimension(3),
	_x(x),
	_y(y),
	_z(z)
{

}


///
///
///
Coordinate::Coordinate( const double & x, const double & y, const double & z )
{
	if ( isNaN(x) || isNaN(y) ){
		BOOST_ASSERT( isNaN(x) && isNaN(y) );
	}else{
		if ( isNaN(z) ){
			_dimension = 2 ;
			_x = x ;
			_y = y ;
			_z = 0 ;
		}else{
			_dimension = 3 ;
			_x = x ;
			_y = y ;
			_z = z ;
		}
	}
}


///
///
///
Coordinate::Coordinate( const Coordinate & other ):
	_dimension(other._dimension),
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
	_dimension = other._dimension ;
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
	return _dimension ;
}


///
///
///
bool Coordinate::isEmpty() const
{
	return _dimension == 0 ;
}

///
///
///
bool Coordinate::is3D() const
{
	return _dimension > 2 ;
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
	if ( x() < other.x() ){
		return true ;
	}else if ( other.x() < x() ){
		return false;
	}

	// comparison along y
	if ( y() < other.y() ){
		return true ;
	}else if ( other.y() < y() ){
		return false;
	}

	// comparison along z if possible
	if ( is3D() ){
		if ( z() < other.z() ){
			return true ;
		}else if ( other.z() < z() ){
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
		return x() == other.x() && y() == other.y()  && z() == other.z() ;
	}else{
		return x() == other.x() && y() == other.y() ;
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


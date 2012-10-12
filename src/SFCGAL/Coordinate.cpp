#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Exception.h>


namespace SFCGAL {

///
///
///
Coordinate::Coordinate():
	_xyz()
{
}

///
///
///
Coordinate::Coordinate( const double & x, const double & y, const double & z )
{
	if ( isNaN(x) || isNaN(y) ){
		BOOST_ASSERT( isNaN(x) && isNaN(y) );
		_xyz = detail::Empty() ;
	}else{
		if ( isNaN(z) ){
			_xyz = Kernel::Point_2( x, y ) ;
		}else{
			_xyz = Kernel::Point_3( x, y, z ) ;
		}
	}
}


///
///
///
Coordinate::Coordinate( const Coordinate & other ):
	_xyz(other._xyz)
{

}

///
///
///
Coordinate& Coordinate::operator = ( const Coordinate & other )
{
	_xyz = other._xyz ;
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
	return detail::isEmpty( _xyz );
}

///
///
///
bool Coordinate::is3D() const
{
	return detail::is3D( _xyz );
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


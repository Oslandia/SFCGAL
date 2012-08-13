#include <SFCGAL/detail/Interval.h>
#include <SFCGAL/numeric.h>

#include <algorithm>


namespace SFCGAL {
namespace detail {

///
///
///
Interval::Interval():
	_lower( NaN() ),
	_upper( NaN() )
{

}

///
///
///
Interval::Interval( const double & value ):
	_lower(value),
	_upper(value)
{

}

///
///
///
Interval::Interval( const double & v1, const double & v2 ):
	_lower( std::min(v1,v2) ),
	_upper( std::max(v1,v2) )
{

}


///
///
///
Interval::Interval( const Interval & other ):
	_lower(other._lower),
	_upper(other._upper)
{

}


///
///
///
Interval& Interval::operator = ( const Interval & other )
{
	_lower = other._lower ;
	_upper = other._upper ;
	return *this ;
}


///
///
///
Interval::~Interval()
{

}


///
///
///
bool Interval::isEmpty() const
{
	return isNaN(_lower) || isNaN(_upper);
}



///
///
///
void  Interval::expandBy( const double & d )
{
	if ( isEmpty() )
		return ;

	_lower = _lower - d ;
	_upper = _upper + d ;
}

///
///
///
void  Interval::expandToInclude( const Interval & other )
{
	//ignore empty interval
	if ( other.isEmpty() )
		return ;

	if ( isEmpty() ){
		(*this) = other ;
	}else{
		_lower = std::min( _lower, other._lower );
		_upper = std::max( _upper, other._upper );
	}
}



///
///
///
void  Interval::expandToInclude( const double & value )
{
	if ( isNaN(value) )
		return ;

	if ( isEmpty() ){
		_lower = value ;
		_upper = value ;
	}else{
		_lower = std::min( _lower, value ) ;
		_upper = std::max( _upper, value ) ;
	}
}

///
///
///
bool  Interval::intersects( const Interval & other ) const
{
	//empty intervals never intersects
	if ( isEmpty() || other.isEmpty() )
		return false ;

	return ! ( _lower > other._upper || _upper < other._lower ) ;
}

///
///
///
bool Interval::operator == ( const Interval& other ) const
{
	if ( isEmpty() && other.isEmpty() )
		return true ;

	return _lower == other._lower && _upper == other._upper ;
}

///
///
///
bool Interval::operator != ( const Interval& other ) const
{
	return ! ( (*this) == other );
}



}//detail
}//SFCGAL


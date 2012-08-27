#include <SFCGAL/Envelope.h>

namespace SFCGAL {

///
///
///
Envelope::Envelope()
{
	for ( size_t i = 0; i < 3; i++ ){
		_bounds[i] = detail::Interval();
	}
}

///
///
///
Envelope::Envelope(
	const double & xmin, const double & xmax,
	const double & ymin, const double & ymax
)
{
	_bounds[0] = detail::Interval(xmin,xmax);
	_bounds[1] = detail::Interval(ymin,ymax);
	_bounds[2] = detail::Interval();
}

///
///
///
Envelope::Envelope(
	const double & xmin, const double & xmax,
	const double & ymin, const double & ymax,
	const double & zmin, const double & zmax
)
{
	_bounds[0] = detail::Interval(xmin,xmax);
	_bounds[1] = detail::Interval(ymin,ymax);
	_bounds[2] = detail::Interval(zmin,zmax);
}

///
///
///
Envelope::Envelope( const Coordinate & p )
{
	expandToInclude(p);
}

///
///
///
Envelope::Envelope( const Coordinate & p1, const Coordinate & p2 )
{
	expandToInclude(p1);
	expandToInclude(p2);
}

///
///
///
Envelope::Envelope( const Envelope& other )
{
	for ( size_t i = 0; i < 3; i++ ){
		_bounds[i] = other._bounds[i] ;
	}
}

///
///
///
Envelope& Envelope::operator = ( const Envelope& other )
{
	for ( size_t i = 0; i < 3; i++ ){
		_bounds[i] = other._bounds[i] ;
	}
	return *this ;
}

///
///
///
Envelope::~Envelope()
{

}


///
///
///
bool Envelope::isEmpty() const
{
	return _bounds[0].isEmpty() || _bounds[1].isEmpty() ;
}

///
///
///
bool Envelope::is3D() const
{
	return ! isEmpty() && ! _bounds[2].isEmpty() ;
}

///
///
///
void Envelope::expandToInclude( const Coordinate & coordinate )
{
	_bounds[0].expandToInclude( coordinate.x() );
	_bounds[1].expandToInclude( coordinate.y() );
	_bounds[2].expandToInclude( coordinate.z() );
}

///
///
///
bool Envelope::contains( const Envelope& a, const Envelope& b )
{
	if ( a.is3D() ) {
		return b.xMin() <= a.xMin() && 
			b.xMax() >= a.xMax() &&
			b.yMin() <= a.yMin() &&
			b.yMax() >= a.yMax() &&
			b.zMin() <= a.zMin() &&
			b.zMax() >= a.zMax();
	}
	return b.xMin() <= a.xMin() && 
		b.xMax() >= a.xMax() &&
		b.yMin() <= a.yMin() &&
		b.yMax() >= a.yMax();
}

///
///
///
std::ostream& Envelope::print( std::ostream& ostr ) const
{
	ostr << "[ " << xMin();
	ostr << ", " << xMax();
	ostr << ", " << yMin();
	ostr << ", " << yMax();
	if ( is3D() ) {
		ostr << ", " << zMin() << ", " << zMax();
	}
	ostr << " ]";
	return ostr;
}

bool operator==( const Envelope& a, const Envelope& b)
{
	if ( a.is3D()) {
		return a.xMin() == b.xMin() &&
			a.yMin() == b.yMin() &&
			a.zMin() == b.zMin() &&
			a.xMax() == b.xMax() &&
			a.yMax() == b.yMax() &&
			a.zMax() == b.zMax();
	}
	return a.xMin() == b.xMin() &&
		a.yMin() == b.yMin() &&
		a.xMax() == b.xMax() &&
		a.yMax() == b.yMax();
}
}//SFCGAL


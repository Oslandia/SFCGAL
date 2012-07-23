#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
Solid::Solid():
	_shells(1,PolyhedralSurface())
{

}

///
///
///
Solid::Solid( const PolyhedralSurface & exteriorShell ):
	_shells(1,exteriorShell)
{

}

///
///
///
Solid::Solid( const std::vector< PolyhedralSurface > & shells ):
	_shells(shells)
{
	if ( _shells.empty() ){
		_shells.resize( 1, PolyhedralSurface() );
	}
}

///
///
///
Solid::Solid( Solid const& other ):
	_shells(other._shells)
{

}

///
///
///
Solid& Solid::operator = ( const Solid & other )
{
	_shells = other._shells ;
	return *this ;
}

///
///
///
Solid::~Solid()
{

}

///
///
///
Solid *  Solid::clone() const
{
	return new Solid( *this );
}




///
///
///
std::string  Solid::geometryType() const
{
	return "Solid" ;
}

///
///
///
GeometryType  Solid::geometryTypeId() const
{
	return TYPE_SOLID ;
}

///
///
///
int  Solid::dimension() const
{
	return 3 ;
}

///
///
///
int  Solid::coordinateDimension() const
{
	return exteriorShell().coordinateDimension() ;
}

///
///
///
bool  Solid::isEmpty() const
{
	return exteriorShell().isEmpty();
}

///
///
///
bool  Solid::is3D() const
{
	return exteriorShell().is3D();
}

///
///
///
void Solid::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void Solid::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL




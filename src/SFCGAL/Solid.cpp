#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
Solid::Solid()
{
	_shells.push_back( new PolyhedralSurface()  );
}

///
///
///
Solid::Solid( const PolyhedralSurface & exteriorShell )
{
	_shells.push_back( exteriorShell.clone() );
}

///
///
///
Solid::Solid( const std::vector< PolyhedralSurface > & shells )
{
	if ( shells.empty() ){
		_shells.resize(1, new PolyhedralSurface() );
	}else{
		for ( size_t i = 0; i < shells.size(); i++ ){
			_shells.push_back( shells[i].clone() ) ;
		}
	}
}

///
///
///
Solid::Solid( Solid const& other )
{
	(*this) = other ;
}

///
///
///
Solid& Solid::operator = ( const Solid & other )
{
	_shells.clear() ;
	for ( size_t i = 0; i < other.numShells(); i++ ){
		_shells.push_back( other.shellN(i).clone() );
	}
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




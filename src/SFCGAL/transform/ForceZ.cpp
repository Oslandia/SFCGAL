#include <SFCGAL/transform/ForceZ.h>
#include <SFCGAL/Point.h>


namespace SFCGAL {
namespace transform {

///
///
///
ForceZ::ForceZ( const double & defaultZ ):
	_defaultZ(defaultZ)
{

}

///
///
///
void ForceZ::transform( Point & p )
{
	if ( ! p.is3D() ){
		p = Point( p.x(), p.y(), _defaultZ );
	}
}


}//transform
}//SFCGAL




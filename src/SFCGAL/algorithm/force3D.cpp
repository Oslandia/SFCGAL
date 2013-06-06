#include <SFCGAL/algorithm/force3D.h>
#include <SFCGAL/detail/transform/ForceZ.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void force3D( Geometry& g, const Kernel::FT & defaultZ )
{
	transform::ForceZ t( defaultZ ) ;
	g.accept( t ) ;
}

} // namespace algorithm
} // namespace SFCGAL


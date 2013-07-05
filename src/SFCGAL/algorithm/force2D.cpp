#include <SFCGAL/algorithm/force2D.h>
#include <SFCGAL/detail/transform/Force2D.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void force2D( Geometry& g )
{
    transform::Force2D t ;
    g.accept( t ) ;
}

} // namespace algorithm
} // namespace SFCGAL


#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/GeometryCollection.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;

namespace SFCGAL {
namespace algorithm
{
    std::auto_ptr<Geometry> intersection3D_( const PolyhedralSurface& polya, const PolyhedralSurface& polyb )
    {
	return std::auto_ptr<Geometry>(new GeometryCollection());
    }

    std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb )
    {
	if ( ga.geometryTypeId() == TYPE_POLYHEDRALSURFACE &&
	     gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE ) {
	}
	// empty geometry
	return std::auto_ptr<Geometry>(new GeometryCollection());
    }
}
}

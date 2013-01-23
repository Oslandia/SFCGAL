#include <SFCGAL/algorithm/translate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/transform/AffineTransform3.h>


namespace SFCGAL {
namespace algorithm {

///
///
///
void   translate( Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	BOOST_ASSERT( ! g.isEmpty() );
	BOOST_ASSERT( g.is3D() );

	transform::AffineTransform3< Kernel > visitor(
		CGAL::Aff_transformation_3< Kernel >( CGAL::TRANSLATION, Kernel::Vector_3(dx,dy,dz) )
	);
	g.accept( visitor ) ;
}

} // namespace algorithm
} // namespace SFCGAL



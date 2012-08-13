#ifndef _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_
#define _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_

#include <CGAL/Aff_transformation_3.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {
namespace transform {

	/**
	 * Wrapper for CGAL::Aff_transform_3
	 */
	template < typename Kernel >
	class AffineTransform3 : public Transform {
	public:
		/**
		 * Constructor with a transform
		 */
		AffineTransform3( CGAL::Aff_transformation_3< Kernel > transform ):
			_transform(transform)
		{

		}

		/*
		 * [SFCGAL::Transform]
		 */
		virtual void transform( Point & p )
		{
			CGAL::Point_3< Kernel > transformed = p.toPoint_3< Kernel >().transform( _transform );
			p.x() = CGAL::to_double( transformed.x() );
			p.y() = CGAL::to_double( transformed.y() );
			p.z() = CGAL::to_double( transformed.z() );
		}

		virtual void transform( LineString & ls )
		{
			for ( size_t i = 0; i < ls.numPoints(); ++i ) {
				transform( ls.pointN(i) );
			}
		}
		virtual void transform( Triangle & tri )
		{
			transform(tri.vertex(0));
			transform(tri.vertex(1));
			transform(tri.vertex(2));
		}
		virtual void transform( Polygon & poly )
		{
			transform( poly.exteriorRing() );
			for ( size_t i = 0; i < poly.numInteriorRings(); ++i ) {
				transform( poly.interiorRingN(i) );
			}
		}

		virtual void transform( PolyhedralSurface & surf )
		{
			for ( size_t i = 0; i < surf.numPolygons(); ++i ) {
				transform( surf.polygonN(i) );
			}
		}

		virtual void transform( TriangulatedSurface & surf )
		{
			for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
				transform( surf.triangleN(i) );
			}
		}

		virtual void transform( Solid & solid )
		{
			transform( solid.exteriorShell() );
			for ( size_t i = 0; i < solid.numInteriorShells(); ++i ) {
				transform( solid.interiorShellN(i) );
			}
		}

	private:
		CGAL::Aff_transformation_3< Kernel > _transform ;
	};


}//transform
}//SFCGAL




#endif


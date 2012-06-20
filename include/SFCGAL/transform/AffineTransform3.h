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
			p.x() = transformed.x() ;
			p.y() = transformed.y() ;
			p.z() = transformed.z() ;
		}

	private:
		CGAL::Aff_transformation_3< Kernel > _transform ;
	};


}//transform
}//SFCGAL




#endif


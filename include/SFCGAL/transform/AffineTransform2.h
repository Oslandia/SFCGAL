#ifndef _SFCGAL_TRANSFORM_AFFINETRANSFORM2_H_
#define _SFCGAL_TRANSFORM_AFFINETRANSFORM2_H_

#include <CGAL/Aff_transformation_2.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {
namespace transform {

	/**
	 * Wrapper for CGAL::Aff_transform_2
	 */
	template < typename Kernel >
	class AffineTransform2 : public Transform {
	public:
		/**
		 * Constructor with a transform
		 */
		AffineTransform2( CGAL::Aff_transformation_2< Kernel > transform ):
			_transform(transform)
		{

		}

		/*
		 * [SFCGAL::Transform]
		 */
		virtual void transform( Point & p )
		{
			p = Point( p.toPoint_2().transform( _transform ) );
		}

	private:
		CGAL::Aff_transformation_2< Kernel > _transform ;
	};


}//transform
}//SFCGAL




#endif


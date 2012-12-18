#ifndef _SFCGAL_TRANSFORM_FORCEZ_ORDER_POINTS_H_
#define _SFCGAL_TRANSFORM_FORCEZ_ORDER_POINTS_H_

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {

namespace transform {

	/**
	 * Force Z. If the 2D surface is pointing down, reverse its points
	 */
	class ForceZOrderPoints : public Transform {
	public:
		/**
		 * Constructor with a default Z value
		 */
		ForceZOrderPoints( const Kernel::FT & defaultZ = 0 ) ;

		/*
		 * [SFCGAL::Transform]
		 */
		virtual void transform( Point & p ) ;

		virtual void visit( Triangle& );
		virtual void visit( Polygon& );

	private:
		Kernel::FT _defaultZ ;
	};


}//transform
}//SFCGAL




#endif


#ifndef _SFCGAL_TRANSFORM_FORCEZ_H_
#define _SFCGAL_TRANSFORM_FORCEZ_H_

#include <SFCGAL/Transform.h>

namespace SFCGAL {
namespace transform {

	/**
	 * Force Z definitions
	 */
	class ForceZ : public Transform {
	public:
		/**
		 * Constructor with a default Z value
		 */
		ForceZ( const double & defaultZ = 0.0 ) ;

		/*
		 * [SFCGAL::Transform]
		 */
		virtual void transform( Point & p ) ;

	private:
		double _defaultZ ;
	};


}//transform
}//SFCGAL




#endif


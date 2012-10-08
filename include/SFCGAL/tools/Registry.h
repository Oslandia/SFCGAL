#ifndef _SFCGAL_REGISTRY_H_
#define _SFCGAL_REGISTRY_H_

#include <string>
#include <map>
#include <vector>

namespace SFCGAL {
	class Geometry ;
}

namespace SFCGAL {
namespace tools {

	/**
	 * Registry for dynamic information about SFCGAL library
	 */
	class Registry {
	public:
		typedef std::vector< Geometry* >::iterator       prototype_iterator ;
		typedef std::vector< Geometry* >::const_iterator const_prototype_iterator ;

		/**
		 * destructor
		 */
		~Registry();

		/**
		 * Register a new Geometry type
		 */
		void addPrototype( const Geometry & g ) ;

		/**
		 * returns the list of the geometry types
		 */
		std::vector< std::string > getGeometryTypes() const ;

		/**
		 * returns the instance of the registry
		 */
		static Registry & instance() ;
	private:
		/**
		 * static instance of the Singleton
		 */
		static Registry * _instance ;
		/**
		 * prototypes of the geometries
		 */
		std::vector< Geometry* > _prototypes ;

		/**
		 * init registry
		 */
		Registry() ;
	};

}//namespace tools
}//namespace SFCGAL

#endif

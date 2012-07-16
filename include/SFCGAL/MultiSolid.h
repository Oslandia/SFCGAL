#ifndef _SFCGAL_MULTISOLID_H_
#define _SFCGAL_MULTISOLID_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {

	/**
	 * A MultiSolid
	 */
	class MultiSolid : public GeometryCollection {
	public:
		/**
		 * Empty MultiSolid constructor
		 */
		MultiSolid() ;
		/**
		 * Copy constructor
		 */
		MultiSolid( MultiSolid const& other ) ;
		/**
		 * assign operator
		 */
		MultiSolid& operator = ( const MultiSolid & other ) ;
		/**
		 * destructor
		 */
		virtual ~MultiSolid() ;

		//-- SFCGAL::Geometry
		virtual MultiSolid *   clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;

		/**
		 * returns the n-th Geometry as a Solid
		 */
		inline Solid &         solidN( const size_t & n )
		{
			return geometryN(n).as< Solid >() ;
		}
		/**
		 * returns the n-th Geometry as a Solid
		 */
		inline const Solid &   solidN( const size_t & n ) const
		{
			return geometryN(n).as< Solid >() ;
		}


		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

	protected:
		//-- SFCGAL::GeometryCollection
		virtual bool           isAllowed( Geometry const& g ) ;
	};


}

#endif

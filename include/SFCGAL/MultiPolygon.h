#ifndef _SFCGAL_MULTIPOLYGON_H_
#define _SFCGAL_MULTIPOLYGON_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/Polygon.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {

	/**
	 * A MultiPolygon in SFA.
	 *
	 * @ŧodo add polygon() etc.
	 */
	class MultiPolygon : public GeometryCollection {
	public:
		/**
		 * Empty MultiPolygon constructor
		 */
		MultiPolygon() ;
		/**
		 * Copy constructor
		 */
		MultiPolygon( MultiPolygon const& other ) ;
		/**
		 * assign operator
		 */
		MultiPolygon& operator = ( const MultiPolygon & other ) ;
		/**
		 * destructor
		 */
		virtual ~MultiPolygon() ;

		//-- SFCGAL::Geometry
		virtual MultiPolygon *   clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;

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

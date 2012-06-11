#ifndef _SFCGAL_MULTIPOINT_H_
#define _SFCGAL_MULTIPOINT_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {

	/**
	 * A MultiPoint in SFA.
	 *
	 * @ŧodo add pointN() etc.
	 */
	class MultiPoint : public GeometryCollection {
	public:
		/**
		 * Empty MultiPoint constructor
		 */
		MultiPoint() ;
		/**
		 * Copy constructor
		 */
		MultiPoint( MultiPoint const& other ) ;
		/**
		 * assign operator
		 */
		MultiPoint& operator = ( const MultiPoint & other ) ;
		/**
		 * destructor
		 */
		virtual ~MultiPoint() ;

		//-- SFCGAL::Geometry
		virtual MultiPoint *   clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;

	protected:
		//-- SFCGAL::GeometryCollection
		virtual bool           isAllowed( Geometry const& g ) ;
	};


}

#endif

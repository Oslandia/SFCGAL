#ifndef _SFCGAL_MULTILINESTRING_H_
#define _SFCGAL_MULTILINESTRING_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/LineString.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {

	/**
	 * A MultiLineString in SFA.
	 *
	 * @ŧodo add lineString() etc.
	 */
	class MultiLineString : public GeometryCollection {
	public:
		/**
		 * Empty MultiLineString constructor
		 */
		MultiLineString() ;
		/**
		 * Copy constructor
		 */
		MultiLineString( MultiLineString const& other ) ;
		/**
		 * assign operator
		 */
		MultiLineString& operator = ( const MultiLineString & other ) ;
		/**
		 * destructor
		 */
		virtual ~MultiLineString() ;

		//-- SFCGAL::Geometry
		virtual MultiLineString *   clone() const ;

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

#ifndef _SFCGAL_MULTIPOLYGON_H_
#define _SFCGAL_MULTIPOLYGON_H_

#include <vector>
#include <boost/assert.hpp>

#include <boost/serialization/base_object.hpp>

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

		/**
		 * returns the n-th Geometry as a Polygon
		 */
		inline Polygon &       polygonN( const size_t & n )
		{
			return geometryN(n).as< Polygon >() ;
		}
		/**
		 * returns the n-th Geometry as a Polygon
		 */
		inline const Polygon & polygonN( const size_t & n ) const
		{
			return geometryN(n).as< Polygon >() ;
		}


		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

		/**
		 * Serializer
		 */
		template <class Archive>
		void serialize( Archive& ar, const unsigned int version )
		{
			ar & boost::serialization::base_object<GeometryCollection>(*this);

		}
	protected:
		//-- SFCGAL::GeometryCollection
		virtual bool           isAllowed( Geometry const& g ) ;
	};


}

#endif

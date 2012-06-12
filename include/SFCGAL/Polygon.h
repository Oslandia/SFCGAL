#ifndef _SFCGAL_POLYGON_H_
#define _SFCGAL_POLYGON_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/LineString.h>
#include <SFCGAL/Surface.h>


namespace SFCGAL {

	/**
	 * A Polygon in SFA with holes
	 */
	class Polygon : public Surface {
	public:
		/**
		 * Empty Polygon constructor
		 */
		Polygon() ;
		/**
		 * Constructor with an exterior ring
		 */
		Polygon( const std::vector< LineString > & rings ) ;
		/**
		 * Constructor with an exterior ring
		 */
		Polygon( const LineString & exteriorRing ) ;

		/**
		 * Copy constructor
		 */
		Polygon( Polygon const& other ) ;

		/**
		 * assign operator
		 */
		Polygon& operator = ( const Polygon & other ) ;

		/**
		 * destructor
		 */
		~Polygon() ;

		//-- SFCGAL::Geometry
		virtual Polygon *   clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;
		//-- SFCGAL::Geometry
		virtual int            coordinateDimension() const ;
		//-- SFCGAL::Geometry
		virtual bool           isEmpty() const ;
		//-- SFCGAL::Geometry
		virtual bool           is3D() const ;


		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline const LineString &    exteriorRing() const {
			return _rings[0];
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline LineString &          exteriorRing() {
			return _rings[0];
		}

		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline size_t                numInteriorRings() const {
			return _rings.size() - 1 ;
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline const LineString &    interiorRingN( const size_t & n ) const {
			return _rings[n+1];
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline LineString &          interiorRingN( const size_t & n ) {
			return _rings[n+1];
		}

		/**
		 * Returns the number of rings
		 */
		inline size_t  numRings() const {
			return _rings.size() ;
		}
		/**
		 * Returns the n-th ring, 0 is exteriorRing
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline const LineString &    ringN( const size_t & n ) const {
			return _rings[n];
		}
		/**
		 * Returns the n-th ring, 0 is exteriorRing
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline LineString &          ringN( const size_t & n ) {
			return _rings[n];
		}

		/**
		 * Rings (both interior and exterior) accessors
		 * @todo expose only the exterior subset, through iterators ?
		 */
		const std::vector< LineString > & rings() const { return _rings; }
		std::vector< LineString > &       rings() { return _rings; }

		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

	private:
		/**
		 * rings forming the polygon (size() >= 1)
		 *
		 * _ring[0] is the interior ring
		 *
		 * @warning never empty, empty LineString as exteriorRing for empty Polygon
		 */
		std::vector< LineString > _rings ;
	};


}

#endif

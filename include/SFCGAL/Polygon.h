#ifndef _SFCGAL_POLYGON_H_
#define _SFCGAL_POLYGON_H_

#include <vector>
#include <boost/assert.hpp>

#include <CGAL/Polygon_with_holes_2.h>

#include <SFCGAL/LineString.h>
#include <SFCGAL/Surface.h>


namespace SFCGAL {

	/**
	 * A Polygon in SFA with holes
	 */
	class Polygon : public Surface {
	public:
		typedef std::vector< LineStringPtr >::iterator       iterator ;
		typedef std::vector< LineStringPtr >::const_iterator const_iterator ;

		/**
		 * Empty Polygon constructor
		 */
		Polygon() ;
		/**
		 * Constructor with an exterior ring
		 * @todo replace by Polygon( const std::vector< LineStringPtr > & rings
		 */
		Polygon( const std::vector< LineString > & rings ) ;
		/**
		 * Constructor with an exterior ring
		 */
		Polygon( const LineString & exteriorRing ) ;
		/**
		 * Constructor with a Triangle
		 */
		Polygon( const Triangle & triangle ) ;
		/**
		 * Copy constructor
		 */
		Polygon( Polygon const& other ) ;

		/**
		 * Constructor from CGAL::Polygon_with_holes_2<K>
		 */
		template < typename K >
		Polygon( const CGAL::Polygon_with_holes_2<K>& poly )
		{
			_rings.push_back( LineStringPtr( new LineString() ) );
			CGAL::Polygon_2<K> outer = poly.outer_boundary();
			typename CGAL::Polygon_2<K>::Edge_const_iterator ei;
			for ( ei = outer.edges_begin(); ei != outer.edges_end(); ++ei ) {
				_rings.back()->addPoint( ei->source() );
			}
			for ( typename CGAL::Polygon_with_holes_2<K>::Hole_const_iterator hit = poly.holes_begin(); hit != poly.holes_end(); ++hit) {
				_rings.push_back(LineStringPtr(new LineString()));
				typename CGAL::Polygon_2<K>::Edge_const_iterator ei;
				for ( ei = hit->edges_begin(); ei != hit->edges_end(); ++ei ) {
					_rings.back()->addPoint( ei->source() );
				}
			}
		}

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
		 * reverse Polygon orientation
		 */
		void reverse() ;


		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline const LineString &    exteriorRing() const {
			return *_rings[0];
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline LineString &          exteriorRing() {
			return *_rings[0];
		}


		/**
		 * Test if the polygon has interior rings
		 */
		inline bool                  hasInteriorRings() const {
			return _rings.size() == 1 ;
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
			return *_rings[n+1];
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline LineString &          interiorRingN( const size_t & n ) {
			return *_rings[n+1];
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
			BOOST_ASSERT( n < _rings.size() );
			return *_rings[n];
		}
		/**
		 * Returns the n-th ring, 0 is exteriorRing
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline LineString &          ringN( const size_t & n ) {
			BOOST_ASSERT( n < _rings.size() );
			return *_rings[n];
		}

		/**
		 * Rings (both interior and exterior) accessors
		 * @todo expose only the exterior subset, through iterators ?
		 * @done could lead to memory leaks since std::vector< LineString > has been changed to std::vector< LineString* >
		const std::vector< LineString > & rings() const {
			return _rings;
		}
		std::vector< LineString > &       rings() {
			return _rings;
		}
		*/

		inline iterator       begin() { return _rings.begin() ; }
		inline const_iterator begin() const { return _rings.begin() ; }

		inline iterator       end() { return _rings.end() ; }
		inline const_iterator end() const { return _rings.end() ; }


		/*
		 * Convert to CGAL::Polygon_2. Does not consider holes, if any
		 */
		template < typename K >
		CGAL::Polygon_2<K> toPolygon_2() const
		{
			return exteriorRing().toPolygon_2<K>();
		}

		/*
		 * Convert to CGAL::Polygon_with_holes_2.
		 */
		template < typename K >
		CGAL::Polygon_with_holes_2<K> toPolygon_with_holes_2() const
		{
			std::list<CGAL::Polygon_2<K> > holes;
			for ( size_t i = 0; i < numInteriorRings(); ++i ) {
				holes.push_back( interiorRingN(i).toPolygon_2<K>() );
			}
			return CGAL::Polygon_with_holes_2<K>( exteriorRing().toPolygon_2<K>(),
							      holes.begin(),
							      holes.end());
		}

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
		std::vector< LineStringPtr > _rings ;
	};


}

#endif

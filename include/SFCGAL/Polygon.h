#ifndef _SFCGAL_POLYGON_H_
#define _SFCGAL_POLYGON_H_

#include <vector>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

#include <CGAL/Polygon_with_holes_2.h>

#include <SFCGAL/LineString.h>
#include <SFCGAL/Surface.h>
#include <SFCGAL/Kernel.h>

namespace SFCGAL {

	/**
	 * A Polygon in SFA with holes
	 */
	class Polygon : public Surface {
	public:
		typedef boost::ptr_vector< LineString >::iterator       iterator ;
		typedef boost::ptr_vector< LineString >::const_iterator const_iterator ;

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
		 * Constructor with an exterior ring (takes ownership)
		 */
		Polygon( LineString * exteriorRing ) ;
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
		Polygon( const CGAL::Polygon_with_holes_2< Kernel >& poly );

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
		 * Check whether the 2D polygon is pointing up
		 */
		bool isCounterClockWiseOriented() const;

		/**
		 * reverse Polygon orientation
		 */
		void reverse() ;


		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline const LineString &    exteriorRing() const {
			return _rings.front();
		}
		/**
		 * [OGC/SFA]returns the exterior ring
		 */
		inline LineString &          exteriorRing() {
			return _rings.front();
		}
		/**
		 * Sets the exterior ring
		 */
		inline void  setExteriorRing( const LineString& ring ){
			_rings.front() = ring ;
		}
		/**
		 * Sets the exterior ring (takes ownership)
		 */
		inline void  setExteriorRing( LineString* ring ){
			_rings.replace( 0, ring );
		}

		/**
		 * Test if the polygon has interior rings
		 */
		inline bool                  hasInteriorRings() const {
			return _rings.size() > 1 ;
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
			BOOST_ASSERT( n < _rings.size() );
			return _rings[n];
		}
		/**
		 * Returns the n-th ring, 0 is exteriorRing
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline LineString &          ringN( const size_t & n ) {
			BOOST_ASSERT( n < _rings.size() );
			return _rings[n];
		}

		/**
		 * append a ring to the Polygon
		 */
		inline void            addInteriorRing( const LineString & ls ) {
			_rings.push_back( ls.clone() ) ;
		}
		/**
		 * append a ring to the Polygon (take ownership)
		 */
		inline void            addInteriorRing( LineString* ls ) {
			BOOST_ASSERT( ls != NULL );
			_rings.push_back( ls ) ;
		}

		/**
		 * append a ring to the Polygon
		 * @deprecated addInteriorRing
		 */
		inline void            addRing( const LineString & ls ) {
			_rings.push_back( ls.clone() ) ;
		}
		/**
		 * append a ring to the Polygon (take ownership)
		 * @deprecated addInteriorRing
		 */
		inline void            addRing( LineString* ls ) {
			BOOST_ASSERT( ls != NULL );
			_rings.push_back( ls ) ;
		}

		inline iterator       begin() { return _rings.begin() ; }
		inline const_iterator begin() const { return _rings.begin() ; }

		inline iterator       end() { return _rings.end() ; }
		inline const_iterator end() const { return _rings.end() ; }


		/*
		 * Convert to CGAL::Polygon_2. Does not consider holes, if any
		 */
		CGAL::Polygon_2<Kernel> toPolygon_2() const;

		/*
		 * Convert to CGAL::Polygon_with_holes_2.
		 */
		CGAL::Polygon_with_holes_2<Kernel> toPolygon_with_holes_2() const;

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
			ar & boost::serialization::base_object<Geometry>(*this);
			ar & _rings;
		}
	private:
		/**
		 * rings forming the polygon (size() >= 1)
		 *
		 * _ring[0] is the interior ring
		 *
		 * @warning never empty, empty LineString as exteriorRing for empty Polygon
		 */
		boost::ptr_vector< LineString > _rings ;
	};


}

#endif

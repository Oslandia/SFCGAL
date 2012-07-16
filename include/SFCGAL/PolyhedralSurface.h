#ifndef _SFCGAL_POLYHEDRALSURFACE_H_
#define _SFCGAL_POLYHEDRALSURFACE_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <CGAL/Polyhedron_3.h>

namespace SFCGAL {

	/**
	 * A PolyhedralSurface in SFA modeled as a Polygon soup
	 *
	 * @todo template < size_t N >?
	 * @todo do better than a "polygon soup" or add topological view?
	 */
	class PolyhedralSurface : public Surface {
	public:
		/**
		 * Empty PolyhedralSurface constructor
		 */
		PolyhedralSurface() ;
		/**
		 * Constructor with a vector of polygons
		 */
		PolyhedralSurface( const std::vector< Polygon > & polygons ) ;
		/**
		 * Copy constructor
		 */
		PolyhedralSurface( PolyhedralSurface const& other ) ;
		/**
		 * assign operator
		 */
		PolyhedralSurface& operator = ( const PolyhedralSurface & other ) ;
		/**
		 * destructor
		 */
		~PolyhedralSurface() ;

		//-- SFCGAL::Geometry
		virtual PolyhedralSurface * clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;
		//-- SFCGAL::Geometry
		virtual int            dimension() const ;
		//-- SFCGAL::Geometry
		virtual int            coordinateDimension() const ;
		//-- SFCGAL::Geometry
		virtual bool           isEmpty() const ;
		//-- SFCGAL::Geometry
		virtual bool           is3D() const ;


		/**
		 * Convert PolyhedralSurface to TriangulatedSurface
		 */
		TriangulatedSurface       toTriangulatedSurface() const ;


		/**
		 * [SFA/OGC]Returns the number of points
		 */
		inline size_t             numPolygons() const { return _polygons.size(); }
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline const Polygon  &   polygonN( size_t const& n ) const {
			BOOST_ASSERT( n < _polygons.size() );
			return _polygons[n];
		}
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline Polygon &          polygonN( size_t const& n ) {
			BOOST_ASSERT( n < _polygons.size() );
			return _polygons[n];
		}
		/**
		 * add a polygon to the PolyhedralSurface
		 */
		void                      addPolygon( const Polygon & polygon ) ;

		/**
		 * Convert to CGAL::Polyhedron_3
		 */
		template < typename K >
		CGAL::Polyhedron_3<K> toPolyhedron_3() const
		{
			TriangulatedSurface tri;
			algorithm::triangulate( *this, tri );
			return tri.toPolyhedron_3<K>();
		}

		const std::vector< Polygon > & polygons() const { return _polygons; }
		std::vector< Polygon > &       polygons() { return _polygons; }

		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;
	private:
		std::vector< Polygon > _polygons ;
	};


}

#endif

/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_POLYHEDRALSURFACE_H_
#define _SFCGAL_POLYHEDRALSURFACE_H_

#include <vector>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>

#include <CGAL/Polyhedron_3.h>

namespace SFCGAL {

	/**
	 * A PolyhedralSurface in SFA modeled as a Polygon soup
	 *
	 * @todo template < size_t N >?
	 * @todo do better than a "polygon soup" or add topological view?
	 */
	class SFCGAL_API PolyhedralSurface : public Surface {
	public:
		typedef boost::ptr_vector< Polygon >::iterator       iterator ;
		typedef boost::ptr_vector< Polygon >::const_iterator const_iterator ;

		/**
		 * Empty PolyhedralSurface constructor
		 */
		PolyhedralSurface() ;
		/**
		 * Constructor with a vector of polygons
		 */
		PolyhedralSurface( const std::vector< Polygon > & polygons ) ;
		/**
		 * Constructor from a CGAL::Polyhedron_3
		 */
		PolyhedralSurface( const MarkedPolyhedron& poly );
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
		//-- SFCGAL::Geometry
		virtual bool           isMeasured() const ;

		/**
		 * Convert PolyhedralSurface to TriangulatedSurface
		 */
		TriangulatedSurface       toTriangulatedSurface() const ;


		/**
		 * [SFA/OGC]Returns the number of points
		 * @deprecated see numGeometries
		 */
		inline size_t             numPolygons() const { return _polygons.size(); }
		/**
		 * [SFA/OGC]Returns the n-th point
		 * @deprecated see geometryN()
		 */
		inline const Polygon  &   polygonN( size_t const& n ) const {
			BOOST_ASSERT( n < _polygons.size() );
			return _polygons[n];
		}
		/**
		 * [SFA/OGC]Returns the n-th point
		 * @deprecated see geometryN()
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
		 * add a polygon to the PolyhedralSurface
		 */
		void                      addPolygon( Polygon* polygon ) ;
		/**
		 * add polygons from an other PolyhedralSurface
		 */
		void                      addPolygons( const PolyhedralSurface & polyhedralSurface ) ;

		//-- SFCGAL::Geometry
		virtual size_t               numGeometries() const ;
		//-- SFCGAL::Geometry
		virtual const Polygon  &    geometryN( size_t const& n ) const ;
		//-- SFCGAL::Geometry
		virtual Polygon &            geometryN( size_t const& n ) ;

		/**
		 * Convert to CGAL::Polyhedron_3
		 */
		template < typename K, typename Polyhedron >
		std::auto_ptr<Polyhedron> toPolyhedron_3() const
		{
			TriangulatedSurface tri;
			triangulate::triangulatePolygon3D( *this, tri );
			return tri.toPolyhedron_3<K, Polyhedron>();
		}


		//-- iterators

		inline iterator       begin() {
			return _polygons.begin() ;
		}
		inline const_iterator begin() const {
			return _polygons.begin() ;
		}

		inline iterator       end() {
			return _polygons.end() ;
		}
		inline const_iterator end() const {
			return _polygons.end() ;
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
			ar & boost::serialization::base_object<Geometry>(*this);
			ar & _polygons;
		}
	private:
		boost::ptr_vector< Polygon > _polygons ;
	};
}

#endif

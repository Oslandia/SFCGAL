/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#ifndef _SFCGAL_TRIANGULATED_SURFACE_H_
#define _SFCGAL_TRIANGULATED_SURFACE_H_

#include <vector>
#include <set>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>


namespace SFCGAL {

	/**
	 * A TriangulatedSurface in SFA modeled as a Triangle soup
	 *
	 * @todo template < size_t N >?
	 * @todo do better than a "triangle soup" or add topological view?
	 *
	 * @warning CGAL::Triangulation_3 contains tetrahedron
	 */
	class TriangulatedSurface : public Surface {
	public:
		typedef boost::ptr_vector< Triangle >::iterator       iterator ;
		typedef boost::ptr_vector< Triangle >::const_iterator const_iterator ;

		/**
		 * Empty TriangulatedSurface constructor
		 */
		TriangulatedSurface() ;
		/**
		 * Constructor with a vector of triangles
		 */
		TriangulatedSurface( const std::vector< Triangle > & triangle ) ;
		/**
		 * Copy constructor
		 */
		TriangulatedSurface( TriangulatedSurface const& other ) ;
		/**
		 * assign operator
		 */
		TriangulatedSurface& operator = ( const TriangulatedSurface & other ) ;
		/**
		 * destructor
		 */
		~TriangulatedSurface() ;

		//-- SFCGAL::Geometry
		virtual TriangulatedSurface *   clone() const ;

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
		 * [SFA/OGC]Returns the number of points
		 * @deprecated see numGeometries()
		 */
		inline size_t             numTriangles() const { return _triangles.size(); }
		/**
		 * [SFA/OGC]Returns the n-th point
		 * @deprecated see geometryN()
		 */
		inline const Triangle  &  triangleN( size_t const& n ) const {
			BOOST_ASSERT( n < _triangles.size() );
			return _triangles[n];
		}
		/**
		 * [SFA/OGC]Returns the n-th point
		 * @deprecated see geometryN()
		 */
		inline Triangle &         triangleN( size_t const& n ) {
			BOOST_ASSERT( n < _triangles.size() );
			return _triangles[n];
		}
		/**
		* add a Triangle to the TriangulatedSurface
		*/
		inline void               addTriangle( const Triangle & triangle )
		{
			addTriangle( triangle.clone() );
		}
		/**
		* add a Triangle to the TriangulatedSurface
		*/
		inline void               addTriangle( Triangle * triangle )
		{
			_triangles.push_back( triangle );
		}
		/**
		 * add triangles from an other TriangulatedSurface
		 */
		void                      addTriangles( const TriangulatedSurface & other ) ;


		//-- SFCGAL::Geometry
		virtual size_t               numGeometries() const ;
		//-- SFCGAL::Geometry
		virtual const Triangle  &   geometryN( size_t const& n ) const ;
		//-- SFCGAL::Geometry
		virtual Triangle &           geometryN( size_t const& n ) ;


		//const std::vector< Triangle > & triangles() const { return _triangles; }
		//std::vector< Triangle > &       triangles() { return _triangles; }

		//-- iterators

		inline iterator       begin() {
			return _triangles.begin() ;
		}
		inline const_iterator begin() const {
			return _triangles.begin() ;
		}

		inline iterator       end() {
			return _triangles.end() ;
		}
		inline const_iterator end() const {
			return _triangles.end() ;
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
			ar & _triangles;
		}
	private:
		boost::ptr_vector< Triangle > _triangles ;

		// Private class
		// A modifier creating triangles from a TriangulatedSurface with the incremental builder.
		template <class HDS>
			class Triangulated2Polyhedron : public CGAL::Modifier_base<HDS> {
		public:
		Triangulated2Polyhedron( const TriangulatedSurface& surf ) : surf(surf) {}
			
			typedef typename HDS::Vertex                Vertex;
			typedef typename Vertex::Point              Point;
			typedef typename HDS::Traits                K;
			typedef std::map<Point, size_t>             PointMap;
			typedef std::set< std::pair<Point, Point> > HalfedgeSet;
			
			void operator()( HDS& hds)
			{
				// Postcondition: `hds' is a valid polyhedral surface.
				CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
				B.begin_surface( /* vertices */ surf.numGeometries() * 3, /* facets */ surf.numGeometries());
				
				size_t vertex_idx = 0;
				
				// first pass: insert vertices, only if they are not shared between faces
				// thanks to a binary tree (PointMap)
				for ( size_t i = 0; i < surf.numGeometries(); i++ ) {
					for ( size_t j = 0; j < 3; j++ ) {
						Point p = surf.geometryN(i).vertex(j).toPoint_3();
						size_t v;
						if ( points.find(p) == points.end() ) {
							B.add_vertex( p );
							v = vertex_idx++;
							points[p] = v;
						}
						else {
							v = points[p];
						}
					}
				}
				// second pass: adjacent triangles must be built with compliant orientations
				// the two halfedges of a shared edge must be of opposite orientation

				// Extract from CGAL's documentation
				// "The convention is that the halfedges are oriented counterclockwise
				// around facets as seen from the outside of the polyhedron"

				for ( size_t i = 0; i < surf.numGeometries(); i++ ) {
					B.begin_facet();
					CGAL::Triangle_3<K> tri( surf.geometryN(i).toTriangle_3());
					CGAL::Point_3<K> pa( tri[0] );
					CGAL::Point_3<K> pb( tri[1] );
					CGAL::Point_3<K> pc( tri[2] );
					
					if ( edges.find( std::make_pair(pa, pb)) != edges.end() ||
					     edges.find( std::make_pair(pb, pc)) != edges.end() ||
					     edges.find( std::make_pair(pc, pa)) != edges.end() ) {
						BOOST_THROW_EXCEPTION(Exception( "When trying to build a CGAL::Polyhedron_3 from a TriangulatedSurface: bad orientation for "
										 + surf.geometryN(i).asText()
										 + " consider using ConsistentOrientationBuilder first"));
					}
					B.add_vertex_to_facet( points[pa] );
					B.add_vertex_to_facet( points[pb] );
					B.add_vertex_to_facet( points[pc] );
					edges.insert( std::make_pair( pa, pb ) );
					edges.insert( std::make_pair( pb, pc ) );
					edges.insert( std::make_pair( pc, pa ) );
					B.end_facet();
				}
				B.end_surface();
			}
		private:
			const TriangulatedSurface& surf;
			PointMap points;
			HalfedgeSet edges;
		};

	public:
		///
		/// Converts a TriangulatedSurface to a CGAL::Polyhedron_3
		template < typename K, typename Polyhedron >
		std::auto_ptr<Polyhedron> toPolyhedron_3() const
		{
			Polyhedron *poly = new Polyhedron();
			Triangulated2Polyhedron<typename Polyhedron::HalfedgeDS> converter( *this );
			poly->delegate( converter);
			return std::auto_ptr<Polyhedron>( poly );
		}
	};
}

#endif

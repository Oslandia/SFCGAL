#ifndef _SFCGAL_TRIANGULATED_SURFACE_H_
#define _SFCGAL_TRIANGULATED_SURFACE_H_

#include <vector>
#include <set>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

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
		 */
		inline size_t             numTriangles() const { return _triangles.size(); }
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline const Triangle  &  triangleN( size_t const& n ) const {
			BOOST_ASSERT( n < _triangles.size() );
			return _triangles[n];
		}
		/**
		 * [SFA/OGC]Returns the n-th point
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
				B.begin_surface( /* vertices */ surf.numTriangles() * 3, /* facets */ surf.numTriangles());
				
				size_t vertex_idx = 0;
				
				// first pass: insert vertices, only if they are not shared between faces
				// thanks to a binary tree (PointMap)
				for ( size_t i = 0; i < surf.numTriangles(); i++ ) {
					for ( size_t j = 0; j < 3; j++ ) {
						Point p = surf.triangleN(i).vertex(j).template toPoint_3<K>();
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

				for ( size_t i = 0; i < surf.numTriangles(); i++ ) {
					B.begin_facet();
					CGAL::Triangle_3<K> tri( surf.triangleN(i).template toTriangle_3<K>());
					CGAL::Point_3<K> pa( tri[0] );
					CGAL::Point_3<K> pb( tri[1] );
					CGAL::Point_3<K> pc( tri[2] );
					
					if ( edges.find( std::make_pair(pa, pb)) != edges.end() ||
					     edges.find( std::make_pair(pb, pc)) != edges.end() ||
					     edges.find( std::make_pair(pc, pa)) != edges.end() ) {
						BOOST_THROW_EXCEPTION(Exception( "When trying to build a CGAL::Polyhedron_3 from a TriangulatedSurface: bad orientation for "
										 + surf.triangleN(i).asText()
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

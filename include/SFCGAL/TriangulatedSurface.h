#ifndef _SFCGAL_TRIANGULATED_SURFACE_H_
#define _SFCGAL_TRIANGULATED_SURFACE_H_

#include <vector>
#include <boost/assert.hpp>

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
	class TriangulatedSurface : public Geometry {
	public:
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
		inline const Triangle  &  triangleN( size_t const& n ) const { return _triangles[n]; }
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline Triangle &         triangleN( size_t const& n ) { return _triangles[n]; }
		/**
		* add a Triangle to the TriangulatedSurface
		*/
		inline void               addTriangle( const Triangle & triangle )
		{
			_triangles.push_back( triangle );
		}


		const std::vector< Triangle > & triangles() const { return _triangles; }
		std::vector< Triangle > &       triangles() { return _triangles; }
	private:
		std::vector< Triangle > _triangles ;
	};


}

#endif

#ifndef _SFCGAL_TRIANGLE_H_
#define _SFCGAL_TRIANGLE_H_

#include <boost/shared_ptr.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/Surface.h>
#include <SFCGAL/TypeForDimension.h>

#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>

namespace SFCGAL {

	/**
	 * [OGC/SFA]Triangle
	 *
	 * @warning According to SFA, a Triangle should be inherited from a Polygon. That means
	 * that a triangle "is a" Polygon with hole. This inheritance is removed in order to
	 * keep CGAL modeling.
	 *
	 * @warning An empty triangle has empty points
	 */
	class Triangle : public Surface {
	public:
		/**
		 * empty Triangle constructor
		 */
		Triangle();
		/**
		 * constructor with 3 points
		 */
		Triangle( const Point & p, const Point & q, const Point & r );
		/**
		 * copy constructor
		 */
		Triangle( const Triangle & other );
		/**
		 * assign operator
		 */
		Triangle& operator = ( const Triangle & other );
		/**
		 * destructor
		 */
		~Triangle();

		//-- SFCGAL::Geometry
		virtual Triangle *   clone() const ;

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
		 * reverse Triangle orientation
		 */
		void                   reverse() ;


		/**
		 * convert a triangle to a polygon
		 */
		Polygon               toPolygon() const ;

		/**
		 * returns the i-th vertex
		 */
		inline const Point & vertex( const int & i ) const
		{
			return _vertices[ i % 3 ];
		}
		/**
		 * returns the i-th vertex
		 */
		inline Point &       vertex( const int & i )
		{
			return _vertices[ i % 3 ];
		}

		/**
		 * Convert to CGAL::Triangle_2
		 */
		inline Kernel::Triangle_2 toTriangle_2() const
		{
			return Kernel::Triangle_2( vertex(0).toPoint_2(), vertex(1).toPoint_2(), vertex(2).toPoint_2() );
		}

 		/**
		 * Convert to CGAL::Triangle_3
		 */
		inline Kernel::Triangle_3 toTriangle_3() const
		{
			return Kernel::Triangle_3( vertex(0).toPoint_3(), vertex(1).toPoint_3(), vertex(2).toPoint_3() );
		}

 		/**
		 * Convert to CGAL::Triangle_2 or CGAL::Triangle_2
		 */
		template < int D >
		inline typename TypeForDimension<D>::Triangle toTriangle_d() const
		{
		    return typename TypeForDimension<D>::Triangle( vertex(0).toPoint_d<D>(), vertex(1).toPoint_d<D>(), vertex(2).toPoint_d<D>() );
		}

		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;
	private:
		/**
		 * point forming the triangle
		 */
		Point _vertices[3];
	};

}

#endif

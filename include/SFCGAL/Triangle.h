#ifndef _SFCGAL_TRIANGLE_H_
#define _SFCGAL_TRIANGLE_H_

#include <boost/shared_ptr.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/Surface.h>

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
		template < typename K >
		inline CGAL::Triangle_2<K> toTriangle_2() const
		{
			return CGAL::Triangle_2<K>( vertex(0).toPoint_2<K>(), vertex(1).toPoint_2<K>(), vertex(2).toPoint_2<K>() );
		}

		/**
		 * Convert to CGAL::Triangle_3
		 */
		template < typename K >
		inline CGAL::Triangle_3<K> toTriangle_3() const
		{
			return CGAL::Triangle_3<K>( vertex(0).toPoint_3<K>(), vertex(1).toPoint_3<K>(), vertex(2).toPoint_3<K>() );
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

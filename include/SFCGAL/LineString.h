#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <vector>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <SFCGAL/Point.h>

#include <CGAL/Polygon_2.h>

namespace SFCGAL {

	/**
	 * A LineString in SFA
	 *
	 * @todo template < size_t N >?
	 */
	class LineString : public Geometry {
	public:
		/**
		 * Empty LineString constructor
		 */
		LineString() ;
		/**
		 * Constructor with a point vector
		 */
		LineString( const std::vector< Point > & points ) ;
		/**
		 * Empty LineString constructor
		 */
		LineString( const Point & startPoint, const Point & endPoint ) ;
		/**
		 * Copy constructor
		 */
		LineString( LineString const& other ) ;

		/**
		 * assign operator
		 */
		LineString& operator = ( const LineString & other ) ;

		/**
		 * destructor
		 */
		~LineString() ;

		//-- SFCGAL::Geometry
		virtual LineString *   clone() const ;

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
		 * reverse LineString orientation
		 */
		void reverse() ;

		/**
		 * [SFA/OGC]Returns the number of points
		 */
		inline size_t          numPoints() const { return _points.size(); }
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline const Point  &  pointN( size_t const& n ) const { return _points[n]; }
		/**
		 * [SFA/OGC]Returns the n-th point
		 */
		inline Point &         pointN( size_t const& n ) { return _points[n]; }


		/**
		 * [SFA/OGC]Returns the first point
		 */
		inline const Point &   startPoint() const { return _points.front(); }
		/**
		 * [SFA/OGC]Returns the first point
		 */
		inline Point &         startPoint() { return _points.front(); }


		/**
		 * [SFA/OGC]Returns the first point
		 */
		inline const Point &   endPoint() const { return _points.back(); }
		/**
		 * [SFA/OGC]Returns the first point
		 */
		inline Point &         endPoint() { return _points.back(); }


		/**
		 * append a point to the LineString
		 */
		inline void            addPoint( const Point & p ) {
			_points.push_back( p ) ;
		}



		const std::vector< Point > & points() const { return _points; }
		std::vector< Point > &       points() { return _points; }


		// TODO: replace by boost::tranform_iterator ?
		/**
		 * Const iterator to 2D points
		 */
		template < typename K >
		class Point_2_const_iterator :
			public boost::iterator_facade<
			Point_2_const_iterator<K>,
			CGAL::Point_2<K> const,
			boost::bidirectional_traversal_tag >
		{
		public:
			Point_2_const_iterator() {}
			explicit Point_2_const_iterator( std::vector<Point>::const_iterator it ) : it_(it) {}
			Point_2_const_iterator( const Point_2_const_iterator<K>& other ) : it_(other.it_) {}
		private:
			friend class boost::iterator_core_access;
			void increment() { it_++; }
			void decrement() { it_--; }
			bool equal( const Point_2_const_iterator<K>& other ) const { return this->it_ == other.it_; }
			const CGAL::Point_2<K>& dereference() const { p_ = it_->toPoint_2<K>(); return p_; }
			mutable CGAL::Point_2<K> p_;
			std::vector<Point>::const_iterator it_;
		};
		template < typename K >
		Point_2_const_iterator<K> points_2_begin() const
		{
			return Point_2_const_iterator<K>(points().begin());
		}
		template < typename K >
		Point_2_const_iterator<K> points_2_end() const
		{
			return Point_2_const_iterator<K>(points().end());
		}
		template < typename K >
		std::pair< Point_2_const_iterator<K>, Point_2_const_iterator<K> > points_2() const
		{
			return std::make_pair( points_2_begin<K>(), points_2_end<K>() );
		}
		
		/**
		 * Const iterator to 3D points
		 */
		template < typename K >
		class Point_3_const_iterator :
			public boost::iterator_facade<
			Point_3_const_iterator<K>,
			CGAL::Point_3<K> const,
			boost::bidirectional_traversal_tag >
		{
		public:
			Point_3_const_iterator() {}
			explicit Point_3_const_iterator( std::vector<Point>::const_iterator it ) : it_(it) {}
			Point_3_const_iterator( const Point_3_const_iterator<K>& other ) : it_(other.it_) {}
		private:
			friend class boost::iterator_core_access;
			void increment() { it_++; }
			void decrement() { it_--; }
			bool equal( const Point_3_const_iterator<K>& other ) const { return this->it_ == other.it_; }
			const CGAL::Point_3<K>& dereference() const { p_ = it_->toPoint_3<K>(); return p_; }
			mutable CGAL::Point_3<K> p_;
			std::vector<Point>::const_iterator it_;
		};
		template < typename K >
		Point_3_const_iterator<K> points_3_begin() const
		{
			return Point_3_const_iterator<K>(points().begin());
		}
		template < typename K >
		Point_3_const_iterator<K> points_3_end() const
		{
			return Point_3_const_iterator<K>(points().end());
		}
		template < typename K >
		std::pair< Point_3_const_iterator<K>, Point_3_const_iterator<K> > points_3() const
		{
			return std::make_pair( points_3_begin<K>(), points_3_end<K>() );
		}

		/*
		 * Convert to CGAL::Polygon_2
		 */

		template < typename K >
		CGAL::Polygon_2<K> toPolygon_2() const
		{
			Point_2_const_iterator<K> pend = points_2_end<K>();
			// skip the last point
			pend--;
			return CGAL::Polygon_2<K>( points_2_begin<K>(), pend );
		}
		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;
	private:
		std::vector< Point > _points ;
	};


}

#endif

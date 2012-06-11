#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <vector>
#include <boost/assert.hpp>

#include <SFCGAL/Point.h>

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


		const std::vector< Point > & points() const { return _points; }
		std::vector< Point > &       points() { return _points; }
	private:
		std::vector< Point > _points ;
	};


}

#endif

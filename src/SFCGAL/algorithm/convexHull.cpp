#include <SFCGAL/algorithm/convexHull.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>

#include <SFCGAL/Exception.h>
#include <boost/format.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/convex_hull_3.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
//typedef CGAL::Exact_predicates_exact_constructions_kernel  Kernel;

typedef CGAL::Point_3< Kernel >                              Point_3;
typedef CGAL::Segment_3< Kernel >                            Segment_3;
typedef CGAL::Triangle_3< Kernel >                           Triangle_3;
typedef CGAL::Polyhedron_3< Kernel >                         Polyhedron_3;

typedef CGAL::Point_2< Kernel >                              Point_2;

namespace SFCGAL {
namespace algorithm {

///
///
///
Geometry* convexHull( const Geometry & g )
{
	using CGAL::object_cast ;


	SFCGAL::detail::GetPointsVisitor getPointVisitor;
	const_cast< Geometry & >(g).accept( getPointVisitor );

	// collect points

	std::vector< Point_2 > points ;
	for ( size_t i = 0; i < getPointVisitor.points.size(); i++ ){
		points.push_back( getPointVisitor.points[i]->toPoint_2< Kernel >() );
	}

	// resulting extreme points
	std::list<Point_2> epoints;
	CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(epoints) ) ;

	if ( epoints.size() == 1 ) {
	    return new Point( *epoints.begin() );
	}
	else if ( epoints.size() == 2 ) {
	    return new LineString( Point(*epoints.begin()), Point( *epoints.end()) );
	}
	else if ( epoints.size() == 3 ) {
	    std::list<Point_2>::const_iterator it = epoints.begin();
	    Point_2 p( *it++ );
	    Point_2 q( *it++ );
	    Point_2 r( *it++ );
	    return new Triangle(p, q, r);
	}
	else if ( epoints.size() > 3 ) {
	    Polygon* poly = new Polygon;
	    for ( std::list<Point_2>::const_iterator it = epoints.begin(); it != epoints.end(); ++it ) {
		poly->exteriorRing().addPoint( Point(*it) );
	    }
	    return poly;
	}
	else {
		BOOST_THROW_EXCEPTION( Exception("unexpected CGAL output type in CGAL::convex_hull_2") );
	}
}

///
///
///
Geometry* convexHull3D( const Geometry & g )
{
	using CGAL::object_cast ;


	SFCGAL::detail::GetPointsVisitor getPointVisitor;
	const_cast< Geometry & >(g).accept( getPointVisitor );

	// collect points

	std::vector< Point_3 > points ;
	for ( size_t i = 0; i < getPointVisitor.points.size(); i++ ){
		points.push_back( getPointVisitor.points[i]->toPoint_3< Kernel >() );
	}

	/*
	 * http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Convex_hull_3/Chapter_main.html
	 *
	 * handles all degenerate cases and returns a CGAL::Object,
	 * which may be a point, a segment, a triangle, or a polyhedron.
	 */
	CGAL::Object hull;
	CGAL::convex_hull_3( points.begin(), points.end(), hull ) ;

	if (const Point_3 * point = object_cast< Point_3 >(&hull)) {
		return new Point( *point );
	} else if (const Segment_3 * segment = object_cast< Segment_3 >(&hull)) {
		return new LineString( Point( segment->start() ), Point( segment->end() ) );
	} else if (const Triangle_3 * triangle = object_cast< Triangle_3 >(&hull)) {
		return new Triangle(
			Point( triangle->vertex(0) ),
			Point( triangle->vertex(1) ),
			Point( triangle->vertex(2) )
		);
	} else if (const Polyhedron_3 * polyhedron = object_cast< Polyhedron_3 >(&hull)) {
		std::auto_ptr< PolyhedralSurface > result( new PolyhedralSurface() );
		for ( Polyhedron_3::Facet_const_iterator it_facet = polyhedron->facets_begin();
				it_facet != polyhedron->facets_end(); ++it_facet)
		{
			Polyhedron_3::Halfedge_around_facet_const_circulator it = it_facet->facet_begin();

			std::vector< Point > ring ;
			do {
				ring.push_back( Point( it->vertex()->point() ) );
			} while ( ++it != it_facet->facet_begin());
			ring.push_back( ring.front() );

			result->addPolygon( Polygon( ring ) );
		}
		return result.release() ;
	}else{
		BOOST_THROW_EXCEPTION( Exception("unexpected CGAL output type in CGAL::convex_hull_3") );
	}
}


}//algorithm
}//SFCGAL

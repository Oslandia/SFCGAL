#include <SFCGAL/algorithm/convexHull.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>

#include <SFCGAL/Exception.h>
#include <boost/format.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef CGAL::Polyhedron_3<Kernel>                           Polyhedron_3;
typedef CGAL::Segment_3< Kernel >                            Segment_3;
typedef CGAL::Point_3< Kernel >                              Point_3;


namespace SFCGAL {
namespace algorithm {

///
///
///
Geometry* convexHull3D( const Geometry & g )
{
	detail::GetPointsVisitor getPointVisitor;
	const_cast< Geometry & >(g).accept( getPointVisitor );

	// collect points

	std::vector< Point_3 > points ;
	for ( size_t i = 0; i < getPointVisitor.points.size(); i++ ){
		points.push_back( getPointVisitor.points[i]->toPoint_3< Kernel >() );
	}

	if ( points.empty() ){
		return new GeometryCollection();
	}else if ( points.size() == 1 ){
		std::vector< Point_3 >::iterator it = points.begin() ;
		return new Point( it->x(), it->y(), it->z() );
	}else if ( points.size() == 2 ){
		std::vector< Point_3 >::iterator it = points.begin() ;
		Point a( it->x(), it->y(), it->z() ); ++it ;
		Point b( it->x(), it->y(), it->z() );
		return new LineString( a, b );
	}else if ( points.size() < 4 ){
		BOOST_THROW_EXCEPTION(Exception(
			(boost::format("Not enougth points to compute ConvexHull3D in %1%") % g.asText()).str()
		));
	}

	Polyhedron_3 polyhedron;
	CGAL::convex_hull_3( points.begin(), points.end(), polyhedron ) ;

	//std::cout << polyhedron.size_of_vertices() << std::endl ;
	std::auto_ptr< PolyhedralSurface > result( new PolyhedralSurface() );
	for ( Polyhedron_3::Facet_iterator it_facet = polyhedron.facets_begin();
			it_facet != polyhedron.facets_end(); ++it_facet)
	{
		Polyhedron_3::Halfedge_around_facet_circulator it = it_facet->facet_begin();

		std::vector< Point > ring ;
		do {
			ring.push_back( Point( it->vertex()->point() ) );
		} while ( ++it != it_facet->facet_begin());
		ring.push_back( ring.front() );

		result->addPolygon( Polygon( ring ) );
	}

	return result.release() ;
}


}//algorithm
}//SFCGAL


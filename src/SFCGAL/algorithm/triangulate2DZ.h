#ifndef _SFCGAL_ALGORITHM_TRIANGULATE2DZ_H_
#define _SFCGAL_ALGORITHM_TRIANGULATE2DZ_H_

#include <SFCGAL/log.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/detail/collectPointsAndConstraints.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>

namespace SFCGAL {
namespace algorithm {
    
    /**
     * 2DZ triangulation of geometries
     * @warning this implements forbids constraints intersections.
     */
    template < typename K >
    TriangulatedSurface<K> triangulate2DZ( const Geometry<K> & geometry ){
        BOOST_LOG_TRIVIAL(info) << "triangulate2DZ...";
        
        // Get vertex and constraints
        BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - collect points and constraints...";
        std::vector< Point<K> > points ;
        std::vector< std::pair< size_t, size_t > > constraints ;
        detail::collectPointsAndConstraints(geometry,points,constraints);
        BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - "<< points.size() << " point(s)," << constraints.size() << " containt(s) found";

        // Build triangulation
        // Note that it is more efficient to batch insert vertices
        typedef CGAL::Projection_traits_xy_3<K>                Gt;
        typedef CGAL::Constrained_Delaunay_triangulation_2<Gt> CDT ;
        typedef typename CDT::Finite_faces_iterator            Finite_faces_iterator ;
        CDT cdt ;
        cdt.insert_constraints(
            points.begin(), points.end(),
            constraints.begin(), constraints.end()
        );

        // Retrieve results
        BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - retrieve " << cdt.number_of_faces() << " triangles " ;
        TriangulatedSurface<K> triangulatedSurface ;
        triangulatedSurface.reserve( cdt.number_of_faces() );
        for ( Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it ) {
            triangulatedSurface.push_back(Triangle<K>(
                it->vertex( 0 )->point(),
                it->vertex( 1 )->point(),
                it->vertex( 2 )->point()
            ));
        }
        return triangulatedSurface ;
    }

} // algorithm
} // SFCGAL

#endif

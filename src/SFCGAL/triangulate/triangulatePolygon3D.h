/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_TRIANGULATE_TRIANGULATEPOLYGON3D_H_
#define _SFCGAL_TRIANGULATE_TRIANGULATEPOLYGON3D_H_

#include <SFCGAL/config.h>
#include <SFCGAL/log.h>

#include <SFCGAL/Geometry.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_2_filtered_projection_traits_3.h>

#include <SFCGAL/detail/collectPointsAndConstraints.h>
#include <SFCGAL/algorithm/plane.h>
#include <SFCGAL/triangulate/detail/markDomains.h>
#include <SFCGAL/io/wkt.h>

namespace SFCGAL {
namespace triangulate {
    
    /**
     * Triangulate a 3D polygon with holes and
     * output triangles in a TIN. The triangulation is 
     * performed in 2D in the polygon plane.
     * 
     * @warning Epeck is required as this implementation create points 
     *  on constraints intersections. Counting points could allow this 
     *  detections but polygon holes are allowed to touch the boundary.
     */
    template < typename K >
    void triangulatePolygon3D(
        const Polygon<K> & g,
        TriangulatedSurface<K> & triangulatedSurface
    ){
        /*
         * collect points and constraints
         */
        BOOST_LOG_TRIVIAL(info) << "triangulatePolygon3D - collect points and constraints...";
        std::vector< Point<K> > points ;
        std::vector< std::pair<size_t,size_t> > constraints;
        SFCGAL::detail::collectPointsAndConstraints(g,points,constraints);
        if ( points.empty() ){
            BOOST_LOG_TRIVIAL(trace) << "triangulatePolygon3D - skip empty polygon...";
            return ;
        }
        BOOST_LOG_TRIVIAL(trace) << "triangulatePolygon3D - "
        << points.size() << " point(s)," << constraints.size() << " containt(s) found" ;

        /*
         * Find polygon plane
         * TODO find plane from point set? only requires an approximative normal
         */
        BOOST_LOG_TRIVIAL(trace) << "triangulatePolygon3D - find polygon plane...";
        CGAL::Plane_3<K> polygonPlane = algorithm::plane3D<K>( g, false ) ;
        if ( polygonPlane.is_degenerate() ) {
            BOOST_THROW_EXCEPTION( Exception(
                ( boost::format( "can't find plane for polygon %s" ) % io::toWkt<K>(g) ).str()
            ) );
        }
        
        typedef detail::FaceInfoWithNestingLevel                             FaceInfo ;
        typedef CGAL::Triangulation_2_filtered_projection_traits_3<K>        Gt;
        typedef CGAL::Triangulation_vertex_base_2<Gt>                        Vb ;
        typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo,Gt>       Fb ;
        typedef CGAL::Constrained_triangulation_face_base_2<Gt,Fb>           Cfb ;
        typedef CGAL::Triangulation_data_structure_2< Vb,Cfb >               TDS ;

        typedef CGAL::Constrained_Delaunay_triangulation_2< Gt, TDS, CGAL::Exact_predicates_tag >  CDT;

        typedef typename CDT::Vertex_handle           Vertex_handle ;
        typedef typename CDT::Face_handle             Face_handle ;
        typedef typename CDT::All_faces_iterator      All_faces_iterator ;
        typedef typename CDT::Finite_faces_iterator   Finite_faces_iterator ;
        
        Gt gt( polygonPlane.orthogonal_vector() );
        CDT cdt(gt);
        
        // fail to build (invoke default ctor of Gt in insert_constraints)
        // Triangulation_2/include/CGAL/Triangulation_2/insert_constraints.h#L55
        // CGAL::Spatial_sort_traits_adapter_2<Geom_traits, const Point*> sort_traits(&(points[0]));
        /*
        cdt.insert_constraints(
            points.begin(), points.end(),
            constraints.begin(), constraints.end()
        );
        */
        cdt.insert(points.begin(),points.end());
        for ( auto it = constraints.begin(); it != constraints.end(); ++it ){
            cdt.insert_constraint( points[it->first], points[it->second] );
        }

        BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - markDomains (" << cdt.number_of_faces() << " triangles) " ;
        detail::markDomains(cdt);
        
        BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - retrieve triangles according to nested level..." ;        
        triangulatedSurface.reserve( triangulatedSurface.size() + cdt.number_of_faces() );
        
        for ( Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it ) {
            if ( it->info().nestingLevel % 2 == 0 ) {
                BOOST_LOG_TRIVIAL(trace) << "triangulate2DZ - skip triangle in hole." ;
                continue ;
            }

            triangulatedSurface.push_back(Triangle<K>(
                it->vertex( 0 )->point(),
                it->vertex( 1 )->point(),
                it->vertex( 2 )->point()
            ));
        }
    }
    
    /**
     * Triangulate a MultiPolygon
     * @warning Could produce a TriangulatedSurface with an inconsistent 
     *  orientation.
     */
    template < typename K >
    void triangulatePolygon3D(
        const MultiPolygon<K> & g,
        TriangulatedSurface<K> & triangulatedSurface
    ){
        for ( const Polygon<K> & part : g ){
            triangulatePolygon3D(part,triangulatedSurface);
        }
    }

    
} // triangulate
} // SFCGAL


#endif

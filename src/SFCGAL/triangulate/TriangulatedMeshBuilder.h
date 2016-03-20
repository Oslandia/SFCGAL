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

#ifndef _SFCGAL_TRIANGULATE_TRIANGULATEDMESHBUILDER_H_
#define _SFCGAL_TRIANGULATE_TRIANGULATEDMESHBUILDER_H_

#include <SFCGAL/config.h>
#include <SFCGAL/log.h>
#include <SFCGAL/Exception.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/detail/VertexArrayBuilder.h>
#include <SFCGAL/triangulate/triangulatePolygon3D.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

namespace SFCGAL {
namespace triangulate {
    
    /**
     * Build SurfaceMesh or Polyhedron from a polygon soup (TriangulatedSurface, 
     * MultiPolygon, PolyhedralSurface).
     * 
     * Note that the resulting surface may have multiple connex parts.
     */
    template < typename K >
    class TriangulatedMeshBuilder {
    public:
        using CT = CGAL::Point_3<K> ;
        using VertexArrayBuilder = SFCGAL::detail::VertexArrayBuilder<CT> ;

        /**
         * Add a triangle
         */
        void addPolygon( const Triangle<K> & g ){
            _triangles.push_back(g);
        }

        /**
         * Add a Polygon
         */
        void addPolygon( const Polygon<K> & g ){
            triangulate::triangulatePolygon3D<K>(g,_triangles);
        }
        
        /**
         * Add a MultiPolygon
         */
        template < typename PolygonIterator >
        void addPolygons( PolygonIterator begin, PolygonIterator end ){
            for ( PolygonIterator it = begin; it != end; ++it ){
                addPolygon(*it);
            }
        }

        
        /**
         * Build the surface mesh ensuring that triangle orientation is 
         *  consistent.
         *
         * Mesh is either a CGAL::Polyhedron<K> or a CGAL::Surface_mesh<K> 
         *
         * @throw InvalidGeometryException if surface is not orientable
         */
         template < typename Mesh >
         void getMesh( Mesh & mesh ) const {
            /*
             * index triangles
             * TODO collectPointsAndPolygons(g,vertices,polygons)?
             */
            BOOST_LOG_TRIVIAL(info) << "buildSurfaceMesh - index triangle soup...";
            std::vector< CGAL::Point_3<K> > points ;
            VertexArrayBuilder vertexBuilder(points);

            std::vector< std::vector< size_t > > polygons ;
            polygons.reserve(_triangles.size()) ;
            for ( const Triangle<K> & triangle : _triangles ){
                std::vector< size_t > polygon(3);
                for ( int i = 0; i < 3; i++ ){
                    polygon[i] = vertexBuilder.addVertex(triangle.vertex(i).toPoint_3());
                }
                polygons.push_back(polygon);
            }
            
            /*
             * Orient polygon soup
             */
            BOOST_LOG_TRIVIAL(info) << "buildSurfaceMesh - orient polygon soup...";
            if ( ! CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons) ){
                BOOST_LOG_TRIVIAL(warning) << "buildSurfaceMesh - orient polygon soup fails!";
                BOOST_THROW_EXCEPTION( GeometryInvalidityException( 
                    "orient_polygon_soup fails on TIN" 
                ) );
            }
            BOOST_LOG_TRIVIAL(info) << "buildSurfaceMesh - polygon_soup_to_polygon_mesh...";
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);
        }

    private:
        /**
         * A soup of triangle
         */
        TriangulatedSurface<K> _triangles ;
    } ;
    
} // triangulate
} // SFCGAL

#endif

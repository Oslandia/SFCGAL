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

#ifndef _SFCGAL_TRIANGULATE_DETAIL_TESSELATEVISITOR_H_
#define _SFCGAL_TRIANGULATE_DETAIL_TESSELATEVISITOR_H_

#include <SFCGAL/types.h>

#include <SFCGAL/mesh/SurfaceMesh.h>
#include <SFCGAL/mesh/detail/ConnectedComponentsExtractor.h>

#include <SFCGAL/triangulate/triangulatePolygon3D.h>
#include <SFCGAL/triangulate/TriangulatedMeshBuilder.h>

#include <CGAL/Polygon_mesh_processing/orientation.h>

namespace SFCGAL {
namespace triangulate {
namespace detail {

    /**
     * Ensure that geometries are exclusively composed of triangles.     * 
     */
    template < typename K >
    class TesselateVisitor : public boost::static_visitor< std::unique_ptr<Geometry<K>> > {
    public:
        
        //TYPE_POLYGON
        std::unique_ptr<Geometry<K>> operator() ( const Polygon<K> & g){
            std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
            triangulate::triangulatePolygon3D( g, *triSurf );
            return std::unique_ptr<Geometry<K>>( triSurf.release() );
        }

        //TYPE_MULTIPOLYGON
        std::unique_ptr<Geometry<K>> operator() ( const MultiPolygon<K> & g){
            typedef mesh::SurfaceMesh<K> Mesh ;
            typedef TriangulatedMeshBuilder<K> MeshBuilder;
            typedef mesh::detail::ConnectedComponentsExtractor< Mesh > ConnectedComponentsExtractor;
            
            // create a triangulated mesh
            Mesh mesh;
            MeshBuilder meshBuilder ;
            meshBuilder.addPolygons( g.cbegin(), g.cend() ) ;
            meshBuilder.getMesh(mesh);

            // extract connected components as TIN
            ConnectedComponentsExtractor extractor(mesh) ;
            if ( extractor.numConnectedComponents() > 1 ){
                std::unique_ptr<GeometryCollection<K>> collection( new GeometryCollection<K>() ) ;
                for ( size_t i = 0; i < extractor.numConnectedComponents(); i++ ){
                    std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
                    extractor.extractTriangles(i,std::back_inserter(*triSurf)) ;
                    collection->push_back( triSurf.release() );
                }
                return std::unique_ptr<Geometry<K>>( collection.release() );
            }else{
                std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
                extractor.extractTriangles(0,std::back_inserter(*triSurf)) ;
                return std::unique_ptr<Geometry<K>>( triSurf.release() );
            }
        }

        //TYPE_POLYHEDRALSURFACE
        std::unique_ptr<Geometry<K>> operator() ( const PolyhedralSurface<K> & g){
            typedef mesh::SurfaceMesh<K> Mesh ;
            typedef TriangulatedMeshBuilder<K> MeshBuilder;
            
            // create a triangulated mesh
            Mesh mesh;
            MeshBuilder meshBuilder ;
            meshBuilder.addPolygons( g.cbegin(), g.cend() ) ;
            meshBuilder.getMesh(mesh);
            
            std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
            mesh::extractTriangles(mesh,std::back_inserter(*triSurf));
            return std::unique_ptr<Geometry<K>>( triSurf.release() );
        }

        //TYPE_SOLID
        std::unique_ptr<Geometry<K>> operator() ( const Solid<K> & g ){
            typedef mesh::SurfaceMesh<K> Mesh ;
            typedef TriangulatedMeshBuilder<K> MeshBuilder;
            
            std::unique_ptr<Solid<K>> ret( new Solid<K>() );

            for ( size_t i = 0; i < g.size(); i++ ){
                const PolyhedralSurface<K> & shellN = g[i];
                
                Mesh mesh;
                MeshBuilder meshBuilder ;
                meshBuilder.addPolygons( shellN.cbegin(), shellN.cend() ) ;
                meshBuilder.getMesh(mesh);
                
                if ( CGAL::is_closed(mesh) ){
                    if ( CGAL::Polygon_mesh_processing::is_outward_oriented(mesh) ){
                        if ( i != 0 ){
                            CGAL::Polygon_mesh_processing::reverse_face_orientations(mesh);
                        }
                    }else{
                        if ( i == 0 ){
                            CGAL::Polygon_mesh_processing::reverse_face_orientations(mesh);
                        }
                    }
                }

                //TODO check close and manage orientation
                PolyhedralSurface<K> triShellN;
                mesh::extractPolygons( mesh, std::back_inserter(triShellN) );
                ret->push_back(triShellN);
            }

            return std::unique_ptr<Geometry<K>>( ret.release() );
        }
        
        //TYPE_MULTISOLID
        std::unique_ptr<Geometry<K>> operator() ( const MultiSolid<K> & g){
            //TODO keep TYPE_MULTISOLID as TYPE_MULTISOLID
            std::unique_ptr<GeometryCollection<K>> ret( new GeometryCollection<K>() );

            for ( const Solid<K> & solid : g ) {
                ret->push_back( 
                    (*this)( solid ).release() 
                );
            }

            return std::unique_ptr<Geometry<K>>( ret.release() );
        }
        
        //TYPE_GEOMETRYCOLLECTION
        std::unique_ptr<Geometry<K>> operator() ( const GeometryCollection<K> & g){
            std::unique_ptr<GeometryCollection<K>> ret( new GeometryCollection<K>() );

            for ( const Geometry<K> & part : g ) {
                ret->push_back( 
                    SFCGAL::apply_visitor( *this, part ).release() 
                );
            }

            return std::unique_ptr<Geometry<K>>( ret.release() );
        }
        
        /**
         * clone geometry for other types
         */
        std::unique_ptr<Geometry<K>> operator() ( const Geometry<K> & g ){
            return std::unique_ptr<Geometry<K>>( g.clone() );
        }

    } ;

} // detail
} // triangulate
} // SFCGAL


#endif


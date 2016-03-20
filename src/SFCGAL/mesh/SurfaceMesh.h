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
#ifndef _SFCGAL_MESH_SURFACEMESH_H_
#define _SFCGAL_MESH_SURFACEMESH_H_

#include <SFCGAL/kernels.h>

#include <CGAL/Surface_mesh.h>

#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace mesh {

    /**
     * Shortcut to bring helpers on CGAL::Surface_mesh< CGAL::Point_3<K> >
     */
    template < typename K >
    using SurfaceMesh = CGAL::Surface_mesh< CGAL::Point_3<K> > ;

    /*
     * Small helper to extract a triangle from a face
     * (adapted from CGAL examples)
     * @pre face is a triangle
     */
    template < typename K >
    CGAL::Triangle_3<K> extractTriangle(
        const SurfaceMesh<K> & mesh, 
        typename SurfaceMesh<K>::Face_index f
    )
    {
        typename SurfaceMesh<K>::Halfedge_index hf = mesh.halfedge(f);
        CGAL::Point_3<K> a = mesh.point(mesh.source(hf));
        hf = mesh.next(hf);
        CGAL::Point_3<K> b = mesh.point(mesh.source(hf));
        hf = mesh.next(hf);
        CGAL::Point_3<K> c = mesh.point(mesh.source(hf));
        hf = mesh.next(hf);
        return CGAL::Triangle_3<K>(a, b, c);
    }

    /**
     * Dump triangles from SurfaceMesh
     * @pre the mesh is triangulated
     */
    template < typename K, typename OutputIterator >
    void extractTriangles( const SurfaceMesh<K> & mesh, OutputIterator out ){
        typedef typename SurfaceMesh<K>::Face_range::iterator face_iterator ;
        face_iterator it,end;
        for ( boost::tie(it,end) = mesh.faces(); it != end; ++it ){
            out = extractTriangle(mesh,*it);
            ++out;
        }
    }
    
    
    /*
     * Small helper to extract a polygon from a face
     * (adapted from CGAL examples)
     * TODO allow non triangle face
     * @pre face is a triangle
     */
    template < typename K >
    Polygon<K> extractPolygon(
        const SurfaceMesh<K> & mesh, 
        typename SurfaceMesh<K>::Face_index f
    )
    {
        auto triangle = extractTriangle(mesh,f);
        Polygon<K> polygon{LineString<K>{
            triangle.vertex(0),
            triangle.vertex(1),
            triangle.vertex(2),
            triangle.vertex(0)
        }};
        return polygon;
    }

    /**
     * Dump triangles from SurfaceMesh
     * @pre the mesh is triangulated
     */
    template < typename K, typename OutputIterator >
    void extractPolygons( const SurfaceMesh<K> & mesh, OutputIterator out ){
        typedef typename SurfaceMesh<K>::Face_range::iterator face_iterator ;
        face_iterator it,end;
        for ( boost::tie(it,end) = mesh.faces(); it != end; ++it ){
            out = extractPolygon(mesh,*it);
            ++out;
        }
    }


    

} // mesh
} // SFCGAL


#endif


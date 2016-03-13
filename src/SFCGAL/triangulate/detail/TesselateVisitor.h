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
#include <SFCGAL/triangulate/triangulatePolygon3D.h>


namespace SFCGAL {
namespace triangulate {
namespace detail {

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
            std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
            triangulate::triangulatePolygon3D( g, *triSurf );
            return std::unique_ptr<Geometry<K>>( triSurf.release() );
        }

        //TYPE_POLYHEDRALSURFACE
        std::unique_ptr<Geometry<K>> operator() ( const PolyhedralSurface<K> & g){
            //TODO keep POLYHEDRALSURFACE as triangulated POLYHEDRALSURFACE
            std::unique_ptr<TriangulatedSurface<K>> triSurf( new TriangulatedSurface<K>() ) ;
            triangulate::triangulatePolygon3D( g, *triSurf );
            return std::unique_ptr<Geometry<K>>( triSurf.release() );
        }

        //TYPE_SOLID
        std::unique_ptr<Geometry<K>> operator() ( const Solid<K> & g ){
            //TODO keep SOLID as SOLID
            std::unique_ptr<GeometryCollection<K>> ret( new GeometryCollection<K>() );

            for ( const PolyhedralSurface<K>& shellN : g ) {
                if ( ! shellN.isEmpty() ) {
                    ret->push_back( (*this)( shellN ).release() );
                }
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


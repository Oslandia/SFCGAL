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

#ifndef _SFCGAL_IO_DETAIL_OSGGEOMETRYBUILDER_H_
#define _SFCGAL_IO_DETAIL_OSGGEOMETRYBUILDER_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>

#include <osg/Geometry>

namespace SFCGAL {
namespace io {
namespace detail {

/**
 * @brief helper class to convert SFCGAL::Geometry to osg::Geometry
 * TODO complete and document type list
 */
template < typename K >
class OsgGeometryBuilder : public boost::static_visitor<> {
public:

    /**
     * Constructor initializing the osg::Geometry
     */
    OsgGeometryBuilder():
        _geometry(new osg::Geometry)
    {
        _geometry->setVertexArray( new osg::Vec3Array() );
        _geometry->setNormalArray( new osg::Vec3Array() );
    }


    /**
     * Get the resulting osg::Geometry
     */
    osg::ref_ptr<osg::Geometry> getOsgGeometry() const {
        return _geometry ;
    }
    
    /**
     * create a osg::Geometry from a Point
     */
    void addGeometry( const Geometry<K>& g ) {
        boost::apply_visitor(*this,g);
    }
    
    /**
     * Add a point to the osg::Geometry
     */
    void operator() ( const Point<K> & g ){
        osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>( _geometry->getVertexArray() );

        size_t start = vertices->size();
        osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, start );
        primitiveSet->push_back( createVertex( vertices, g ) );
        _geometry->addPrimitiveSet( primitiveSet );
    }

    /**
     * add a LineString to the osg::Geometry
     */
    void operator() ( const LineString<K>& g ){
        osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>( _geometry->getVertexArray() );

        size_t start = vertices->size() ;

        for ( size_t i = 0; i < g.size(); i++ ) {
            createVertex( vertices, g[i] ) ;
        }

        _geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, start, g.size() ) );
    }

    /**
     * add a Triangle to a osg::Geometry
     */
    void operator() ( const Triangle<K>& g )
    {
        osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>( _geometry->getVertexArray() );

        size_t start = vertices->size();
        osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, start );

        for ( size_t i = 0; i < 3; i++ ) {
            primitiveSet->push_back( createVertex( vertices, g.vertex( i ) ) );
        }

        _geometry->addPrimitiveSet( primitiveSet );
    }

    /**
     * add a Polygon to the osg::Geometry
     * TODO : restore
    void operator() ( const Polygon<K>& g ){
        TriangulatedSurface surf;
        triangulate::triangulatePolygon3D( g, surf );
        (*this)( geometry, surf );
    }
    */
     
    /**
     * add a TIN to the osg::Geometry
     */
    void operator() ( const TriangulatedSurface<K>& g ){
        osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>( _geometry->getVertexArray() );
        osg::Vec3Array* normals = static_cast<osg::Vec3Array*>( _geometry->getNormalArray() );

        size_t start = vertices->size() ;

        for ( const Triangle<K>& triangle : g ) {
            osg::Vec3 a = createVec3( triangle.vertex( 0 ) );
            osg::Vec3 b = createVec3( triangle.vertex( 1 ) );
            osg::Vec3 c = createVec3( triangle.vertex( 2 ) );

            //vertices
            createVertex( vertices, a ) ;
            createVertex( vertices, b ) ;
            createVertex( vertices, c ) ;

            //normal
            osg::Vec3 normal = ( c - b ) ^ ( a - b ) ;
            normal.normalize();

            normals->push_back( normal ) ;
            normals->push_back( normal ) ;
            normals->push_back( normal ) ;
        }

        _geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
        _geometry->addPrimitiveSet(  new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, start, g.size() * 3 ) );
    }

    /**
     * add a PolyhedralSurface to a osg::Geometry
     * TODO restore
    void operator() ( const PolyhedralSurface& ){
        TriangulatedSurface surf;
        triangulate::triangulatePolygon3D( g, surf );
        (*this)( surf );
    }
    */

    /**
     * add a Solid to a osg::Geometry
     * TODO restore
    vvoid operator() ( const Solid<K>& g){
        TriangulatedSurface triangulatedSurface ;

        for ( size_t i = 0; i < g.numShells(); i++ ) {
            triangulate::triangulatePolygon3D( g.shellN( i ), triangulatedSurface );
        }

        (*this)( triangulatedSurface );
    }
    */
     
    /**
     * add a GeometryCollection to a osg::Geometry
     */
    template < typename GT >
    void operator() ( const Collection<GT>& collection ){
        for ( const GT & geometry : collection ) {
            addGeometry( geometry );
        }
    }

    /**
     * Handle unexpected type
     * TODO remove
     */
    void operator() ( const Geometry<K> & g ){
        BOOST_THROW_EXCEPTION( Exception(
            ( boost::format( "can't convert %1% to osg::Geometry" ) % g.which() ).str()
        ) );
    }


private:
    /**
     * The produced osg::Geometry
     */
    osg::ref_ptr<osg::Geometry> _geometry ;
    
    
    /**
     * create a osg::Vec3 from a Point
     */
    osg::Vec3 createVec3( const Point<K>& g ) const {
        return osg::Vec3(
            CGAL::to_double( g.x() ),
            CGAL::to_double( g.y() ),
            CGAL::to_double( g.z() )
        ) ;
    }

    /**
     * create a vertex and returns its position in a vertice array
     */
    size_t createVertex( osg::Vec3Array* vertices, const Point<K>& g ) {
        return createVertex( vertices, createVec3( g ) );
    }
    /**
     * create a vertex and returns its position in a vertice array
     */
    size_t createVertex( osg::Vec3Array* vertices, const osg::Vec3& g ) {
        size_t id = vertices->size();
        vertices->push_back( g );
        return id ;
    }

};

}//detail
}//io
}//SFCGAL


#endif

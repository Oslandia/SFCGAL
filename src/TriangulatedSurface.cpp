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

#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
TriangulatedSurface::TriangulatedSurface():
    Surface(),
    _triangles()
{

}

///
///
///
TriangulatedSurface::TriangulatedSurface( const std::vector< Triangle >& triangles ):
    Surface()
{
    for ( size_t i = 0; i < triangles.size(); i++ ) {
        _triangles.push_back( triangles[i].clone() ) ;
    }
}

///
///
///
TriangulatedSurface::TriangulatedSurface( const TriangulatedSurface& other ):
    Surface(),
    _triangles( other._triangles )
{

}

///
///
///
TriangulatedSurface& TriangulatedSurface::operator = ( TriangulatedSurface other )
{
    swap( other );
    return *this ;
}

///
///
///
TriangulatedSurface::~TriangulatedSurface()
{

}

///
///
///
TriangulatedSurface* TriangulatedSurface::clone() const
{
    return new TriangulatedSurface( *this );
}

///
///
///
std::string  TriangulatedSurface::geometryType() const
{
    return "TriangulatedSurface" ;
}

///
///
///
GeometryType TriangulatedSurface::geometryTypeId() const
{
    return TYPE_TRIANGULATEDSURFACE ;
}

///
///
///
int TriangulatedSurface::dimension() const
{
    //surface
    return 2 ;
}

///
///
///
int TriangulatedSurface::coordinateDimension() const
{
    if ( _triangles.empty() ) {
        return 0 ;
    }
    else {
        return _triangles[0].coordinateDimension() ;
    }
}

///
///
///
bool TriangulatedSurface::isEmpty() const
{
    return _triangles.empty() ;
}

///
///
///
bool TriangulatedSurface::is3D() const
{
    return ! _triangles.empty() && _triangles.front().is3D() ;
}

///
///
///
bool TriangulatedSurface::isMeasured() const
{
    return ! _triangles.empty() && _triangles.front().isMeasured() ;
}


///
///
///
void  TriangulatedSurface::addTriangles( const TriangulatedSurface& other )
{
    for ( TriangulatedSurface::const_iterator it = other.begin(); it != other.end(); ++it ) {
        addTriangle( *it ) ;
    }
}


///
///
///
size_t  TriangulatedSurface::numGeometries() const
{
    return _triangles.size();
}

///
///
///
const Triangle&    TriangulatedSurface::geometryN( size_t const& n ) const
{
    BOOST_ASSERT( n < numGeometries() );
    return _triangles[n];
}

///
///
///
Triangle&    TriangulatedSurface::geometryN( size_t const& n )
{
    BOOST_ASSERT( n < numGeometries() );
    return _triangles[n];
}

///
///
///
void TriangulatedSurface::reserve( const size_t& n )
{
    _triangles.reserve( n );
}

///
///
///
void TriangulatedSurface::accept( GeometryVisitor& visitor )
{
    return visitor.visit( *this );
}

///
///
///
void TriangulatedSurface::accept( ConstGeometryVisitor& visitor ) const
{
    return visitor.visit( *this );
}


// Private class
// A modifier creating triangles from a TriangulatedSurface with the incremental builder.
template <class HDS>
class Triangulated2Polyhedron : public CGAL::Modifier_base<HDS> {
public:
    Triangulated2Polyhedron( const TriangulatedSurface& surf ) : surf( surf ) {}

    typedef typename HDS::Vertex                Vertex;
    typedef typename Vertex::Point              Point;
    typedef typename HDS::Traits                K;
    typedef std::map<Point, size_t>             PointMap;
    typedef std::set< std::pair<Point, Point> > HalfedgeSet;

    void operator()( HDS& hds ) {
        // Postcondition: `hds' is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true );
        B.begin_surface( /* vertices */ surf.numGeometries() * 3,
                                        /* facets */ surf.numGeometries(),
                                        /* halfedges */ surf.numGeometries() * 3 );

        size_t vertex_idx = 0;

        // first pass: insert vertices, only if they are not shared between faces
        // thanks to a binary tree (PointMap)
        for ( size_t i = 0; i < surf.numGeometries(); i++ ) {
            for ( size_t j = 0; j < 3; j++ ) {
                Point p = surf.geometryN( i ).vertex( j ).toPoint_3();

                if ( points.find( p ) == points.end() ) {
                    B.add_vertex( p );
                    points[p] = vertex_idx++;
                }
            }
        }

        // second pass: adjacent triangles must be built with compliant orientations
        // the two halfedges of a shared edge must be of opposite orientation

        // Extract from CGAL's documentation
        // "The convention is that the halfedges are oriented counterclockwise
        // around facets as seen from the outside of the polyhedron"

        for ( size_t i = 0; i < surf.numGeometries(); i++ ) {
            B.begin_facet();
            CGAL::Triangle_3<K> tri( surf.geometryN( i ).toTriangle_3() );
            CGAL::Point_3<K> pa( tri[0] );
            CGAL::Point_3<K> pb( tri[1] );
            CGAL::Point_3<K> pc( tri[2] );

            if ( edges.find( std::make_pair( pa, pb ) ) != edges.end() ||
                    edges.find( std::make_pair( pb, pc ) ) != edges.end() ||
                    edges.find( std::make_pair( pc, pa ) ) != edges.end() ) {
                BOOST_THROW_EXCEPTION( Exception( "When trying to build a CGAL::Polyhedron_3 from a TriangulatedSurface: bad orientation for "
                                                  + surf.geometryN( i ).asText()
                                                  + " consider using ConsistentOrientationBuilder first" ) );
            }

            B.add_vertex_to_facet( points[pa] );
            B.add_vertex_to_facet( points[pb] );
            B.add_vertex_to_facet( points[pc] );
            edges.insert( std::make_pair( pa, pb ) );
            edges.insert( std::make_pair( pb, pc ) );
            edges.insert( std::make_pair( pc, pa ) );
            B.end_facet();
        }

        B.end_surface();
    }
private:
    const TriangulatedSurface& surf;
    PointMap points;
    HalfedgeSet edges;
};

template <typename Polyhedron>
struct Plane_from_facet {
    typename Polyhedron::Plane_3 operator()( typename Polyhedron::Facet& f ) {
        typename Polyhedron::Halfedge_handle h = f.halfedge();
        return typename Polyhedron::Plane_3( h->vertex()->point(),
                                             h->next()->vertex()->point(),
                                             h->opposite()->vertex()->point() );
    }
};

template < typename K, typename Polyhedron >
std::auto_ptr<Polyhedron> TriangulatedSurface::toPolyhedron_3() const
{
    Polyhedron* poly = new Polyhedron();
    Triangulated2Polyhedron<typename Polyhedron::HalfedgeDS> converter( *this );
    poly->delegate( converter );

    // compute planes
    std::transform( poly->facets_begin(), poly->facets_end(), poly->planes_begin(), Plane_from_facet<Polyhedron>() );

    return std::auto_ptr<Polyhedron>( poly );
}

template SFCGAL_CXX_API std::auto_ptr< detail::MarkedPolyhedron > TriangulatedSurface::toPolyhedron_3<Kernel, detail::MarkedPolyhedron >() const;
template SFCGAL_CXX_API std::auto_ptr< CGAL::Polyhedron_3<Kernel> > TriangulatedSurface::toPolyhedron_3<Kernel, CGAL::Polyhedron_3<Kernel> >() const;

}//SFCGAL



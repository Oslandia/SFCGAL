/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_
#define _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_

#include <CGAL/Aff_transformation_3.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {
namespace transform {

/**
 * Wrapper for CGAL::Aff_transform_3
 * @todo unittest
 */
template < typename K >
class AffineTransform3 : public Transform {
public:
    /**
     * Constructor with a transform
     */
    AffineTransform3( CGAL::Aff_transformation_3< Kernel > transform ):
        _transform( transform ) {

    }

    /*
     * [SFCGAL::Transform]
     */
    virtual void transform( Point& p ) {
        if ( ! p.isEmpty() ) {
            p = Point( p.toPoint_3().transform( _transform ) ) ;
        }
    }

    virtual void transform( LineString& ls ) {
        for ( size_t i = 0; i < ls.numPoints(); ++i ) {
            transform( ls.pointN( i ) );
        }
    }
    virtual void transform( Triangle& tri ) {
        transform( tri.vertex( 0 ) );
        transform( tri.vertex( 1 ) );
        transform( tri.vertex( 2 ) );
    }
    virtual void transform( Polygon& poly ) {
        transform( poly.exteriorRing() );

        for ( size_t i = 0; i < poly.numInteriorRings(); ++i ) {
            transform( poly.interiorRingN( i ) );
        }
    }

    virtual void transform( PolyhedralSurface& surf ) {
        for ( size_t i = 0; i < surf.numPolygons(); ++i ) {
            transform( surf.polygonN( i ) );
        }
    }

    virtual void transform( TriangulatedSurface& surf ) {
        for ( size_t i = 0; i < surf.numGeometries(); ++i ) {
            transform( surf.geometryN( i ) );
        }
    }

    virtual void transform( Solid& solid ) {
        transform( solid.exteriorShell() );

        for ( size_t i = 0; i < solid.numInteriorShells(); ++i ) {
            transform( solid.interiorShellN( i ) );
        }
    }

private:
    CGAL::Aff_transformation_3< K > _transform ;
};


}//transform
}//SFCGAL




#endif


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

#ifndef _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_
#define _SFCGAL_TRANSFORM_AFFINETRANSFORM3_H_

#include <SFCGAL/config.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/Transform.h>

#include <CGAL/Aff_transformation_3.h>

namespace SFCGAL {
namespace transform {

/**
 * Wrapper for CGAL::Aff_transform_3
 * @todo unittest
 */
class SFCGAL_CXX_API AffineTransform3 : public Transform {
public:
    /**
     * Constructor with a transform
     */
    AffineTransform3( CGAL::Aff_transformation_3< Kernel > transform ) ;

    /*
     * [SFCGAL::Transform]
     */
    virtual void transform( Point& p ) ;

    virtual void transform( LineString& ls ) ;
    virtual void transform( Triangle& tri ) ;
    virtual void transform( Polygon& poly ) ;

    virtual void transform( PolyhedralSurface& surf ) ;

    virtual void transform( TriangulatedSurface& surf ) ;

    virtual void transform( Solid& solid ) ;

private:
    CGAL::Aff_transformation_3< Kernel > _transform ;
};


}//transform
}//SFCGAL




#endif


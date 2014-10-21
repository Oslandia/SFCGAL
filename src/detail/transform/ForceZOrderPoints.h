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

#ifndef _SFCGAL_TRANSFORM_FORCEZ_ORDER_POINTS_H_
#define _SFCGAL_TRANSFORM_FORCEZ_ORDER_POINTS_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {

namespace transform {

/**
 * Force Z. If the 2D surface is pointing down, reverse its points
 *
 * @todo unittest
 * @todo move outside (it's not a coordinate transform)?
 */
class SFCGAL_API ForceZOrderPoints : public Transform {
public:
    /**
     * Constructor with a default Z value
     */
    ForceZOrderPoints( const Kernel::FT& defaultZ = 0 ) ;

    /*
     * [SFCGAL::Transform]
     */
    virtual void transform( Point& p ) ;

    virtual void visit( Triangle& );
    virtual void visit( Polygon& );

private:
    Kernel::FT _defaultZ ;
};


}//transform
}//SFCGAL




#endif


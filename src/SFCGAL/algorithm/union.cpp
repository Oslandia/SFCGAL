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

#include <SFCGAL/algorithm/union.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {

namespace algorithm {

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    (void)ga;
    (void)gb;
    return std::auto_ptr<Geometry>();
}

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );

    return union_( ga, gb, NoValidityCheck() );
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    (void)ga;
    (void)gb;
    return std::auto_ptr<Geometry>();
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );

    return union3D( ga, gb, NoValidityCheck() );
}
}
}

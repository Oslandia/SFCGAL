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

#ifndef SFCGAL_UNION_ALGORITHM
#define SFCGAL_UNION_ALGORITHM

#include <SFCGAL/config.h>

#include <memory>

namespace SFCGAL {
class Geometry;
namespace detail {
template <int Dim> class GeometrySet;
template <int Dim> struct PrimitiveHandle;
}

namespace algorithm {
struct NoValidityCheck;

/**
 * Union on 2D geometries.
 * @pre ga and gb are valid geometries
 * @ingroup public_api
 */
SFCGAL_CXX_API std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb );

/**
 * Union on 2D geometries. No validity check variant
 * @pre ga and gb are valid geometries
 * @ingroup detail
 * @warning No actual validity check is done.
 */
SFCGAL_CXX_API std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb,NoValidityCheck );

/**
 * Union on 3D geometries. Assume z = 0 if needed
 * @pre ga and gb are valid geometries
 * @ingroup public_api
 */
SFCGAL_CXX_API std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb );

/**
 * Union on 3D geometries. Assume z = 0 if needed
 * @pre ga and gb are valid geometries
 * @ingroup detail
 * @warning@ No actual validity check is done
 */
SFCGAL_CXX_API std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb, NoValidityCheck );

/**
 * @ingroup detail
 */
template <int Dim>
void union_( const detail::GeometrySet<Dim>& a, const detail::GeometrySet<Dim>& b, detail::GeometrySet<Dim>& );

/**
 * @ingroup detail
 */
template <int Dim>
void union_( const detail::PrimitiveHandle<Dim>& a, const detail::PrimitiveHandle<Dim>& b, detail::GeometrySet<Dim>& );

}
}

#endif

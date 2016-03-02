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
#ifndef _SFCGAL_TRIANGLE_H_
#define _SFCGAL_TRIANGLE_H_

#include <SFCGAL/kernels.h>

namespace SFCGAL {

    namespace detail {
        template < typename K, int N >
        struct TriangleTraits {

        } ;

        template < typename K>
        struct TriangleTraits<K,2> {
            typedef CGAL::Triangle_2< K > Type ;
        } ;

        template < typename K>
        struct TriangleTraits<K,3> {
            typedef CGAL::Triangle_3< K > Type ;
        } ;
    } // namespace

    /**
     * SFCGAL Triangle with a generic dimension
     */
    template < typename K, int N >
    using Triangle = typename detail::TriangleTraits<K,N>::Type ;


} // namespace SFCGAL

#endif

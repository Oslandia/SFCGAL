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

#ifndef _SFCGAL_DETAIL_COORDINATETOPOINT2VISITOR_H_
#define _SFCGAL_DETAIL_COORDINATETOPOINT2VISITOR_H_

namespace SFCGAL {
    class Empty ;
namespace detail {

    template < typename K2 >
    class CoordinateToPoint2Visitor : public boost::static_visitor<typename K2::Point_2> {
    public:
        typedef typename K2::Point_2 result_type ;

        result_type operator()( const Empty& ) const {
            return result_type( CGAL::ORIGIN );
        }
        
        template < typename K1 >
        result_type operator()( const CGAL::Point_2<K1>& storage ) const {
            CGAL::Cartesian_converter<K1,K2> converter ;
            return converter( storage );
        }
        
        template < typename K1 >
        result_type operator()( const CGAL::Point_3<K1>& storage ) const {
            CGAL::Cartesian_converter<K1,K2> converter ;
            return converter(
                CGAL::Point_2<K1>( storage.x(), storage.y() )
            );
        }
    };

}//detail
}//SFCGAL

#endif

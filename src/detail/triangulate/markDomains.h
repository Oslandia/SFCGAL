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

#ifndef _SFCGAL_TRIANGULATE_DETAIL_MARKDOMAINS_H_
#define _SFCGAL_TRIANGULATE_DETAIL_MARKDOMAINS_H_


namespace SFCGAL {
namespace triangulate {
namespace detail {

/**
 * @brief fill nestingLevel
 *
 * Adapted from CGAL-4.1/examples/Triangulation_2/polygon_triangulation.cpp
 */
template < typename CDT >
void markDomains(
    CDT& cdt,
    typename CDT::Face_handle start,
    int index,
    std::list< typename CDT::Edge >& border
)
{
    typedef typename CDT::Face_handle Face_handle ;
    typedef typename CDT::Edge        Edge ;

    if ( start->info().nestingLevel != -1 ) {
        return;
    }

    std::list< Face_handle > queue;
    queue.push_back( start );

    while ( !queue.empty() ) {
        Face_handle fh = queue.front();
        queue.pop_front();

        if ( fh->info().nestingLevel == -1 ) {
            fh->info().nestingLevel = index;

            for ( int i = 0; i < 3; i++ ) {
                Edge e( fh, i );
                Face_handle n = fh->neighbor( i );

                if ( n->info().nestingLevel == -1 ) {
                    if ( cdt.is_constrained( e ) ) {
                        border.push_back( e );
                    }
                    else {
                        queue.push_back( n );
                    }
                }
            }
        }
    }
}

/**
 * @brief fill nestingLevel
 *
 * Adapted from CGAL-4.1/examples/Triangulation_2/polygon_triangulation.cpp
 */
template < typename CDT >
void markDomains( CDT& cdt )
{
    typedef typename CDT::All_faces_iterator All_faces_iterator ;
    typedef typename CDT::Face_handle        Face_handle ;
    typedef typename CDT::Edge               Edge ;

    for ( All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it ) {
        it->info().nestingLevel = -1;
    }

    int index = 0;
    std::list< Edge > border;
    markDomains( cdt, cdt.infinite_face(), index++, border );

    while ( ! border.empty() ) {
        Edge e = border.front();
        border.pop_front();
        Face_handle n = e.first->neighbor( e.second );

        if ( n->info().nestingLevel == -1 ) {
            markDomains( cdt, n, e.first->info().nestingLevel + 1, border );
        }
    }
}

}//namespace detail
}//namespace triangulate
}//namespace SFCGAL


#endif

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
#ifndef _SFCGAL_COLLECTION_H_
#define _SFCGAL_COLLECTION_H_

#include <vector>
#include <SFCGAL/Point.h>

namespace SFCGAL {
        
    /**
     * A wrapper to provide strong typing on geometry collections
     */
    template < typename GeometryType > 
    class Collection {
    public:
        typedef GeometryType                                       value_type ;
        typedef std::vector< value_type >                          container_t ;
        typedef typename std::vector< value_type >::iterator       iterator ;
        typedef typename std::vector< value_type >::const_iterator const_iterator ;

        Collection() = default ;
        
        template < typename Iterator >
        Collection( Iterator & begin, Iterator & end ):
            _geometries(begin,end)
        {
            
        }
        
        Collection( const Collection & other ) = default;
        
        ~Collection(){}
        
        size_t size() const {
            return _geometries.size() ;
        }
        
        void reserve( size_t n ){
            _geometries.reserve(n);
        }
        
        void push_back( const value_type & geometry ){
            _geometries.push_back(geometry);
        }
        
        value_type & geometry( const size_t & n ) {
            return _geometries[n] ;
        }
        const value_type & geometry( const size_t & n ) const {
            return _geometries[n] ;
        }
        
        
        const_iterator begin() const { return _geometries.begin() ; }
        const_iterator end() const { return _geometries.end() ; }

        iterator       begin() { return _geometries.begin() ; }
        iterator       end() { return _geometries.end() ; }
                
    private:
        container_t _geometries ;
    } ;

}

#endif

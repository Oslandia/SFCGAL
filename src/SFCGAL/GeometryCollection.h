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
#ifndef _SFCGAL_GEOMETRYCOLLECTION_H_
#define _SFCGAL_GEOMETRYCOLLECTION_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
        
    /**
     * A geometry collection
     */
    template < typename K > 
    class GeometryCollection : public Geometry<K>, public boost::ptr_vector< Geometry<K> > {
        using Container = boost::ptr_vector< Geometry<K> > ;
    public:
        using Kernel = K ;

        // forward Container's ctor's
        using Container::Container;

        virtual ~GeometryCollection(){}

        /**
         * Get n'th geometry
         * @deprecated use g.size()
         */        
        int numGeometries() const {
            return this->size() ;
        }
        /**
         * Get n'th geometry
         * @deprecated use g[i] or g.at(i)
         */
        Geometry<K> & geometryN( const size_t & n ) {
            BOOST_ASSERT( n < this->size() );
            return (*this)[n] ;
        }
        /**
         * Get n'th geometry
         * @deprecated use g[i] or g.at(i)
         */
        const Geometry<K> & geometryN( const size_t & n ) const {
            BOOST_ASSERT( n < this->size() );
            return (*this)[n] ;
        }

        //-- IGeometry
        virtual bool isEmpty() const {
            return this->empty() ;
        }
        //-- IGeometry
        virtual bool is3D() const {
            return (! isEmpty()) && this->front().is3D();
        }
        //-- IGeometry
        virtual bool isMeasured() const {
            return (! isEmpty()) && this->front().isMeasured();
        }
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_GEOMETRYCOLLECTION ;
        }
        //--- IGeometry
        virtual std::string geometryType() {
            return "GeometryCollection";
        }
    } ;

}

#endif

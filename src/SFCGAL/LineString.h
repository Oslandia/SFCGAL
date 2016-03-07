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
#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <SFCGAL/kernels.h>
#include <SFCGAL/Point.h>

namespace SFCGAL {
    
    /**
     * A polyline defined by a list of points
     */
    template < typename K >
    class LineString : public Geometry<K>, public std::vector< Point<K> > {
        using Base = std::vector< Point<K> > ;
    public:
        using Kernel = K ;

        // forward std::vector constructors
        using Base::Base;
        
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_LINESTRING ;
        }
        //--- IGeometry
        virtual std::string geometryType() const {
            return "LineString";
        }
        //-- IGeometry
        virtual bool isEmpty() const {
            return this->empty() ;
        }
        //-- IGeometry
        virtual bool is3D() const {
            return (! isEmpty()) && startPoint().is3D();
        }
        //-- IGeometry
        virtual bool isMeasured() const {
            return (! isEmpty()) && startPoint().isMeasured();
        }
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new LineString<K>(*this);
        }
        
        /**
         * Test if LineString is closed
         */
        bool isClosed() const {
            return ( ! isEmpty() ) && ( startPoint() == endPoint() ) ;
        }

        /**
         * Get start point
         * @deprecated use front()
         */
        const Point<K> & startPoint() const {
            BOOST_ASSERT( ! this->empty() );
            return this->front();
        }
        
        /**
         * Get end point
         * @deprecated use back()
         */
        const Point<K> & endPoint() const {
            BOOST_ASSERT( ! this->empty() );    
            return this->back();
        }
        
        
        /**
         * [SFA/OGC]Returns the number of points
         * 
         * @deprecated use g.size()
         */
        size_t          numPoints() const {
            return this->size();
        }

        /**
        * [SFA/OGC]Returns the n-th point
        * 
        * @deprecated use g[n] or g.at(n)
        */
        const Point<K>&    pointN( size_t const& n ) const {
            BOOST_ASSERT( n < numPoints() ) ;
            return (*this)[n];
        }

        /**
         * [SFA/OGC]Returns the n-th point
         * 
         * @deprecated use g[n] or g.at(n)
         */
        Point<K>&          pointN( size_t const& n ) {
            BOOST_ASSERT( n < numPoints() ) ;
            return (*this)[n];
        }

    };
    
    

} // SFCGAL

#endif

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
#ifndef _SFCGAL_POLYGON_H_
#define _SFCGAL_POLYGON_H_

#include <SFCGAL/log.h>
#include <SFCGAL/kernels.h>
#include <SFCGAL/LineString.h>

namespace SFCGAL {

    /**
     * A polygon with holes
     * 
     * note : v1.3.* rely on the fact that the internal vector is never empty. This 
     *  behavior change in v2 : Polygon is no more initialized to size 1. Instead, 
     *  a warning hack that using exteriorRing create an empty exteriorRing if missing.
     */
    template < typename K >
    class Polygon : public Geometry<K>, public std::vector< LineString<K> > {
        using Base = std::vector< LineString<K> > ;
    public:
        using Kernel = K ;
    
        // forward ctor from std::vector<LineString<K>>
        using Base::Base ;

        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_POLYGON ;
        }
        //--- IGeometry
        virtual std::string geometryType() {
            return "Polygon";
        }
        //--- IGeometry
        virtual bool isEmpty() const {
            return this->empty() || this->front().isEmpty() ;            
        }
        //--- IGeometry
        virtual bool is3D() const {
            return (! isEmpty()) && this->front().is3D();
        }
        //--- IGeometry
        virtual bool isMeasured() const {
            return (! isEmpty()) && this->front().isMeasured();
        }
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new Polygon<K>(*this);
        }

        /**
         * [OGC/SFA]returns the exterior ring
         */
        const LineString<K>&     exteriorRing() const {
            ensureExteriorRingExists();
            return this->front();
        }
        /**
         * [OGC/SFA]returns the exterior ring
         */
        LineString<K>&   exteriorRing() {
            ensureExteriorRingExists();
            return this->front();
        }
        /**
         * Sets the exterior ring
         */
        void  setExteriorRing( const LineString<K>& ring ) {
            if ( this->empty() ){
                this->push_back(ring);
            }else{
                this->front() = ring ;
            }
        }

        /**
         * Test if the polygon has interior rings
         */
        bool  hasInteriorRings() const {
            return this->size() > 1 ;
        }

        /**
         * [OGC/SFA]returns the exterior ring
         */
        size_t numInteriorRings() const {
            return std::max( 0, numRings() - 1 ) ;
        }
        /**
         * [OGC/SFA]returns the n'th interior ring
         */
        const LineString<K>&     interiorRingN( const int& n ) const {
            BOOST_ASSERT( n+1 < this->size() );
            return (*this)[n+1];
        }
        /**
         * [OGC/SFA]returns the n'th interior ring
         */
        LineString<K>&  interiorRingN( const int& n ) {
            BOOST_ASSERT( n+1 < this->size() );
            return (*this)[n+1];
        }

        /**
         * Returns the number of rings
         *
         * @deprecated use polygon.size()
         */
        int  numRings() const {
            return static_cast< int >( this->size() ) ;
        }
        /**
         * Returns the n-th ring, 0 is exteriorRing
         * @warning not standard, avoid conditionnal to access rings
         * 
         * @deprecated use polygon[n] or polygon.at(n)
         */
        const LineString<K>&     ringN( const size_t& n ) const {
            BOOST_ASSERT( n < this->size() );
            return (*this)[n];
        }
        /**
         * Returns the n-th ring, 0 is exteriorRing
         * @warning not standard, avoid conditionnal to access rings
         *
         * @deprecated use polygon[n] or polygon.at(n)
         */
        LineString<K>&           ringN( const size_t& n ) {
            BOOST_ASSERT( n < this->size() );
            return (*this)[n];
        }

    private:
        /**
         * This method is a hack invoked by exteriorRing accessors. It simplify
         * the port of algorithm relying on the initialization of the exteriorRing
         * in 1.3.* Polygon constructor's. 
         */
        void ensureExteriorRingExists() const {
            if ( this->empty() ){
                BOOST_LOG_TRIVIAL(warning) << "automatic creation of exteriorRing (remove acess to exteriorRing on empty polygon)";
                const_cast< Polygon<K> * >(this)->resize(1);
            }
        }
        
    };
    

} // SFCGAL

#endif

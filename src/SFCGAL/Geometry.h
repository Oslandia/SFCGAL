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
#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <SFCGAL/kernels.h>
#include <SFCGAL/GeometryType.h>

namespace SFCGAL {

    typedef int srid_t ;
    
    /**
     * Base class for geometries with static kernel
     */
    template < typename K >
    class Geometry {
    public:
        using Kernel = K ;

        virtual ~Geometry(){}
        
        /**
         * Clone the geometry
         */
        virtual Geometry<K>* clone() const = 0 ;
        
        /**
         * Test if geometry is instance of T
         */
        template < typename T >
        bool is() const { 
            return dynamic_cast<const T*>(this) != nullptr; 
        }

        /**
         * Downcast to T&
         */
        template < typename T >
        T & as() { 
            T* result = dynamic_cast<T*>(this) ;
            BOOST_ASSERT(result != nullptr);
            return *result ;  
        }
        /**
         * Downcast to const T&
         */
        template < typename T >
        const T & as() const { 
            const T* result = dynamic_cast<const T*>(this) ;
            BOOST_ASSERT(result != nullptr);
            return *result ;  
        }
        
        /**
         * Get GeometryType identifier
         */
        virtual GeometryType geometryTypeId() const = 0 ;
        /**
         * Get GeometryType name
         */
        virtual std::string geometryType() const = 0 ;
        
        /**
         * Indicates if the geometry is empty
         */
        virtual bool isEmpty() const = 0 ;
        /**
         * Indicates if the geometry is 3D
         */
        virtual bool is3D() const = 0 ;
        /**
         * Indicates if the geometry has M coordinates
         */
        virtual bool isMeasured() const = 0 ;

    protected:
        Geometry() = default ;
        Geometry( const Geometry& other ) = default;
    } ;


} // SFCGAL

#endif

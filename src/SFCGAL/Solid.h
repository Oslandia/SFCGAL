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
#ifndef _SFCGAL_SOLID_H_
#define _SFCGAL_SOLID_H_

#include <SFCGAL/Collection.h>
#include <SFCGAL/PolyhedralSurface.h>

namespace SFCGAL {

    /**
     * A solid where shells are represented by MultiPolygon (PolyhedralSurface)
     * The first one is the exterior shell, the other are interior shells
     *
     * @warning A 2D solid is an artefact to provide the same type list
     *  in the Geometry variant for 2D and 3D
     *
     * TODO check if exteriorShell was initialized in 1.3.*, restore 
     *   deprecated accessors if needed by algorithms
     */
    template < typename K >
    class Solid : public Geometry<K>, public std::vector< PolyhedralSurface<K> > {
        using Container = std::vector< PolyhedralSurface<K> >;
    public:
        using Kernel = K ;
    
        // forward Container's ctor
        using Container::Container;
        
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_SOLID ;
        }
        //--- IGeometry
        virtual std::string geometryType() {
            return "Solid";
        }
        //--- IGeometry
        bool isEmpty() const {
            return this->empty() ;
        }
        //--- IGeometry
        bool is3D() const {
            return ( ! isEmpty() ) && this->front().is3D() ;
        }
        //--- IGeometry
        bool isMeasured() const {
            return ( ! isEmpty() ) && this->front().isMeasured() ;
        }
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new Solid<K>(*this);
        }

    } ;

} // SFCGAL

#endif

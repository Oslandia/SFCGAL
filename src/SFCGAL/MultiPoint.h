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
#ifndef _SFCGAL_MULTIPOINT_H_
#define _SFCGAL_MULTIPOINT_H_

#include <SFCGAL/Point.h>
#include <SFCGAL/Collection.h>

namespace SFCGAL {
    
    /**
     * A collection of Point
     */
    template < typename K >
    class MultiPoint : public Collection< Point<K> > {
        using Base = Collection< Point<K> >;
    public:
        using Kernel = K ;

        //-- forward Collection's ctor
        using Base::Base;
        
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_MULTIPOINT ;
        }
        //--- IGeometry
        virtual std::string geometryType() const {
            return "MultiPoint";
        }
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new MultiPoint<K>(*this);
        }
    };

} // SFCGAL

#endif

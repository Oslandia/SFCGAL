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
#ifndef _SFCGAL_POINT_H_
#define _SFCGAL_POINT_H_

#include <SFCGAL/kernels.h>
#include <SFCGAL/numeric.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    
    /**
     * 
     * SFCGAL's Point add metadata to a CGAL::Point_3 that should be considered as
     * it's coordinate. It's implicitely castable to CGAL::Point_3 and CGAL::Point_2.
     * 
     * Simple Feature points can be Empty, 2D or 3D. They are
     * converted to 3D and tagged with their dimension.
     */
    template < typename K >
    class Point : public Geometry<K> {
    public:
        using Kernel = K ;
        using FT = typename K::FT ;

        /**
         * Empty point constructor
         */
        Point():
            _coordinate(CGAL::ORIGIN),
            _empty(true),
            _3d(false),
            _m(SFCGAL::NaN())
        {
            
        }
        
        /**
         * Empty point constructor
         */
        Point( const FT & x, const FT & y ):
            _coordinate(x,y,FT(0)),
            _empty(false),
            _3d(false),
            _m(SFCGAL::NaN())
        {
            
        }
        
        /**
         * 2D point constructor
         */
        Point( const CGAL::Point_2<K> & other ):
            _coordinate(other.x(),other.y(),FT(0)),
            _empty(false),
            _3d(false),
            _m(SFCGAL::NaN())
        {
            
        }
        
        /**
         * 3D point constructor
         */
        Point( const FT & x, const FT & y, const FT & z ):
            _coordinate(x,y,z),
            _empty(false),
            _3d(true),
            _m(SFCGAL::NaN())
        {
            
        }

        /**
         * 3D point constructor
         */
        Point( const CGAL::Point_3<K> & other ):
            _coordinate(other),
            _empty(false),
            _3d(true),
            _m(SFCGAL::NaN())
        {
            
        }

        Point( const Point<K> & ) = default;
        Point( Point<K>&& ) = default;
        
        Point<K> & operator = ( const Point<K> & ) = default;
        Point<K> & operator = ( Point<K>&& ) = default;        
        
        ~Point() = default;
        
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_POINT ;
        }
        //--- IGeometry
        virtual std::string geometryType() const {
            return "Point";
        }
        //--- IGeometry
        virtual bool isEmpty() const {
            return _empty ;
        }
        //--- IGeometry
        virtual bool is3D() const {
            return _3d ;
        }
        //--- IGeometry
        virtual bool isMeasured() const {
            return ! SFCGAL::isNaN(_m);
        }
        
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new Point<K>(*this);
        }
        
        /**
         * Get M coordinate
         */
        double & m() { return _m ; }
        /**
         * Get M coordinate
         */
        const double & m() const { return _m ; }
                
        /**
         * Get the 2D point coordinate
         */
        CGAL::Point_2<K> toPoint_2() const {
            return CGAL::Point_2<K>(_coordinate.x(),_coordinate.y());
        }

        /**
         * Get the 3D point coordinate
         */
        const CGAL::Point_3<K>& toPoint_3() const {
            return _coordinate;
        }

        /**
         * Get the 2D vector from ORIGIN
         */
        CGAL::Vector_2<K> toVector_2() const {
            return toPoint_2() - CGAL::ORIGIN ;
        }
        
        /**
         * Get the 3D vector from ORIGIN
         */
        CGAL::Vector_3<K> toVector_3() const {
            return toPoint_3() - CGAL::ORIGIN ;
        }
        
        /**
         *
         * @deprecated use CGAL data structures to perform operations
         */
        FT x() const {
            return _coordinate.x();
        }
        /**
         *
         * @deprecated use CGAL data structures to perform operations
         */
        FT y() const {
            return _coordinate.y();
        }
        /**
         *
         * @deprecated use CGAL data structures to perform operations
         */
        FT z() const {
            return _coordinate.z();
        }
        
        /**
         *  Compare point (based on 3D coordinate comparison)
         *
         * @deprecated performs comparison on CGAL data structures
         */
        bool operator < ( const Point<K> & other ) const {
            return _coordinate < other._coordinate ;
        }
        /**
         * Compare point (based on 3D coordinate comparison)
         * @deprecated performs comparison on CGAL data structures
         */
        bool operator != ( const Point<K> & other ) const {
            return _coordinate != other._coordinate ;
        }
        /**
         * Compare point (based on 3D coordinate comparison)
         * @deprecated performs comparison on CGAL data structures
         */
        bool operator == ( const Point<K> & other ) const {
            return _coordinate == other._coordinate ;
        }
        
    private:
        CGAL::Point_3<K> _coordinate;
        /**
         * measure attached to the point
         * TODO customize a kernel so that the measure persists throw CGAL algorithm
         *   invocation.
         */
        double _m ;
        /**
         * Tag for empty coordinates
         */
        bool _empty ;
        /**
         * Tag for 3D coordinate
         */
        bool _3d ;
    } ;


} // namespace SFCGAL

#endif

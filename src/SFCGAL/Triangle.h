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
#ifndef _SFCGAL_TRIANGLE_H_
#define _SFCGAL_TRIANGLE_H_

#include <SFCGAL/kernels.h>
#include <SFCGAL/Point.h>

namespace SFCGAL {

    /**
     * SFCGAL's Triangle is defined with 3 points
     */
    template < typename K >
    class Triangle : public Geometry<K> {
    public:
        using Kernel = K ;

        /**
         * Empty triangle constructor
         */
        Triangle(){}
        
        /**
         * Constructor with 3 points
         */
        Triangle( const Point<K> & a, const Point<K> & b, const Point<K> & c ){
            _vertices[0] = a ;
            _vertices[1] = b ;
            _vertices[2] = c ;       
        }
        
        /**
         * Constructor from CGAL::Triangle_2
         */
        Triangle(const CGAL::Triangle_2<K> & g ){
            for ( int i = 0; i < 3; i++ ){
                _vertices[i] = Point<K>(g.vertex(i));
            }
        }
        
        /**
         * Constructor from CGAL::Triangle_3
         */
        Triangle(const CGAL::Triangle_3<K> & g ){
            for ( int i = 0; i < 3; i++ ){
                _vertices[i] = Point<K>(g.vertex(i));
            }
        }
        
        Triangle( const Triangle & ) = default ;
        Triangle( Triangle && ) = default ;
        
        Triangle & operator = ( const Triangle & ) = default ;
        Triangle & operator = ( Triangle && ) = default ;
        
        ~Triangle() = default;
        
        //--- IGeometry
        virtual GeometryType geometryTypeId() const {
            return TYPE_TRIANGLE ;
        }
        //--- IGeometry
        virtual std::string geometryType() {
            return "Triangle";
        }
        //--- IGeometry
        bool isEmpty() const {
            return _vertices[0].isEmpty() ;
        }
        //--- IGeometry
        bool is3D() const {
            return _vertices[0].is3D() ;
        }
        //--- IGeometry
        bool isMeasured() const {
            return _vertices[0].isMeasured() ;
        }
        //--- Geometry<K>
        virtual Geometry<K>* clone() const {
            return new Triangle<K>(*this);
        }

        
        Point<K> & vertex(const int & n) {
            return _vertices[n%3];
        }
        const Point<K> & vertex(const int & n) const {
            return _vertices[n%3];
        }
        
        /**
         * Get the 2D triangle
         */
        CGAL::Triangle_2<K> toTriangle_2() const {
            return CGAL::Triangle_2<K>(
                _vertices[0].toPoint_2(),
                _vertices[1].toPoint_2(),            
                _vertices[2].toPoint_2()                                
            ) ;
        }
        
        /**
         * Get the 3D triangle
         */
        CGAL::Triangle_3<K> toTriangle_3() const {
            return CGAL::Triangle_3<K>(
                _vertices[0].toPoint_3(),
                _vertices[1].toPoint_3(),            
                _vertices[2].toPoint_3()                                
            ) ;
        }
        
    private:
        Point<K> _vertices[3] ;
    } ;


} // namespace SFCGAL

#endif

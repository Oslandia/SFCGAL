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

#ifndef _SFCGAL_ENVELOPE_H_
#define _SFCGAL_ENVELOPE_H_

#include <memory>
#include <boost/assert.hpp>

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>

#include <SFCGAL/config.h>

#include <SFCGAL/detail/Interval.h>
#include <SFCGAL/Coordinate.h>

namespace SFCGAL {

class LineString ;
class Polygon ;
class Solid ;
class PolyhedralSurface ;

/**
 * Represents a bounding box
 * @todo add asText instead of "print"?
 * @todo add basic operations (especialy intersects/intersects3D)
 */
class SFCGAL_API Envelope {
public:
    /**
     * default constructor (empty bounding box)
     */
    Envelope() ;
    /**
     * 2D box constructor with min,max values
     */
    Envelope(
        const double& xmin, const double& xmax,
        const double& ymin, const double& ymax
    ) ;
    /**
     * 3D box constructor with min,max values
     */
    Envelope(
        const double& xmin, const double& xmax,
        const double& ymin, const double& ymax,
        const double& zmin, const double& zmax
    ) ;
    /**
     * default constructor (empty bounding box)
     */
    Envelope( const Coordinate& p ) ;
    /**
     * default constructor (empty bounding box)
     */
    Envelope( const Coordinate& p1, const Coordinate& p2 ) ;
    /**
    * copy constructor
    */
    Envelope( const Envelope& other ) ;
    /**
    * assign operator
    */
    Envelope& operator = ( const Envelope& other ) ;
    /**
     * destructor
     */
    ~Envelope() ;

    /**
     * indicates if the bounding box is empty
     */
    bool isEmpty() const ;
    /**
     * indicates if the bounding box has a 3D component
     */
    bool is3D() const ;

    /**
     * expand the box to include coordinate
     */
    void expandToInclude( const Coordinate& coordinate ) ;


    inline const double& xMin() const {
        return _bounds[0].lower();
    }
    inline const double& yMin() const {
        return _bounds[1].lower();
    }
    inline const double& zMin() const {
        return _bounds[2].lower();
    }

    inline const double& xMax() const {
        return _bounds[0].upper();
    }
    inline const double& yMax() const {
        return _bounds[1].upper();
    }
    inline const double& zMax() const {
        return _bounds[2].upper();
    }

    /**
     * returns the n-th bound
     */
    inline detail::Interval& boundsN( const size_t& n ) {
        BOOST_ASSERT( n < 3 );
        return _bounds[n] ;
    }
    /**
     * returns the n-th bound
     */
    inline const detail::Interval& boundsN( const size_t& n ) const {
        BOOST_ASSERT( n < 3 );
        return _bounds[n] ;
    }


    /**
     * Convenience function. Convert to CGAL::BBox_2
     */
    inline CGAL::Bbox_2 toBbox_2() const {
        BOOST_ASSERT( ! isEmpty() );

        return CGAL::Bbox_2(
                   _bounds[0].lower(), _bounds[1].lower(),
                   _bounds[0].upper(), _bounds[1].upper()
               );
    }

    /**
     * Convenience function. Convert to CGAL::BBox_3
     */
    inline CGAL::Bbox_3 toBbox_3() const {
        if ( is3D() ) {
            return CGAL::Bbox_3(
                       _bounds[0].lower(), _bounds[1].lower(), _bounds[2].lower(),
                       _bounds[0].upper(), _bounds[1].upper(), _bounds[2].upper()
                   );
        }

        return CGAL::Bbox_3(
                   _bounds[0].lower(), _bounds[1].lower(), 0.0,
                   _bounds[0].upper(), _bounds[1].upper(), 0.0
               );
    }



    /**
     * Global binary operator on Envelopes. Test if A's bounding box contains B's
     * FIXME: consider moving that outside of the class
     */
    static bool contains( const Envelope& a, const Envelope& b );

    /**
     * Global binary operator on Envelopes. Test if A's bounding box overlaps B's
     */
    static bool overlaps( const Envelope& a, const Envelope& b );


    //-- helpers

    /**
     * @brief convenience method to convert to 2D Polygon ring
     * @warning empty LineString for empty Envelope, may be X or Y collapsed
     */
    std::unique_ptr< LineString > toRing() const ;
    /**
     * @brief convenience method to convert to 2D Polygon
     * @warning empty Polygon for empty Envelope, may be X or Y collapsed
     */
    std::unique_ptr< Polygon >    toPolygon() const ;

    /**
     * @brief convenience method to convert to 3D Shell
     * @warning empty Solid for empty or non 3D Envelope, may be X, Y or Z collapsed
     */
    std::unique_ptr< PolyhedralSurface >   toShell() const ;
    /**
     * @brief convenience method to convert to 3D Solid
     * @warning empty Solid for empty or non 3D Envelope, may be X, Y or Z collapsed
     */
    std::unique_ptr< Solid >      toSolid() const ;


    /*
     * Display method
     */
    std::ostream& print( std::ostream& ) const;

private:
    /**
     * bounds of the interval ((xmin,xmax),(ymin,ymax),(zmin,zmax))
     */
    detail::Interval _bounds[3] ;
};

/**
 * Global comparison operator on Envelope
 */
SFCGAL_API bool operator==( const Envelope&, const Envelope& );

}//SFCGAL

#endif

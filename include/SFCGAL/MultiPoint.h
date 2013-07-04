/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_MULTIPOINT_H_
#define _SFCGAL_MULTIPOINT_H_

#include <vector>
#include <boost/assert.hpp>

#include <boost/serialization/base_object.hpp>

#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {

/**
 * A MultiPoint in SFA.
 * @ingroup public_api
 */
class SFCGAL_API MultiPoint : public GeometryCollection {
public:
    /**
     * Empty MultiPoint constructor
     */
    MultiPoint() ;
    /**
     * Copy constructor
     */
    MultiPoint( const MultiPoint& other ) ;
    /**
     * assign operator
     */
    MultiPoint& operator = ( MultiPoint other ) ;
    /**
     * destructor
     */
    virtual ~MultiPoint() ;

    //-- SFCGAL::Geometry
    virtual MultiPoint*    clone() const ;

    //-- SFCGAL::Geometry
    virtual std::string    geometryType() const ;
    //-- SFCGAL::Geometry
    virtual GeometryType   geometryTypeId() const ;


    /**
     * returns the n-th Geometry as a Point
     */
    inline Point&          pointN( const size_t& n ) {
        return geometryN( n ).as< Point >() ;
    }
    /**
     * returns the n-th Geometry as a Point
     */
    inline const Point&    pointN( const size_t& n ) const {
        return geometryN( n ).as< Point >() ;
    }




    //-- visitors

    //-- SFCGAL::Geometry
    virtual void accept( GeometryVisitor& visitor ) ;
    //-- SFCGAL::Geometry
    virtual void accept( ConstGeometryVisitor& visitor ) const ;

    /**
     * Serializer
     */
    template <class Archive>
    void serialize( Archive& ar, const unsigned int /*version*/ ) {
        ar& boost::serialization::base_object<GeometryCollection>( *this );

    }
protected:
    //-- SFCGAL::GeometryCollection
    virtual bool           isAllowed( Geometry const& g ) ;
};


}

#endif

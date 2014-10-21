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

#include <vector>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

/**
 * A GeometryCollection in SFA.
 * @ingroup public_api
 */
class SFCGAL_API GeometryCollection : public Geometry {
public:
    typedef boost::ptr_vector< Geometry >::iterator       iterator ;
    typedef boost::ptr_vector< Geometry >::const_iterator const_iterator ;

    /**
     * Empty GeometryCollection constructor
     */
    GeometryCollection() ;
    /**
     * Copy constructor
     */
    GeometryCollection( const GeometryCollection& other ) ;
    /**
     * assign operator
     */
    GeometryCollection& operator = ( GeometryCollection other ) ;
    /**
     * destructor
     */
    virtual ~GeometryCollection() ;

    //-- SFCGAL::Geometry
    virtual GeometryCollection*    clone() const ;

    //-- SFCGAL::Geometry
    virtual std::string    geometryType() const ;
    //-- SFCGAL::Geometry
    virtual GeometryType   geometryTypeId() const ;
    //-- SFCGAL::Geometry
    virtual int            dimension() const ;
    //-- SFCGAL::Geometry
    virtual int            coordinateDimension() const ;
    //-- SFCGAL::Geometry
    virtual bool           isEmpty() const ;
    //-- SFCGAL::Geometry
    virtual bool           is3D() const ;
    //-- SFCGAL::Geometry
    virtual bool           isMeasured() const ;

    //-- SFCGAL::Geometry
    virtual size_t              numGeometries() const ;
    //-- SFCGAL::Geometry
    virtual const Geometry&    geometryN( size_t const& n ) const ;
    //-- SFCGAL::Geometry
    virtual Geometry&           geometryN( size_t const& n ) ;

    /**
     * [SFA/OGC]add a geometry to the collection (takes ownership)
     */
    void                      addGeometry( Geometry* geometry ) ;
    /**
     * [SFA/OGC]add a geometry to the collection (clone instance)
     */
    void                      addGeometry( Geometry const& geometry ) ;

    //-- iterators

    inline iterator       begin() {
        return _geometries.begin() ;
    }
    inline const_iterator begin() const {
        return _geometries.begin() ;
    }

    inline iterator       end() {
        return _geometries.end() ;
    }
    inline const_iterator end() const {
        return _geometries.end() ;
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
        ar& boost::serialization::base_object<Geometry>( *this );
        ar& _geometries;
    }
private:
    boost::ptr_vector< Geometry > _geometries ;

protected:
    /**
     * Test if a geometry in the collection
     */
    virtual bool           isAllowed( Geometry const& g ) ;


    /**
     * Swap
     */
    void  swap( GeometryCollection& other ) {
        _geometries.swap( other._geometries );
    }
};


}

#endif

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

#ifndef _SFCGAL_TRIANGULATED_SURFACE_H_
#define _SFCGAL_TRIANGULATED_SURFACE_H_

#include <vector>
#include <set>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>


#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>


namespace SFCGAL {

/**
 * A TriangulatedSurface in SFA modeled as a Triangle soup
 * @ingroup public_api
 * @todo do better than a "triangle soup" or add topological view?
 */
class SFCGAL_API TriangulatedSurface : public Surface {
public:
    typedef boost::ptr_vector< Triangle >::iterator       iterator ;
    typedef boost::ptr_vector< Triangle >::const_iterator const_iterator ;

    /**
     * Empty TriangulatedSurface constructor
     */
    TriangulatedSurface() ;
    /**
     * Constructor with a vector of triangles
     */
    TriangulatedSurface( const std::vector< Triangle >& triangle ) ;
    /**
     * Copy constructor
     */
    TriangulatedSurface( const TriangulatedSurface& other ) ;
    /**
     * assign operator
     */
    TriangulatedSurface& operator = ( TriangulatedSurface other ) ;
    /**
     * destructor
     */
    ~TriangulatedSurface() ;

    //-- SFCGAL::Geometry
    virtual TriangulatedSurface*    clone() const ;

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

    /**
     * [SFA/OGC]Returns the number of points
     * @deprecated see numGeometries()
     */
    inline size_t             numTriangles() const {
        return _triangles.size();
    }
    /**
     * [SFA/OGC]Returns the n-th point
     * @deprecated see geometryN()
     */
    inline const Triangle&    triangleN( size_t const& n ) const {
        BOOST_ASSERT( n < _triangles.size() );
        return _triangles[n];
    }
    /**
     * [SFA/OGC]Returns the n-th point
     * @deprecated see geometryN()
     */
    inline Triangle&          triangleN( size_t const& n ) {
        BOOST_ASSERT( n < _triangles.size() );
        return _triangles[n];
    }
    /**
    * add a Triangle to the TriangulatedSurface
    */
    inline void               addTriangle( const Triangle& triangle ) {
        addTriangle( triangle.clone() );
    }
    /**
    * add a Triangle to the TriangulatedSurface
    */
    inline void               addTriangle( Triangle* triangle ) {
        _triangles.push_back( triangle );
    }
    /**
     * add triangles from an other TriangulatedSurface
     */
    void                      addTriangles( const TriangulatedSurface& other ) ;


    //-- SFCGAL::Geometry
    virtual size_t               numGeometries() const ;
    //-- SFCGAL::Geometry
    virtual const Triangle&     geometryN( size_t const& n ) const ;
    //-- SFCGAL::Geometry
    virtual Triangle&            geometryN( size_t const& n ) ;


    //-- optimization

    void reserve( const size_t& n ) ;

    //-- iterators

    inline iterator       begin() {
        return _triangles.begin() ;
    }
    inline const_iterator begin() const {
        return _triangles.begin() ;
    }

    inline iterator       end() {
        return _triangles.end() ;
    }
    inline const_iterator end() const {
        return _triangles.end() ;
    }

    //-- visitors

    //-- SFCGAL::Geometry
    virtual void accept( GeometryVisitor& visitor ) ;
    //-- SFCGAL::Geometry
    virtual void accept( ConstGeometryVisitor& visitor ) const ;


    //-- helpers

    /**
     * @brief Converts a TriangulatedSurface to a CGAL::Polyhedron_3
     */
    template < typename K, typename Polyhedron >
    std::auto_ptr<Polyhedron> toPolyhedron_3() const;

    /**
     * Serializer
     */
    template <class Archive>
    void serialize( Archive& ar, const unsigned int /*version*/ ) {
        ar& boost::serialization::base_object<Geometry>( *this );
        ar& _triangles;
    }
private:
    boost::ptr_vector< Triangle > _triangles ;

    void swap( TriangulatedSurface& other ) {
        std::swap( _triangles, other._triangles );
    }
};
}

#endif

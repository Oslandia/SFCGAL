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
#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <vector>

#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

#include <SFCGAL/Point.h>

#include <CGAL/Polygon_2.h>

namespace SFCGAL {

/**
 * A LineString in SFA
 * @ingroup public_api
 */
class SFCGAL_API LineString : public Geometry {
public:
    typedef boost::ptr_vector< Point >::iterator       iterator ;
    typedef boost::ptr_vector< Point >::const_iterator const_iterator ;

    /**
     * Empty LineString constructor
     */
    LineString() ;
    /**
     * Constructor with a point vector
     */
    LineString( const std::vector< Point > & points ) ;
    /**
     * LineString constructor
     */
    LineString( const Point& startPoint, const Point& endPoint ) ;
    /**
     * Copy constructor
     */
    LineString( LineString const& other ) ;

    /**
     * assign operator
     */
    LineString& operator = ( LineString other ) ;

    /**
     * destructor
     */
    ~LineString() ;

    //-- SFCGAL::Geometry
    virtual LineString*    clone() const ;

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
     * remove all points from the LineString
     */
    void clear() ;

    /**
     * reverse LineString orientation
     */
    void reverse() ;

    /**
     * [SFA/OGC]Returns the number of points
     */
    inline size_t          numPoints() const {
        return _points.size();
    }
    /**
     * Returns the number of segments
     * @warning not standard, returns zero if LineString contains only one point
     */
    size_t                  numSegments() const ;


    /**
     * [SFA/OGC]Returns the n-th point
     */
    inline const Point&    pointN( size_t const& n ) const {
        BOOST_ASSERT( n < numPoints() ) ;
        return _points[n];
    }
    /**
     * [SFA/OGC]Returns the n-th point
     */
    inline Point&          pointN( size_t const& n ) {
        BOOST_ASSERT( n < numPoints() ) ;
        return _points[n];
    }


    /**
     * [SFA/OGC]Returns the first point
     */
    inline const Point&    startPoint() const {
        return _points.front();
    }
    /**
     * [SFA/OGC]Returns the first point
     */
    inline Point&          startPoint() {
        return _points.front();
    }


    /**
     * [SFA/OGC]Returns the first point
     */
    inline const Point&    endPoint() const {
        return _points.back();
    }
    /**
     * [SFA/OGC]Returns the first point
     */
    inline Point&          endPoint() {
        return _points.back();
    }


    /**
     * append a Point to the LineString
     */
    inline void            addPoint( const Point& p ) {
        _points.push_back( p.clone() ) ;
    }
    /**
     * append a Point to the LineString and takes ownership
     */
    inline void            addPoint( Point* p ) {
        _points.push_back( p ) ;
    }


    //-- methods

    /**
     * test if the LineString is closed
     */
    bool isClosed() const ;


    //-- iterators

    inline iterator       begin() {
        return _points.begin() ;
    }
    inline const_iterator begin() const {
        return _points.begin() ;
    }

    inline iterator       end() {
        return _points.end() ;
    }
    inline const_iterator end() const {
        return _points.end() ;
    }

    //-- optimization

    void reserve( const size_t& n ) ;


    /**
     * Const iterator to 2D points
     * TODO: replace by boost::tranform_iterator ?
     */
    class Point_2_const_iterator :
        public boost::iterator_facade<
        Point_2_const_iterator,
        Kernel::Point_2 const,
            boost::bidirectional_traversal_tag > {
    public:
        Point_2_const_iterator() {}
        explicit Point_2_const_iterator( const_iterator it ) : it_( it ) {}
        //Point_2_const_iterator( const Point_2_const_iterator<K>& other ) : it_(other.it_) {}
    private:
        friend class boost::iterator_core_access;
        void increment() {
            it_++;
        }
        void decrement() {
            it_--;
        }
        bool equal( const Point_2_const_iterator& other ) const {
            return this->it_ == other.it_;
        }
        const Kernel::Point_2& dereference() const {
            p_ = it_->toPoint_2();
            return p_;
        }
        mutable Kernel::Point_2 p_;
        const_iterator it_;
    };
    Point_2_const_iterator points_2_begin() const {
        return Point_2_const_iterator( begin() );
    }
    Point_2_const_iterator points_2_end() const {
        return Point_2_const_iterator( end() );
    }
    std::pair< Point_2_const_iterator, Point_2_const_iterator > points_2() const {
        return std::make_pair( points_2_begin(), points_2_end() );
    }


    /**
     * Const iterator to 3D points
     * TODO: replace by boost::tranform_iterator ?
     */
    class Point_3_const_iterator :
        public boost::iterator_facade<
        Point_3_const_iterator,
        Kernel::Point_3 const,
            boost::bidirectional_traversal_tag > {
    public:
        Point_3_const_iterator() {}
        explicit Point_3_const_iterator( const_iterator it ) : it_( it ) {}
        Point_3_const_iterator( const Point_3_const_iterator& other ) : it_( other.it_ ) {}
    private:
        friend class boost::iterator_core_access;
        void increment() {
            it_++;
        }
        void decrement() {
            it_--;
        }
        bool equal( const Point_3_const_iterator& other ) const {
            return this->it_ == other.it_;
        }
        const Kernel::Point_3& dereference() const {
            p_ = it_->toPoint_3();
            return p_;
        }
        mutable Kernel::Point_3 p_;
        const_iterator it_;
    };

    Point_3_const_iterator points_3_begin() const {
        return Point_3_const_iterator( begin() );
    }
    Point_3_const_iterator points_3_end() const {
        return Point_3_const_iterator( end() );
    }
    std::pair< Point_3_const_iterator, Point_3_const_iterator > points_3() const {
        return std::make_pair( points_3_begin(), points_3_end() );
    }

    /*
     * @brief Convert to CGAL::Polygon_2
     * @param forceCounterClocksize force exterior ring orientation (counter clocksize)
     */
    CGAL::Polygon_2< Kernel > toPolygon_2( bool fixOrientation = true ) const;

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
        ar& _points;
    }
private:
    boost::ptr_vector< Point > _points ;

    void swap( LineString& other ) {
        std::swap( _points, other._points );
    }
};


}



#endif

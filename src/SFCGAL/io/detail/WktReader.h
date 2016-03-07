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

#ifndef _SFCGAL_IO_DETAIL_WKTREADER_H_
#define _SFCGAL_IO_DETAIL_WKTREADER_H_

#include <sstream>
#include <limits>

#include <boost/format.hpp>

#include <SFCGAL/config.h>
#include <SFCGAL/numeric.h>
#include <SFCGAL/types.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/io/detail/InputStreamReader.h>

namespace SFCGAL {
namespace io {
namespace detail {
    
    
    /**
     * 
     * Reader for WKT geometries
     * 
     * @warning POLYHEDRALSURFACE are converted to MULTIPOLYGON, some Geometry
     * types are extensions of the standard.
     */
    template < typename K >
    class WktReader {
    public:
        
        WktReader(std::istream &s):
            _reader(s)
        {
            
        }
        
        /**
         * Read a geometry of unknown type
         * TODO unique_ptr
         */
        Geometry<K>* read(){
            // POINT
            {
                Point<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // LINESTRING
            {
                LineString<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // POLYGON
            {
                Polygon<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            
            // MULTIPOINT
            {
                MultiPoint<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // MULTILINESTRING
            {
                MultiLineString<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // MULTIPOLYGON (and POLYHEDRALSURFACE)
            {
                MultiPolygon<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // POLYHEDRALSURFACE
            {
                PolyhedralSurface<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            // TRIANGLE
            {
                Triangle<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }

            // TIN
            {
                TriangulatedSurface<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            
            // SOLID
            {
                Solid<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            
            // MULTISOLID
            {
                MultiSolid<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
            
            // GEOMETRYCOLLECTION
            {
                GeometryCollection<K> candidate ;
                if ( read(candidate) ){
                    return candidate.clone();
                }
            }
                        
            // Unexpected type
            BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
        }
        
        /**
         * Read SRID
         */
        srid_t readSRID()
        {
            srid_t srid = 0;
            if ( _reader.imatch( "SRID=" ) ) {
                _reader.read( srid );
                if ( !_reader.match( ";" ) ) {
                    BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
                }
            }
            return srid;
        }

        
        // -- POINT ------------------------------------------------------------
        
        /**
         * Read a WKT point
         */
        bool read( Point<K> & g ){
            if ( ! _reader.imatch( "POINT" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = Point<K>();
                return true ;
            }
            readInner(g);
            return true;
        }
        
        /**
         * Read inner Point
         */
        void readInner(Point<K> & g){
            if ( ! _reader.match('(') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
            readCoordinate(g);
            if ( ! _reader.match(')') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }

        // -- LINESTRING--------------------------------------------------------
        
        /**
         * Read a WKT LineString
         */
        bool read(LineString<K>& g){
            if ( ! _reader.imatch( "LINESTRING" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = LineString<K>();
                return true ;
            }
            readInner(g);
            return true;
        }
    
        /**
         * Read inner LineString
         */ 
        void readInner(LineString<K>& g){
            if ( ! _reader.match('(') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
            while ( ! _reader.eof() ){
                if ( ! _reader.match("EMPTY") ){
                    Point<K> p;
                    readCoordinate(p);
                    g.push_back(p);
                }
                if ( _reader.match(',') ){
                    continue;
                }
                break;
            }
            if ( ! _reader.match(')') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        // -- TRIANGLE  --------------------------------------------------------
        
        /**
         * Read a WKT Triangle
         */
        bool read(Triangle<K>& g){
            if ( ! _reader.imatch( "TRIANGLE" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = Triangle<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        /**
         * Read inner Triangle
         */
        void readInner(Triangle<K>& g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            // 4 points to read
            std::vector< Point<K> > points ;

            while ( ! _reader.eof() ) {
                points.push_back( Point<K>() ) ;
                readCoordinate( points.back() );

                if ( ! _reader.match( "," ) ) {
                    break;
                }
            }

            if ( points.size() != 4 ) {
                BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, expected 4 points for triangle" ) );
            }

            if ( points.back() != points.front() ) {
                BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, first point different of the last point for triangle" ) );
            }

            g = Triangle<K>( points[0], points[1], points[2] );

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        

        // -- POLYGON   --------------------------------------------------------
        
        /**
         * Read a WKT Polygon
         */
        bool read(Polygon<K>& g){
            if ( ! _reader.imatch( "POLYGON" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = Polygon<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
    
        /**
         * Read inner Polygon
         */ 
        void readInner(Polygon<K>& g){
            if ( ! _reader.match('(') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
            while ( ! _reader.eof() ){
                if ( ! _reader.match("EMPTY") ){
                    LineString<K> ring;
                    readInner(ring);
                    g.emplace_back(ring);
                }
                if ( _reader.match(',') ){
                    continue;
                }
                break;
            }
            if ( ! _reader.match(')') ){
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        // -- MULTIPOINT -------------------------------------------------------
        
        /**
         * Read a WKT MULTIPOINT
         */
        bool read( MultiPoint<K> & g ){
            if ( ! _reader.imatch( "MULTIPOINT" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = MultiPoint<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        /**
         * Read inner MultiPoint
         */
        void readInner(MultiPoint<K> & g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {

                // optional open/close parenthesis
                bool parenthesisOpen = false ;

                if ( _reader.match( '(' ) ) {
                    parenthesisOpen = true ;
                }
                if ( ! _reader.match("EMPTY") ){
                    Point<K> p ;
                    readCoordinate( p );
                    g.push_back(p);
                }
                if ( parenthesisOpen && ! _reader.match( ')' ) ) {
                    BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
                }

                //break if not followed by another point
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        // -- MULTILINESTRING --------------------------------------------------
        
        /**
         * Read a WKT MULTILINESTRING
         */
        bool read( MultiLineString<K> & g ){
            if ( ! _reader.imatch( "MULTILINESTRING" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = MultiLineString<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read Inner MultiLineString
         */
        void readInner(MultiLineString<K> & g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    LineString<K> lineString ;
                    readInner( lineString );
                    if ( ! lineString.empty() ){
                        g.push_back(lineString);
                    }
                }
                //break if not followed by another linestring
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }
            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        //--- MULTIPOLYGON -----------------------------------------------------
        
        /**
         * Read a WKT MULTIPOLYGON and POLYHEDRALSURFACE
         */
        bool read( MultiPolygon<K> & g ){
            if ( ! _reader.imatch( "MULTIPOLYGON" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = MultiPolygon<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read inner multipolygon
         */
        void readInner(MultiPolygon<K> & g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    Polygon<K> polygon;
                    readInner( polygon );
                    if ( ! polygon.empty() ){
                        g.push_back(polygon);
                    }
                }
                //break if not followed by another polygon
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        //--- POLYHEDRALSURFACE ------------------------------------------------
        
        /**
         * Read a WKT POLYHEDRALSURFACE
         */
        bool read( PolyhedralSurface<K> & g ){
            if ( ! _reader.imatch("POLYHEDRALSURFACE") ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = PolyhedralSurface<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read inner multipolygon
         */
        void readInner(PolyhedralSurface<K> & g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    Polygon<K> polygon;
                    readInner( polygon );
                    if ( ! polygon.empty() ){
                        g.push_back(polygon);
                    }
                }
                //break if not followed by another polygon
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }

        
        //--- TRIANGULATEDSURFACE-----------------------------------------------
        
        /**
         * Read a WKT TRIANGULATEDSURFACE
         */
        bool read( TriangulatedSurface<K> & g ){
            if ( ! _reader.imatch( "TIN" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = TriangulatedSurface<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read inner TriangulatedSurface
         */
        void readInner( TriangulatedSurface<K> & g ){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    Triangle<K> triangle ;
                    readInner( triangle );
                    g.push_back( triangle ) ;
                }
                //break if not Oui tout à fait, le Z n'est pas pris en compte dans ce cas là.followed by another points
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        
        
        //--- SOLID              -----------------------------------------------
        
        /**
         * Read a WKT SOLID
         */
        bool read( Solid<K> & g ){
            if ( ! _reader.imatch( "SOLID" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = Solid<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read inner TriangulatedSurface
         */
        void readInner( Solid<K> & g ){
            //solid begin
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while ( ! _reader.eof() ){
                if ( ! _reader.match("EMPTY") ){
                    PolyhedralSurface<K> shell ;
                    readInner( shell );
                    g.push_back(shell);
                }
                //break if not followed by another points
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            //solid end
            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        
        //--- MULTISOLID         -----------------------------------------------
        
        /**
         * Read a WKT MULTISOLID
         */
        bool read( MultiSolid<K> & g ){
            if ( ! _reader.imatch( "MULTISOLID" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = MultiSolid<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read inner MultiSolid
         */
        void readInner( MultiSolid<K> & g ){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    Solid<K> solid ;
                    readInner( solid );
                    g.push_back(solid);
                }
                //break if not followed by another points
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
        
        
        //--- GEOMETRYCOLLECTION -----------------------------------------------
        
        /**
         * Read a WKT GEOMETRYCOLLECTION
         */
        bool read( GeometryCollection<K> & g ){
            if ( ! _reader.imatch( "GEOMETRYCOLLECTION" ) ){
                return false;
            }
            _is3D       = _reader.imatch( "Z" );
            _isMeasured = _reader.imatch( "M" );
            if ( _reader.match("EMPTY") ){
                g = GeometryCollection<K>();
                return true ;
            }
            readInner(g);
            return true ;
        }
        
        /**
         * Read geometries in a GeometryCollection
         */
        void readInner(GeometryCollection<K> & g){
            if ( ! _reader.match( '(' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }

            while( ! _reader.eof() ) {
                if ( ! _reader.match("EMPTY") ){
                    // read geometry
                    g.push_back( read() );
                }
                //break if not followed by another geometry
                if ( ! _reader.match( ',' ) ) {
                    break ;
                }
            }

            if ( ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }
        
    protected:
        InputStreamReader _reader ;
        bool _is3D ;
        bool _isMeasured ;
        
        /**
         * Read a point coordinate
         */
        bool readCoordinate( Point<K> & g ){
            typename K::FT x = 0 ,y = 0 ,z = 0 ;
            double m = SFCGAL::NaN() ;
            readXYZM(x,y,z,m);
            
            if ( _is3D ){
                g = Point<K>(x,y,z);
            }else{
                g = Point<K>(x,y);
            }
            // NaN if not measured (no need to test _isMeasured)
            g.m() = m ;
        }

        /**
         * provides a generic message
         */
        std::string parseErrorMessage()
        {
            std::ostringstream oss;
            oss << "WKT parse error (" << _reader.context() << ")" ;
            return oss.str();
        }
    
    private:    
        /**
         * Helper that read x,y,z,m according to is3D and isMeasured
         */
        template < typename FT >
        void readXYZM( FT & x, FT & y, FT & z, double & m){
            // EMPTY in coordinate sequence
            if ( _reader.match("EMPTY") ){
                BOOST_THROW_EXCEPTION( WktParseException(
                    ( boost::format( "EMPTY is not allowed in coordinate sequence (%s)" ) % _reader.context() ).str()
                ) );
            }

            if ( ! ( _reader.read(x) && _reader.read(y) ) ){
                BOOST_THROW_EXCEPTION( WktParseException(
                    ( boost::format( "WKT parse error, Coordinate dimension < 2 (%s)" ) % _reader.context() ).str()
                ) );
            }
            if( _is3D && _isMeasured ){
                if ( ! _reader.read(z) ){
                    BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, missing Z coordinate" ) );
                }
                if ( ! _reader.read(m) ){
                    BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, missing M coordinate" ) );
                }
            }else if ( _isMeasured ){
                // M coordinate
                if ( ! _reader.read(m) ){
                    BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, missing M coordinate" ) );
                }
            }else{
                //optional Z
                if ( _reader.read(z) ){
                    // force metadata
                    _is3D = true ;
                }
            }
            
            FT dummy;
            if ( _reader.read(dummy) ){
                BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, unexpected extra coordinate" ) );
            }
        }
    } ;
    

}//detail
}//io
}//SFCGAL

#endif

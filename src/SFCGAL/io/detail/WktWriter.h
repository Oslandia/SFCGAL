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

#ifndef _SFCGAL_IO_DETAIL_WKTWRITER_H_
#define _SFCGAL_IO_DETAIL_WKTWRITER_H_

#include <sstream>
#include <SFCGAL/config.h>
#include <SFCGAL/Geometry.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/io/detail/InputStreamReader.h>

namespace SFCGAL {
namespace io {
namespace detail {
    
    /**
     * Write WKT to output stream
     * @warning there are no empty point or triangle in SFCGAL
     */
    template < typename K >
    class WktWriter : public boost::static_visitor<> {
    public:
        WktWriter( std::ostream & s ):
            _s(s)
        {
            
        }
        
        void operator () ( const Point<K> & g ){
            _s << "POINT" ;
            if ( g.isEmpty() ){
                _s << " EMPTY";
                return;
            }
            writeInner( g );
        }

        void operator () ( const LineString<K> & g ){
            _s << "LINESTRING" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            writeInner(g);
        }

        void operator () ( const Polygon<K> & g ){
            _s << "POLYGON" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            writeInner(g);
        }
        
        void operator () ( const MultiPoint<K> & g ){
            _s << "MULTIPOINT" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            _s << "(";
            writeInner( g );
            _s << ")" ;
        }

        void operator () ( const MultiLineString<K> & g ){
            _s << "MULTILINESTRING" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            _s << "(";
            writeInner( g );
            _s << ")" ;
        }

        void operator () ( const MultiPolygon<K> & g ){
            _s << "MULTIPOLYGON" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            _s << "(";
            writeInner( g );
            _s << ")" ;
        }
        
        void operator () ( const GeometryCollection<K> & g ){
            _s << "GEOMETRYCOLLECTION" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            _s << "(";
            for ( int i = 0; i < g.size(); i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                boost::apply_visitor(*this,g.geometry(i));
            }
            _s << ")" ;
        }
        
        void operator () ( const Triangle<K> & g ){
            _s << "TRIANGLE" ;
            if ( g.isEmpty() ){
                _s << " EMPTY";
                return;
            }
            writeInner(g);
        }
        
        void operator () ( const TriangulatedSurface<K> & g ){
            _s << "TIN" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            writeInner(g);
        }
        
        
        void operator () ( const Solid<K> & g ){
            _s << "SOLID" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            
        }
        
        void operator () ( const MultiSolid<K> & g ){
            _s << "MULTISOLID" ;
            if ( g.empty() ){
                _s << " EMPTY";
                return;
            }
            writeInner(g);
        }
        
        
        /*
        template < typename T >
        void operator () ( const T & g ){
            BOOST_THROW_EXCEPTION(NotImplementedException("not supported geometry type")); 
        }
        */

    private:
        std::ostream & _s ;
        
        void writeCoordinate( const Point<K> & g ){
            if ( g.is3D() ){
                _s << g.toPoint_3() ;
            }else{
                _s << g.toPoint_2() ;
            }
        }
        
        void writeInner( const Point<K> & g ){
            _s << "(";
            writeCoordinate(g);
            _s << ")";
        }
        
        void writeInner( const LineString<K> & g ){
            _s << "(";
            for ( int i = 0; i < g.size(); i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                writeCoordinate( g[i] ) ;
            }
            _s << ")" ;
        }
        
        void writeInner( const Polygon<K> & g ){
            _s << "(";
            for ( int i = 0; i < g.size(); i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                writeInner(g[i]) ;
            }
            _s << ")" ;
        }
        
        void writeInner( const Triangle<K> & g ){
            _s << "(";
            for ( int i = 0; i < 4; i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                writeInner( g.vertex(i) );
            }
            _s << ")" ;
        }
        
        void writeInner( const Solid<K> & g ){
            _s << "(";
            for ( int i = 0; i < g.size(); i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                writeInner( g[i] );
            }
            _s << ")" ;
        }

        
        template < typename GT>
        void writeInner( const Collection<GT> & g ){
            _s << "(";
            for ( int i = 0; i < g.size(); i++ ){
                if ( i != 0 ){
                    _s << ",";
                }
                writeInner(g.geometry(i)) ;
            }
            _s << ")" ;
        }
        
    } ;

}//detail
}//io
}//SFCGAL


#endif //_SFCGAL_IO_DETAIL_WKTWRITER_H_



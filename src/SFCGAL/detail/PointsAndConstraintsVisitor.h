#ifndef _SFCGAL_DETAIL_POINTSANDCONSTRAINTSVISITOR_H_
#define _SFCGAL_DETAIL_POINTSANDCONSTRAINTSVISITOR_H_

#include <vector>
#include <SFCGAL/Exception.h>

namespace SFCGAL {
namespace detail {
    
    /**
     * Visitor that extract vertices and edges from a Geometry
     */
    template < typename K >
    class PointsAndConstraintsVisitor : public boost::static_visitor<> {
    public:
        PointsAndConstraintsVisitor(
            std::vector< Point<K> > & points,
            std::vector< std::pair< size_t, size_t > > & constraints
        ):
            _points(points),
            _constraints(constraints)
        {
            
        }
        
        void operator () ( const Point<K> & g ){
            _points.push_back(g) ;
        }
        
        void operator () ( const LineString<K> & g ){
            size_t last = 0 ;
            for ( int i = 0; i < g.size(); i++ ){
                size_t index = _points.size() ;
                _points.push_back(g[i]);
                if ( i != 0 ){
                    _constraints.push_back(std::make_pair(last,index));
                }
                last = index ;
            }
        }
        
        void operator () ( const Triangle<K> & g ){
            size_t last = 0 ;
            for ( int i = 0; i < 3; i++ ){
                size_t index = _points.size() ;
                _points.push_back(g.vertex(i));
                if ( i != 0 ){
                    _constraints.push_back(std::make_pair(last,index));
                }
                last = index ;
            }
        }
        
        void operator () ( const Polygon<K> & g ){
            for ( const LineString<K> & lineString : g ){
                (*this)(g);
            }
        }
        
        template < typename GT >
        void operator () ( const Collection<GT> & geometries ){
            for ( const GT & geometry : geometries ){
                (*this)(geometry);
            }
        }
        
        void operator () ( const Geometry<K> & geometry ){
            boost::apply_visitor((*this),geometry);
        }

    private:
        std::vector< Point<K> > & _points ;
        std::vector< std::pair< size_t, size_t > > & _constraints ;
    } ;
    
} // namespace detail
} // namespace SFCGAL

#endif

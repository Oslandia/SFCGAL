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
#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>
#include <SFCGAL/graph/algorithm/orientation.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
ConsistentOrientationBuilder::ConsistentOrientationBuilder():
	_graph(),
	_graphBuilder( _graph ),
	_triangles()
{

}

///
///
///
void ConsistentOrientationBuilder::addTriangle( const Triangle & triangle )
{
	_triangles.push_back( _graphBuilder.addTriangle( triangle, graph::Edge( _triangles.size() ) ) ) ;
}

///
///
///
void ConsistentOrientationBuilder::addTriangulatedSurface( const TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < triangulatedSurface.numGeometries(); i++ ){
		addTriangle( triangulatedSurface.geometryN( i ) ) ;
	}
}

///
///
///
TriangulatedSurface ConsistentOrientationBuilder::buildTriangulatedSurface()
{
	_makeOrientationConsistent() ;
	TriangulatedSurface triangulatedSurface ;
	for ( size_t i = 0; i < numTriangles(); i++ ){
		triangulatedSurface.addTriangle( triangleN(i) );
	}
	return triangulatedSurface ;
}

///
///
///
Triangle  ConsistentOrientationBuilder::triangleN( const size_t & n ) const
{
	const edge_descriptor & ab = _triangles[n][0] ;
	const edge_descriptor & bc = _triangles[n][1] ;
	const edge_descriptor & ca = _triangles[n][2] ;

	return Triangle(
		Point( _graph[ _graph.source(ab) ].coordinate ),
		Point( _graph[ _graph.source(bc) ].coordinate ),
		Point( _graph[ _graph.source(ca) ].coordinate )
	);
}


///
///
///
void ConsistentOrientationBuilder::_makeOrientationConsistent()
{
	if ( _triangles.empty() )
		return ;

	/*
	 * mark all triangles as not oriented and not visited
	 */
	_visited.resize( numTriangles() ) ;
	_oriented.resize( numTriangles() ) ;

	for ( size_t i = 0; i < numTriangles(); i++ ){
		_visited[i]  = false ;
		_oriented[i] = false ;
	}

	_computeNeighbors();

	// mark first one as oriented (reference)
	int currentTriangle = -1 ;
	while ( ( currentTriangle = _findNextTriangle() ) != -1 ){
		//mark triangle as visited
		_visited[ currentTriangle ] = true ;

		//orient neighbors
		const std::set< size_t > & neighbors = _neighbors[ currentTriangle ] ;
		for ( std::set< size_t >::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it ){
			bool hasOppositeEdge, hasParallelEdge ;
			graph::algorithm::studyOrientation(
				_graph,
				_triangles[ currentTriangle ],
				_triangles[ (*it) ],
				hasOppositeEdge,
				hasParallelEdge
			);

			// orientation is consistent
			if ( ! hasParallelEdge ){
				_oriented[ *it ] = true ;
				continue ;
			}

			// orientation can't be consistent
			if ( hasOppositeEdge && hasParallelEdge ){
				BOOST_THROW_EXCEPTION(Exception(
					"can't build consistent orientation from triangle set"
				));
			}

			// orientation has already been fixed (moebius)
			if ( hasParallelEdge && _oriented[ *it ] ){
				BOOST_THROW_EXCEPTION(Exception(
					"can't build consistent orientation from triangle set, inconsistent orientation for triangle"
				));
			}

			//here, neighbor triangle should be reversed
			_graph.reverse( _triangles[ (*it) ] );
			_oriented[ *it ] = true ;
		}
	}
}

///
///
///
void ConsistentOrientationBuilder::_computeNeighbors()
{
	_neighbors.clear() ;
	_neighbors.resize( numTriangles() );

	for ( size_t i = 0; i < _triangles.size(); i++ ){
		const std::vector< edge_descriptor > & triangle = _triangles[i] ;
		for ( size_t j = 0; j < triangle.size(); j++ ){
			vertex_descriptor source = _graph.source( triangle[j] ) ;
			vertex_descriptor target = _graph.target( triangle[j] ) ;

			//get neighbor edges
			std::vector< directed_edge_descriptor > neighborEdges = _graph.edges( source, target );
			//use marker to fill neighborGraph
			for ( size_t k = 0; k < neighborEdges.size(); k++ ){
				size_t idOtherTriangle = (size_t)_graph[ neighborEdges[k].first ].face;
				if ( idOtherTriangle == i )
					continue ;

				_neighbors[i].insert( idOtherTriangle );
			}
		}
	}
}

///
///
///
int ConsistentOrientationBuilder::_findNextTriangle()
{
	int result = -1 ;

	/*
	 * find an oriented triangle (reached) and not visited
	 */
	for ( size_t i = 0; i < numTriangles(); i++ ){
		if ( ! _oriented[i] || _visited[i] )
			continue ;
		result = i ;
		break ;
	}

	//triangle found
	if ( result != -1 )
		return result ;

	/*
	 * here, a new connected part begins
	 */
	for ( size_t i = 0; i < numTriangles(); i++ ){
		if ( ! _oriented[i] ){
			_oriented[i] = true ;
			return i ;
		}
	}

	BOOST_ASSERT( result == -1 );
	return result ;
}




}//algorithm
}//SFCGAL


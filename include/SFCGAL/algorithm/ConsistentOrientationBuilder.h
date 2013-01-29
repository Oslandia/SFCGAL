/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#ifndef _SFCGAL_ALGORITHM_CONSISTENTORIENTATIONBUILDER_H_
#define _SFCGAL_ALGORITHM_CONSISTENTORIENTATIONBUILDER_H_

#include <SFCGAL/Geometry.h>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Make orientation consistent in a triangle set
	 */
	class ConsistentOrientationBuilder {
	public:
		typedef graph::GeometryGraph::vertex_descriptor        vertex_descriptor ;
		typedef graph::GeometryGraph::edge_descriptor          edge_descriptor ;
		typedef graph::GeometryGraph::directed_edge_descriptor directed_edge_descriptor ;

		/**
		 * default constructor
		 */
		ConsistentOrientationBuilder() ;

		/**
		 * add a Triangle
		 */
		void addTriangle( const Triangle & triangle ) ;
		/**
		 * add a TriangulatedSurface
		 */
		void addTriangulatedSurface( const TriangulatedSurface & triangulatedSurface ) ;

		/**
		 * get the resulting TriangulatedSurface where each connected part
		 * has consistent orientation.
		 *
		 * @throw SFCGAL::Exception if such a TriangulatedSurface can't be built
		 */
		TriangulatedSurface buildTriangulatedSurface() ;

		/**
		 * returns the number of triangles
		 */
		inline size_t  numTriangles() const { return _triangles.size(); }
		/**
		 * returns the n-th triangle
		 */
		Triangle       triangleN( const size_t & n ) const ;


		/**
		 * [advanced]use after buildTriangulatedSurface
		 */
		const std::set< size_t > & neighbors( const size_t & n ) const ;
	private:
		graph::GeometryGraph                          _graph ;
		graph::GeometryGraphBuilder                   _graphBuilder ;
		std::vector< std::vector< edge_descriptor > > _triangles ;

		std::vector< bool >                           _visited ;
		std::vector< bool >                           _oriented ;
		std::vector< std::set< size_t > >             _neighbors ;


		/**
		 * make triangle orientation consistent
		 */
		void _makeOrientationConsistent() ;

		/**
		 * compute neighbors for each triangles
		 */
		void _computeNeighbors() ;

		/**
		 * find the next triangle to visit (may select a new reference triangle)
		 */
		int _findNextTriangle() ;

	};


}//algorithm
}//SFCGAL


#endif

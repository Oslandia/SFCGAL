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
#ifndef _SFCGAL_TRIANGULATE_CONSTRAINTDELAUNAYTRIANGULATION_H_
#define _SFCGAL_TRIANGULATE_CONSTRAINTDELAUNAYTRIANGULATION_H_

#include <SFCGAL/Coordinate.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

namespace SFCGAL {
	class TriangulatedSurface ;
}


namespace SFCGAL {
namespace triangulate {

	/**
	 * @brief 2DZ constraint delaunay triangulation
	 */
	class ConstraintDelaunayTriangulation {
	public:
		/**
		 * @brief vertex info in triangulation
		 */
		struct VertexInfo {
			VertexInfo() :
				original()
			{
			}
			Coordinate original ;
		};

		/**
		 * face information (depth)
		 */
		struct FaceInfo {
			FaceInfo():
				nestingLevel(-1)
			{
			}
			int nestingLevel;

			bool in_domain() {
				return nestingLevel % 2 == 1;
			}
		};

		typedef CGAL::Triangulation_vertex_base_with_info_2< VertexInfo, Kernel >            Triangulation_vertex_base ;
		typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, Kernel>                  Triangulation_face_base;
		typedef CGAL::Constrained_triangulation_face_base_2<Kernel, Triangulation_face_base>  Constrained_triangulation_face_base;
		typedef CGAL::Triangulation_data_structure_2< Triangulation_vertex_base,
				Constrained_triangulation_face_base >                                          Triangulation_data_structure ;

		typedef CGAL::Constrained_Delaunay_triangulation_2< Kernel, Triangulation_data_structure, CGAL::Exact_predicates_tag >  CDT;

		typedef CDT::Vertex_handle           Vertex_handle ;
		typedef CDT::Face_handle             Face_handle ;
		typedef CDT::All_faces_iterator      All_faces_iterator ;
		typedef CDT::Finite_faces_iterator   Finite_faces_iterator ;

		/**
		 * @brief default constructor
		 */
		ConstraintDelaunayTriangulation();

		/**
		 * @brief add a vertex to the triangulation
		 */
		Vertex_handle addVertex( const Coordinate & position ) ;
		/**
		 * @brief add a vertex to the triangulation
		 */
		void  addConstraint( Vertex_handle source, Vertex_handle target ) ;


		/**
		 * @brief clear the triangulation
		 */
		void clear() ;

		/**
		 * @brief Returns the number of finite vertices.
		 */
		size_t	numVertices() const ;
		/**
		 * @brief Returns the number of finite faces.
		 */
		size_t	numTriangles() const ;

		/**
		 * get the resulting triangulated surface
		 */
		std::auto_ptr< TriangulatedSurface > getTriangulatedSurface() const ;

		/**
		 * @brief get finite face iterator
		 */
		inline Finite_faces_iterator finite_faces_begin() const {
			return _cdt.finite_faces_begin() ;
		}
		/**
		 * @brief get finite face iterator
		 */
		inline Finite_faces_iterator finite_faces_end() const {
			return _cdt.finite_faces_end() ;
		}

		/**
		 * @brief get all face iterator
		 */
		inline All_faces_iterator all_faces_begin() const {
			return _cdt.all_faces_begin() ;
		}
		/**
		 * @brief get all face iterator
		 */
		inline All_faces_iterator all_faces_end() const {
			return _cdt.all_faces_end() ;
		}


		/**
		 * @brief fill nesting_level info in face info
		 */
		void markDomains() ;

	private:
		/**
		 * @brief wrapped triangulation
		 */
		CDT _cdt ;
		/**
		 * @brief plan in which the triangulation is done
		 * @todo take in account
		 */
		Kernel::Plane_3 _plane ;


		/**
		 * @brief helper method to mark domains
		 */
		void _markDomains( CDT::Face_handle start, int index, std::list< CDT::Edge >& border ) ;
	};

} // namespace triangulate
} // namespace SFCGAL

#endif

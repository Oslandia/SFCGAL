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
#ifndef _SFCGAL_ALGORITHM_AREA_H_
#define _SFCGAL_ALGORITHM_AREA_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Kernel.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief Compute the 2D area for a Geometry
	 * @ingroup public_api
	 * @warning Z component is ignored, there is no 2D projection for 3D geometries
	 */
	SFCGAL_API double     area( const Geometry & g ) ;

	/**
	 * @brief Compute the 2D signed area for a Triangle
	 */
	SFCGAL_API Kernel::FT signedArea( const Triangle & g );
	/**
	 * @brief Compute the 2D signed area for a closed LineString
	 */
	SFCGAL_API Kernel::FT signedArea( const LineString & g );

	/**
	 * Returns Compute the 2D area for a Triangle
	 */
	SFCGAL_API double    area( const Triangle & g ) ;
	/**
	 * Returns Compute the 2D area for a Polygon
	 */
	SFCGAL_API double     area( const Polygon & g ) ;
	/**
	 * Returns the 2D area for a GeometryCollection
	 */
	SFCGAL_API double     area( const GeometryCollection & g ) ;
	/**
	 * Returns the 2D area for a TriangulatedSurface
	 */
	SFCGAL_API double     area( const TriangulatedSurface & g ) ;
	/**
	 * Returns the 2D area for a TriangulatedSurface
	 */
	SFCGAL_API double      area( const PolyhedralSurface & g ) ;


	/**
	 * Returns 3D area for a Geometry
	 * @ingroup public_api
	 * @warning Solid area is set to 0 (might be defined as the area of the surface)
	 */
	SFCGAL_API double         area3D( const Geometry & g ) ;


	/**
	 * Returns 3D area for a Polygon
	 */
	SFCGAL_API double         area3D( const Polygon & g ) ;
	/**
	 * Returns the 3D area for a MultiPolygon
	 */
	SFCGAL_API double         area3D( const GeometryCollection & g ) ;

	/**
	 * Returns the 3D area for a PolyhedralSurface
	 */
	SFCGAL_API double         area3D( const PolyhedralSurface & g ) ;

	/**
	 * Returns the 3D area for a Triangle
	 */
	SFCGAL_API double         area3D( const Triangle & g ) ;

	/**
	 * Returns the 3D area for a TriangulatedSurface
	 */
	SFCGAL_API double         area3D( const TriangulatedSurface & g ) ;



}//algorithm
}//SFCGAL


#endif

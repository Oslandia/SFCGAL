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
#ifndef _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_
#define _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_

#include <memory>

namespace SFCGAL {
	class Geometry ;
	class Polygon ;
	class MultiPolygon ;
	class MultiLineString ;
}

namespace SFCGAL {
namespace algorithm {
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 * @param g input geometry
	 * @param autoOrientation check and fix polygon orientation
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation = true ) ;
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const Polygon& g, bool autoOrientation = true ) ;
	/**
	 * @brief build a 2D straight skeleton for a Polygon
	 */
	std::auto_ptr< MultiLineString > straightSkeleton( const MultiPolygon& g, bool autoOrientation = true ) ;

}//namespace algorithm
}//namespace SFCGAL


#endif

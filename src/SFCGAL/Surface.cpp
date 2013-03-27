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
#include <SFCGAL/Surface.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
Surface::~Surface()
{

}

///
///
///
int  Surface::dimension() const
{
	return 2 ;
}

///
///
///
Surface::Surface() :
	Geometry()
{

}

///
///
///
Surface::Surface( Surface const& other ) :
	Geometry()
{

}

/**
 * [OGC/SFS]"The area of this Surface, as measured in the spatial reference system of this Surface"
 */
//virtual double area() const = 0 ;
/**
 * [OGC/SFS]"The mathematical centroid for this Surface as a Point. The result in not guaranteed to
 * be on this Surface"
 */
//virtual Point centroid() const = 0 ;
/**
 * [OGC/SFS]"A Point guaranteed to be on this Surface"
 * @warning empty point is isEmpty()
 */
//virtual Point pointOnSurface() const = 0 ;


}//SFCGAL




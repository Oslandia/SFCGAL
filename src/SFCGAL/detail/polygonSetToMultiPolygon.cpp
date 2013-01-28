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
#include <SFCGAL/detail/polygonSetToMultiPolygon.h>

#include <CGAL/Polygon_with_holes_2.h>

#include <list>

namespace SFCGAL {
namespace detail {

///
///
///
std::auto_ptr< MultiPolygon > polygonSetToMultiPolygon( const CGAL::Polygon_set_2< Kernel > & polygonSet )
{
	typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;

	std::list<Polygon_with_holes_2> res;
	polygonSet.polygons_with_holes( std::back_inserter(res) ) ;

	std::auto_ptr< MultiPolygon > result( new MultiPolygon );
	for ( std::list<Polygon_with_holes_2>::const_iterator it = res.begin(); it != res.end(); ++it ){
		result->addGeometry( new Polygon( *it ) );
	}
	return result ;
}

} // namespace detail
} // namespace SFCGAL

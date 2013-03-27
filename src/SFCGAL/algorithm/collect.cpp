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
#include <SFCGAL/algorithm/collect.h>
#include <SFCGAL/all.h>

namespace SFCGAL {
namespace algorithm
{
	std::auto_ptr<Geometry> collect( const Geometry& ga, const Geometry& gb )
	{
		if ( ga.geometryTypeId() == gb.geometryTypeId() ) {
			if ( ga.geometryTypeId() == TYPE_POINT ) {
				MultiPoint *mp = new MultiPoint;
				mp->addGeometry( ga );
				mp->addGeometry( gb );
				return std::auto_ptr<Geometry>(mp);
			}
			else if ( ga.geometryTypeId() == TYPE_LINESTRING ) {
				MultiLineString *mls = new MultiLineString();
				mls->addGeometry(ga);
				mls->addGeometry(gb);
				return std::auto_ptr<Geometry>( mls );
			}
			else if ( ga.geometryTypeId() == TYPE_POLYGON ) {
				MultiPolygon *mp = new MultiPolygon();
				mp->addGeometry(ga);
				mp->addGeometry(gb);
				return std::auto_ptr<Geometry>( mp );
			}
			else if ( ga.geometryTypeId() == TYPE_SOLID ) {
				MultiSolid *mp = new MultiSolid();
				mp->addGeometry(ga);
				mp->addGeometry(gb);
				return std::auto_ptr<Geometry>( mp );
			}
		}

		// else
		GeometryCollection* coll = new GeometryCollection();
		coll->addGeometry(ga);
		coll->addGeometry(gb);
		return std::auto_ptr<Geometry>( coll );
	}
}
}

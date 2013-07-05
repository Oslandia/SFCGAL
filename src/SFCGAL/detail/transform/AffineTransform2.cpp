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
#include <SFCGAL/detail/transform/AffineTransform2.h>

#include <SFCGAL/Point.h>

namespace SFCGAL {
namespace transform {

///
///
///
AffineTransform2::AffineTransform2( CGAL::Aff_transformation_2< Kernel > transform ):
	_transform( transform ) {

}

/*
 * [SFCGAL::Transform]
 */
void AffineTransform2::transform( Point& p ) {
	if ( ! p.isEmpty() ){
		p = Point( p.toPoint_2().transform( _transform ) );
	}
}


}//transform
}//SFCGAL


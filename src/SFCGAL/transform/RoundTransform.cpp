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
#include <SFCGAL/transform/RoundTransform.h>

#include <SFCGAL/all.h>

namespace SFCGAL {
namespace transform {

///
///
///
RoundTransform::RoundTransform( const double & scale ):
	_scale( scale )
{

}

///
///
///
void RoundTransform::transform( Point & p )
{
	p.coordinate().round( _scale ) ;
}

}//transform
}//SFCGAL




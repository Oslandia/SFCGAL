/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <SFCGAL/MultiPolygon.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/offset.h>

using namespace SFCGAL ;

int main(){
	std::unique_ptr< Geometry > g( io::readWkt("MULTIPOINT(0 0,5 6,3 2,7 1,4 1,3 5,2 9)") );

	for ( size_t i = 1; i <= 50; i++ ){
		std::unique_ptr< Geometry > buffer( algorithm::offset( *g, 0.2*i ) );
		std::cout << buffer->asText(5) << std::endl ;
	}

	return 0 ;
}

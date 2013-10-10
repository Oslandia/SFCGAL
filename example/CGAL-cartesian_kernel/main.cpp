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
#include <CGAL/Cartesian.h>
#include <CGAL/squared_distance_2.h>



//declaration Kernel simple
typedef CGAL::Cartesian< double > K ;
typedef K::Point_2 Point_2 ;


int main(){
	Point_2 a( 0.0, 0.0 );
	Point_2 b( 3.0, 4.0 );
	K::Compute_squared_distance_2 squared_distance;
	std::cout << squared_distance( a, b ) << std::endl;	
	return 0;
}


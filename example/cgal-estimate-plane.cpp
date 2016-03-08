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
#include <iostream>
#include <algorithm>

#include <SFCGAL/log.h>

#include <CGAL/Random.h>
#include <CGAL/Cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>



typedef CGAL::Cartesian< double > K ;
typedef K::Point_3 Point;
typedef K::Plane_3 Plane;


// http://doc.cgal.org/latest/Principal_component_analysis/group__PkgPrincipalComponentAnalysisDLLSF3.html
int main(){
    int N_POINTS = 100000;
    BOOST_LOG_TRIVIAL(info) << "generate " << N_POINTS << " random points around OXY..." ;
    CGAL::Random r;
    std::vector<Point> points;
    points.reserve(N_POINTS);
    for ( int i = 0; i < N_POINTS; i++ ){
        points.push_back(Point(
            r.get_double(0.0,10.0),
            r.get_double(0.0,10.0),
            r.get_double(-0.1,0.1)
        ));
    }

    BOOST_LOG_TRIVIAL(info) << "estimate plane..." ;
    Plane plane;
    Point centroid;
    K::FT quality = CGAL::linear_least_squares_fitting_3(
        points.begin(),
        points.end(),
        plane,
        centroid,
        CGAL::Dimension_tag<0>()
    );
    BOOST_LOG_TRIVIAL(info) << "quality : "<< quality ;
    BOOST_LOG_TRIVIAL(info) << "plane : "<< plane << " (normal : " << plane.orthogonal_vector() << ")" ;
    BOOST_LOG_TRIVIAL(info) << "centroid : "<< centroid ;
    
    K::FT squaredDistanceMax = 0.0;
    for ( const Point & p : points ){
        squaredDistanceMax = std::max(
            squaredDistanceMax,
            CGAL::squared_distance(p, plane.projection(p)) 
        );
    }
    BOOST_LOG_TRIVIAL(info) << "dmax : " << CGAL::sqrt(CGAL::to_double(squaredDistanceMax)) ;
    
	return 0;
}

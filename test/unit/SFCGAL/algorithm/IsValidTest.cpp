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
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/isValid.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

// print each ring has a different polygon
void vtk( const Polygon & poly, const std::string & file)
{
    std::stringstream pointStr;
    std::stringstream polyStr;
    size_t numPoints=0;
    size_t numData=0;
    for ( size_t r=0; r!=poly.numRings(); ++r ) {
        polyStr << poly.ringN(r).numPoints();
        for ( size_t p=0; p!=poly.ringN(r).numPoints(); ++p ) {
            pointStr << poly.ringN(r).pointN(p).x() << " " << poly.ringN(r).pointN(p).y() << " " << poly.ringN(r).pointN(p).z() << "\n";
            polyStr << " " << numPoints;
            ++numPoints;
            ++numData;
        }
        ++numData;
        polyStr << "\n";
    }
    std::ofstream out(file.c_str());
    out << "# vtk DataFile Version 1.0\n"
        << "Polygon output\n"
        << "ASCII\n"
        << "\n"
        << "DATASET POLYDATA\n"
        << "POINTS " << numPoints << " float\n"
        << pointStr.str()
        << "POLYGONS " << poly.numRings() << " " << numData << "\n"
        << polyStr.str();
}

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IsValid )

BOOST_AUTO_TEST_CASE( geometryIsValid )
{
    struct TestGeometry
    {
        const std::string _wkt;
        const bool _valid;
        const std::string _comment;
    };

    TestGeometry testGeometry[] = 
    {
    // Point2D 
        // valid
        {"POINT EMPTY", true, ""},
        {"POINT(-1.0 -1.0)", true, ""},
    // Point3D 
        // valid
        {"POINT(-1.0 -1.0 -1.0)", true, ""},
    // Linestring2D
        // valid
        {"LINESTRING EMPTY", true , ""},
        {"LINESTRING(-1.0 -1.0,1.0 1.0)", true , ""},
        // invalid
        {"LINESTRING(-1.0 -1.0,-1.0 -1.0)", false, "zero length"},
    // Linestring3D
        {"LINESTRING(-1.0 -1.0 -1.0,1.0 1.0 1.0)", true , ""},
        {"LINESTRING(-1.0 -1.0 -1.0,-1.0 -1.0 -1.0)", false, "zero length"},
    // Polygon2D
        // valid
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0))", true , ""},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,0.5 -0.5,-0.5 -0.5))", true, "with interior ring"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,1.0 -0.5,-0.5 -0.5))", true, "one contact point between interior ans exterior"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.1 0.5,-0.1 -0.5,-0.5 -0.5),(0.1 -0.5,0.1 0.5,0.5 0.5,0.5 -0.5,0.1 -0.5))", true, "with interior rings"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.1 0.5,0.1 -0.5,-0.5 -0.5),(0.1 -0.5,0.1 0.5,0.5 0.5,0.5 -0.5,0.1 -0.5))", true, "one contact point between 2 interior rings"},
        // invalid
        {"POLYGON((-1.0 -1.0,-1.0 1.0,-1.0 -1.0))", false, "only 3 points"},
        {"POLYGON((-1.0 -1.0,-1.0 1.0,1.0 1.0,-1.0 -1.0,-1.0 1.0))", false, "not closed"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 -1.0,-1.0 -1.0,-1.0 -1.0))", false, "zero surface"},
        {"POLYGON((-1.0 -1.0,2.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0))", false, "ring adjacency (spyke)"},
        {"POLYGON((-1.0 -1.0,1.0 1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0))", false, "ring intersection"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring only 3 points"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring counterclockwise"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,-0.5 -0.5,-0.5 0.5))", false, "interior ring not closed"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 -0.5,-0.5 -0.5,-0.5 -0.5))", false, "interior ring no surface"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.7 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring adjacency (spyke)"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 0.5,0.5 -0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring intersection"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,1.0 -0.5,1.0 0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring adjacent to exterior"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,2.0 -0.5,2.0 0.5,-0.5 0.5,-0.5 -0.5))", false, "interior ring intersection with exterior"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(1.5 -0.5,2.0 -0.5,2.0 0.5,1.5 0.5,1.5 -0.5))", false, "interior ring is ouside exterior"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.0 -0.5,0.0 0.5,-0.5 0.5,-0.5 -0.5),(0.0 -0.5,0.5 -0.5,0.5 0.5,0.0 0.5,0.0 -0.5))", false, "adjacent interior rings"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.2 -0.4,-0.1 0.5,-0.5 0.5,-0.5 -0.5),(0.1 -0.5,0.5 -0.5,0.5 0.5,0.1 0.5,0.1 -0.5))", false, "intersection between interior rings"},
        {"POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5),(-0.2 -0.2,0.2 -0.2,0.2 0.2,-0.2 0.2,-0.2 -0.2))", false, "one inetrior ring is inside the other"},
    // Polygon3D
        // valid
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", true, ""},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 0.5 0.5,1.0 0.5 -0.5,1.0 -0.5 -0.5))", true, "with interior ring"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 0.5 0.5,1.0 1.0 -0.5,1.0 -0.5 -0.5))", true, "one contact point between interior and exterior"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 -0.1 0.5,1.0 -0.1 -0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.1 0.5,1.0 0.5 0.5,1.0 0.5 -0.5,1.0 0.1 -0.5))", true, "two interior rings"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 -0.1 0.5,1.0 0.1 -0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.1 0.5,1.0 0.5 0.5,1.0 0.5 -0.5,1.0 0.1 -0.5))", true, "one contact point between 2 interior rings"},
        // invalid
        {"POLYGON((1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", false, "only 3 points"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 1.0 1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0))", false, "not closed"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 -1.0))", false, "zero surface"},
        {"POLYGON((0.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", false, "non plane"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 2.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", false, "ring adjacency (spike)"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", false, "ring self intersection"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))", false, "interior ring only 3 points"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 0.5 0.5,1.0 -0.5 -0.5,1.0 -0.5 0.5))", false, "interior ring not closed"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 -0.5,1.0 0.5 -0.5,1.0 -0.5 -0.5,1.0 -0.5 -0.5))", false, "interior ring no surface"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.7 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))", false, "interior ring adjacency (1.0 spyke)"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 0.5,1.0 0.5 -0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))", false, "interior ring intersection"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 1.0 -0.5,1.0 1.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))", false, "interior ring adjacent to exterior"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 2.0 -0.5,1.0 2.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))", false, "interior ring intersection with exterior"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 1.5 -0.5,1.0 2.0 -0.5,1.0 2.0 0.5,1.0 1.5 0.5,1.0 1.5 -0.5))", false, "interior ring is ouside exterior"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.0 -0.5,1.0 0.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.0 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.0 0.5,1.0 0.0 -0.5))", false, "adjacent interior rings"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.2 -0.4,1.0 -0.1 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.1 0.5,1.0 0.1 -0.5))", false, "intersection between interior rings"},
        {"POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 -0.2 -0.2,1.0 0.2 -0.2,1.0 0.2 0.2,1.0 -0.2 0.2,1.0 -0.2 -0.2))", false, "one inetrior ring is inside the other"},
    // Multipoint2D
        // valid
        {"MULTIPOINT((-1.0 -1.0),(1.0 1.0))", true, ""},
    // Multipoint3D
        // valid
        {"MULTIPOINT((-1.0 -1.0 -1),(1.0 1.0 1.0))", true, ""},
    // MultiLinestring2D 
        // valid
        {"MULTILINESTRING((-1.0 -1.0,1.0 1.0),(1.0 1.0,1.0 -1.0))", true, ""},
        // invalid
        {"MULTILINESTRING((-1.0 -1.0,1.0 1.0),(1.0 1.0,1.0 1.0))", false, "second linestring has zero length"},
    // MultiLinestring3D 
        // valid
        {"MULTILINESTRING((1.0 -1.0 -1.0,1.0 1.0 1.0),(1.0 1.0 1.0,1.0 1.0 -1.0))", true, ""},
        // invalid
        {"MULTILINESTRING((1.0 -1.0 -1.0,1.0 1.0 1.0),(1.0 1.0 1.0,1.0 1.0 1.0))", false, "the second linestring has length zero"},
    // MultiPolygon2D
        // valid
        {"MULTIPOLYGON(((-3.0 -1.0,-1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0)))", true, ""},
        {"MULTIPOLYGON(((-3.0 -1.0,1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0)))", true, "one contact point"},
        // invalid
        {"MULTIPOLYGON(((-3.0 -1.0,-1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0)))", false, "second polygon is invalid"},
        {"MULTIPOLYGON(((-2.0 -1.0,0.0 -1.0,0.0 1.0,-2.0 1.0,-2.0 -1.0)),((0.0 -1.0,2.0 -1.0,2.0 1.0,0.0 1.0,0.0 -1.0)))", false, "adjacent"},
        {"MULTIPOLYGON(((-3.0 -1.0,1.3 0.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0)))", false, "intersection"},
    // MultiPolygon3D
        // valid
        {"MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0)))", true, ""},
        {"MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0)))", true, "one contact point"},
        // invalid
        {"MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0)))", false, "second polygon is invalid"},
        {"MULTIPOLYGON(((1.0 -2.0 -1.0,1.0 0.0 -1.0,1.0 0.0 1.0,1.0 -2.0 1.0,1.0 -2.0 -1.0)),((1.0 0.0 -1.0,1.0 2.0 -1.0,1.0 2.0 1.0,1.0 0.0 1.0,1.0 0.0 -1.0)))", false, "adjacent"},
        {"MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 1.3 0.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0)))", false, "intersection"},
    // Triangle2D
        // valid
        {"TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0))", true, ""},
        // invalid
        {"TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 -1.0))", false, "only 3 points"},
        {"TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 -1.0,-1.0 -1.0))", false, "zero surface"},
    // Triangle3D
        // valid
        {"TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))", true, ""},
    // invalid
        {"TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0))", false, "only 3 points"},
        {"TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 -1.0))", false, "zero surface"},
    // TIN2D
        // valid
        {"TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((-1.0 1.0,1.0 -1.0,1.0 1.0,-1.0 1.0)))", true, ""},
        // invalid
        {"TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((-1.0 1.0,1.0 -1.0,1.0 1.0)))", false, "second triangle is invalid"},
        {"TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((1.0 -1.0,-1.0 1.0,1.0 1.0,1.0 -1.0)))", false, "inconsitent orientation"},
        {"TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((0.0 1.0,2.0 -1.0,2.0 1.0,0.0 1.0)))", false, "not connected"},
    // TIN3D
        // valid
        {"TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 -1.0 1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0)))", true, ""},
        // invalid
        {"TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 -1.0 1.0,1.0 1.0 -1.0,1.0 1.0 1.0)))", false, "second triangle is invalid"},
        {"TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0)))", false, "inconsitent orientation"},
        {"TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 0.0 1.0,1.0 2.0 -1.0,1.0 2.0 1.0,1.0 0.0 1.0)))", false, "not connected"},
        {"TIN(((0.0 0.0 0.0,1.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0)),((0.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 1.0,0.0 0.0 0.0)),((0.0 0.0 0.0,0.0 0.0 1.0,0.5 0.1 -0.1,0.0 0.0 0.0)))", false, "self intersect"},
    };
    const std::size_t nbOfTest = sizeof(testGeometry)/sizeof(TestGeometry);
    std::cout << nbOfTest << "\n";
    for (std::size_t t=0; t<nbOfTest; t++)
    {
        const TestGeometry & tg = testGeometry[t];
        std::auto_ptr< Geometry > g( io::readWkt(tg._wkt) );
        if ( g->is< Polygon >() ) vtk( g->as< Polygon >(), (boost::format("/tmp/geom_%d.vtk") % t).str() );
        Validity v = algorithm::isValid( *g );
        BOOST_CHECK_MESSAGE( v == tg._valid, ( boost::format("%d:%s should be %s (%s), algorythim says: %s") % t % tg._wkt % (tg._valid?"valid":"invalid") % tg._comment % v.reason() ) );
    }

}

BOOST_AUTO_TEST_SUITE_END()

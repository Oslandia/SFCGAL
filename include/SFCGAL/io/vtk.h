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
#ifndef _SFCGAL_IO_VTK_H_
#define _SFCGAL_IO_VTK_H_

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <SFCGAL/all.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

namespace SFCGAL{
namespace io {
// print each ring has a different polygon
inline
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

inline
void vtk( const MultiPolygon & multiPoly, const std::string & file)
{
    std::stringstream pointStr;
    std::stringstream polyStr;
    size_t numPoints=0;
    size_t numRings=0;
    size_t numData=0;
    for ( size_t i=0; i!=multiPoly.numGeometries(); ++i) {
        const Polygon& poly = multiPoly.polygonN(i);
        for ( size_t r=0; r!=poly.numRings(); ++r ) {
            polyStr << poly.ringN(r).numPoints();
            for ( size_t p=0; p!=poly.ringN(r).numPoints(); ++p ) {
                pointStr << poly.ringN(r).pointN(p).x() << " " << poly.ringN(r).pointN(p).y() << " " << poly.ringN(r).pointN(p).z() << "\n";
                polyStr << " " << numPoints;
                ++numPoints;
                ++numData;
            }
            ++numData;
            ++numRings;
            polyStr << "\n";
        }
    }
    std::ofstream out(file.c_str());
    out << "# vtk DataFile Version 1.0\n"
        << "Polygon output\n"
        << "ASCII\n"
        << "\n"
        << "DATASET POLYDATA\n"
        << "POINTS " << numPoints << " float\n"
        << pointStr.str()
        << "POLYGONS " << numRings << " " << numData << "\n"
        << polyStr.str();
}
}
}

#endif

/**
 *   SFCGAL
 *
 *   Copyright (C) 2020 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2020 IGN (http://www.ign.fr)
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

// SFCGAL
#include <SFCGAL/algorithm/lineSegment.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Exception.h>

namespace SFCGAL
{

namespace algorithm
{

std::unique_ptr<LineString> lineSegment( const LineString& ls, double start, double end )
{
    BOOST_THROW_EXCEPTION( Exception( "SFCGAL::lineSegment:- Not implemented." ) );
    return nullptr;
}

std::unique_ptr<LineString> lineSegment3D( const LineString& ls, double start, double end )
{
    BOOST_THROW_EXCEPTION( Exception( "SFCGAL::lineSegment3D:- Not implemented." ) );
    return nullptr;
}

} // ! namespace algorithm

} // ! namespace SFCGAL


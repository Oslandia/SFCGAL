/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2020 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2020 IGN (http://www.ign.fr)
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

#ifndef _SFCGAL_ALGORITHM_LINESEGMENT_H_
#define _SFCGAL_ALGORITHM_LINESEGMENT_H_

// C++
#include <memory>

// SFCGAL
#include <SFCGAL/config.h>

namespace SFCGAL
{

// Class forward delclarations.
class LineString ;

namespace algorithm
{

/**
 * @brief Retrieve a segment of a specified LineString,
 *   between the specified fractional distances from the start
 *   of the specified LineString.
 * @param ls The specified LineString.
 * @param start The fraction along the specified LineString defining the
 *   start of the desired segment.
 * @param end The fraction along the specified LineString defining the
 *   end of the desired segment.
 * @note Negative values of {@code start} and/or {@code end} will be
 *   interpreted as a fractional distance taken from the end of the
 *   specified LineString. +/-0 will always be interpreted as the start
 *   of {@code ls}.
 * @note For open lines, a negative length range will result in a line
 *   segment terminating at the specified points, but with an
 *   orientation reversed relative to {@code ls}. For closed lines the
 *   a negative range corresponds to the complentary section of {@code ls}
 *   with an orientation equal to that of it.
 * @return The specified line segment.
 */
SFCGAL_API std::unique<LineString> lineSegment( const LineString& ls
					      , double start
					      , double end
					      );

} // ! namespace algorithm

} // ! namespace SFCGAL


#endif // ! _SFCGAL_ALGORITHM_LINESEGMENT_H_

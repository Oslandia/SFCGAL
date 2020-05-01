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

// C++
#include <cmath>

// SFCGAL
#include <SFCGAL/algorithm/lineSegment.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Exception.h>

namespace SFCGAL
{

namespace algorithm
{

namespace
{
  std::unique_ptr<LineString> openLineSegment( const LineString& ls, double start, double end )
  {

  }

  std::unique_ptr<LineString> closedLineSegment( const LineString& ls, double start, double end )
  {

  }
  
} // ! anon namespace

std::unique_ptr<LineString> lineSegment( const LineString& ls, double start, double end )
{
    const double len = SFCGAL::algorithm::length(ls);
    if ( ls.isEmpty() || ( len < tol ) )
    {
        // Empty or zero-length line, start and end are
        // irrelevant.
        return std::unique_ptr<LineString>( new LineString() );
    }

    // Check for out of range start, end.

    if ( std::fabs( start ) > 1.0 )
    {
        BOOST_THROW_EXCEPTION( Exception( "SFCGAL::algorithm::lineSegment: start value out of range." ) );
    }

    if ( std::fabs( end ) > 1.0 )
    {
        BOOST_THROW_EXCEPTION( Exception( "SFCGAL::algorithm::lineSegment: end value out of range." ) );
    }

    // Check for equal start and end.
    if ( ( std::fabs ( start - end ) < tol ) || ( std::fabs( start + end ) < tol ) )
    {
        // start and end are equal, return empty line segment.
        return std::unique_ptr<LineString>( new LineString() );
    }

    const long N = static_cast<long>( ls.numPoints() );
    const bool is_measured = ls.isMeasured();

    if ( ls.isClosed() )
    {
        // Closed lines.
    }
    else
    {
        // Open lines.
      
        // Convert start and end into their equivalent positive values.

        if ( start < 0.0 )
	{
	    start = 1.0 - start;
	}

        if ( end < 0.0 )
	{
	    end = 1.0 - end;
	}

	// Swap start and end so that they define a positive
	// range on ls, setting the reverse flag to reverse
	// the resulting segment if necessary.

        bool reverse = false;
	if ( start < end )
	{
	    std::swap( start, end );
	    reverse = true;
	}

	// Find Point immediately before/on start position.

	double cur_length = 0.0;

	double seg_length = 0.0;
	double target_length = len * start;
	bool on_point = false;

	std::size_t start_idx = 0 ;
	for ( start_idx = 0 ; start_idx < N ; ++start_idx )
	{
	  const Point& p = ls.pointN( start_idx );
	  const Point& q = ls.pointN( start_idx+1 );

	  seg_length = std::sqrt( std::pow(CGAL::to_double(p.x()), 2.0) +
				  std::pow(CGAL::to_double(p.y()), 2.0)
				);

	  cur_length += seg_length;

	  if ( std::fabs( cur_length - target_length ) < tol )
	  {
	    // Adjust start_idx to be that of the Point coincident
	    // with the desired start position.

	    ++start_idx;
	    on_point = true;

	    break;
	  }
	  else if ( cur_length > target_length )
	  {
	    // We went too far. Subtract seg_length so
	    // cur_length is the distance along ls
	    // to the start_idx'th point.
	    cur_length -= seg_length;
	    break;
	  }
	}

	// Calculate fraction between start_idx and start_idx + 1 where
	// the desired start position resides.

	double start_frac = 0.0;
	if ( ! on_point )
	{
	  BOOST_ASSERT( seg_length > tol );
	  start_frac = ( target_length - cur_length ) / seg_length ;
	}
	
	// Calculate start point.

	Point start;
	if ( ! on_point )
	{
	    start = ls.pointN( start_idx );
	}
	else
	{
	  const Point& p = ls.pointN( start_idx );
	  const Point& q = ls.pointN( start_idx + 1 );

	  start.x() = p.x() + ( start_frac * ( q.x() - p.x() ) ) ;
	  start.x() = p.x() + ( start_frac * ( q.x() - p.x() ) ) ;

	  if ( is_measured )
	  {
	      start.m() = p.m() + ( start_frac * ( q.m() - p.m() ) ) ;
	  }
        }

	// Find Point immediately before/on end position.

	seg_length = 0.0;
	target_length = len * end;
	bool on_point = false;

	std::size_t end_idx = start_idx ;
	for ( end_idx = 0 ; end_idx < N ; ++end_idx )
	{
	  const Point& p = ls.pointN( end_idx );
	  const Point& q = ls.pointN( end_idx+1 );

	  seg_length = std::sqrt( std::pow(CGAL::to_double(p.x()), 2.0) +
				  std::pow(CGAL::to_double(p.y()), 2.0)
				);

	  cur_length += seg_length;

	  if ( std::fabs( cur_length - target_length ) < tol )
	  {
	    // Adjust end_idx to be that of the Point coincident
	    // with the desired end position.

	    ++end_idx;
	    on_point = true;

	    break;
	  }
	  else if ( cur_length > target_length )
	  {
	    // We went too far. Subtract seg_length so
	    // cur_length is the distance along ls
	    // to the end_idx'th point.
	    cur_length -= seg_length;
	    break;
	  }
	}

	// Calculate fraction between end_idx and end_idx + 1 where
	// the desired end position resides.

	double end_frac = 0.0;
	if ( ! on_point )
	{
	  BOOST_ASSERT( seg_length > tol );
	  end_frac = ( target_length - cur_length ) / seg_length ;
	}
	
	// Calculate end point.

	Point end;
	if ( ! on_point )
	{
	    end = ls.pointN( end_idx );
	}
	else
	{
	  const Point& p = ls.pointN( end_idx );
	  const Point& q = ls.pointN( end_idx + 1 );

	  end.x() = p.x() + ( end_frac * ( q.x() - p.x() ) ) ;
	  end.x() = p.x() + ( end_frac * ( q.x() - p.x() ) ) ;

	  if ( is_measured )
	  {
	      end.m() = p.m() + ( end_frac * ( q.m() - p.m() ) ) ;
	  }
        }

	// Construct desired line segment.

	LineString segment;
	
	// Add start point.
	segment.addPoint( start );

	for ( std::size_t i = start_idx + 1 ; i <= end_idx; ++i )
	{
	    segment.addPoint( ls.pointN( i ) ) ;
	}

	// Add end point if we have not already.
	if ( ! on_point )
	{
	    segment.addPoint ( end ) ;
	}

	if ( reverse )
	{
	    // Reverse the constructed segment.
	    segment.reverse();
	}

	return segment;
    }

    return nullptr;
}

std::unique_ptr<LineString> lineSegment3D( const LineString& ls, double start, double end )
{
    BOOST_THROW_EXCEPTION( Exception( "SFCGAL::lineSegment3D:- Not implemented." ) );
    return nullptr;
}

} // ! namespace algorithm

} // ! namespace SFCGAL


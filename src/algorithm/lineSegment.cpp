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
  Point find_position( const LineString& ls
	 	     , const long N
		     , const double line_fraction
		     , const double tol
		     , const bool find_start
		     , std::size_t& idx
		     , std::size_t& frac
		     , bool& on_point
		     )
  {
      double cur_length = 0.0;
      double seg_length = 0.0;
      double target_length = len * line_fraction;
      on_point = false;

      for ( ; idx < N ; ++idx )
      {
	  const Point& p = ls.pointN( idx );
	  const Point& q = ls.pointN( idx+1 );

	  seg_length = std::sqrt( std::pow(CGAL::to_double(p.x()), 2.0) +
				  std::pow(CGAL::to_double(p.y()), 2.0)
				);

	  cur_length += seg_length;

	  if ( std::fabs( cur_length - target_length ) < tol )
	  {
	      // Adjust idx to be that of the Point coincident
	      // with the desired position.

	      ++idx;
	      on_point = true;

	      break;
	  }
	  else if ( cur_length > target_length )
	  {
	      // We went too far. Subtract seg_length so
	      // cur_length is the distance along ls
	      // to the idx'th point.
	      cur_length -= seg_length;
	      break;
	  }
      }

      // Calculate fraction between idx and idx + 1 where
      // the desired position resides.

      frac = 0.0;
      if ( ! on_point )
      {
	  BOOST_ASSERT( seg_length > tol );
	  frac = ( target_length - cur_length ) / seg_length ;
      }

      // Calculate point.

      Point ret;
      if ( on_point )
      {
	  ret = ls.pointN( idx );
      }
      else
      {
	  const Point& p = ls.pointN( idx );
	  const Point& q = ls.pointN( idx + 1 );

	  ret.x() = p.x() + ( frac * ( q.x() - p.x() ) ) ;
	  ret.y() = p.y() + ( frac * ( q.y() - p.y() ) ) ;

	  if ( is_measured )
	  {
	      ret.m() = p.m() + ( frac * ( q.m() - p.m() ) ) ;
	  }
      }

      return ret;
  }

} // ! anonymous namespace

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

    // Convert start and end into their equivalent positive values.

    if ( start < 0.0 )
    {
	start = 1.0 - start;
    }

    if ( end < 0.0 )
    {
	end = 1.0 - end;
    }

    bool reverse = false;
    if ( start < end )
    {
	// Swap the start and end positions so that they
	// define a positive range on ls, setting the
	// reverse flag to reverse to indicate that
	// we have done so. Note that reverse being set
        // has different consequences for open and
        // closed lines.

        std::swap( start, end );
	reverse = true;
    }

    // Find Point immediately before/on start position.
    std::size_t start_idx = 0; // Must initialise first.
    double start_frac = 0.0;
    on_point = false;
    const Point pstart = find_position( ls
				      , N
				      , start
				      , tol
				      , true // Find start.
				      , start_idx
				      , start_frac
				      , on_point // This result is not used.
				      );

    // Find Point immediately before/on end position.
    std::size_t end_idx = start_idx; // Must initialise first.
    double end_frac = 0.0;
    on_point = false;
    const Point pend = find_position( ls
				    , N
				    , end
				    , tol
				    , false // Find end.
				    , end_idx
				    , end_frac
				    , on_point // This result is used.
				    );

    if ( reverse && ls.isClosed() )
    {
        // For closed lines we always want to follow the
        // direction of the original line. A set reversed
        // flag indicates that we are going to cross the
        // join, and hence we add N to the end_idx and
        // use modulus operation when dereferncing the points
        // to be added to the desired line segment.
        std::swap( start_idx, end_idx );
	std::swap( start_frac, end_frac );
	std::swap( pstart, pend );

	end_idx += N;
    }

    // Construct the desired line segment.

    LineString segment;

    // Add start point.

    segment.addPoint( pstart );

    // Add intermediate points.

    for ( std::size_t i = start_idx + 1 ; i <= end_idx; ++i )
    {
        const Point& p = ls.pointN( i % N ) ) ;

        if ( reverse && closed && ( i == 0 ) )
	{
	    // Check we are not adding a duplicate point.
	    // at the join of the closed line.
	    if ( p == segment.end() )
	    {
	        // Skip the duplicate point. If this was
	        // the last point and we ended up with
	        // an invalid single point segment, the
	        // fact that it is now made invalid is
	        // not a concern since if added we would
	        // have obtained a zero-length segment,
	        // which is also invalid.
	        continue;
	    }
	}

	segment.addPoint( p ) ;
    }

    // Add end point if we have not already.

    if ( ! on_point )
    {
	segment.addPoint ( pend ) ;
    }

    if ( reverse && ( ! closed ) )
    {
	// Reverse the constructed segment.
	segment.reverse();
    }

    return segment;
}

std::unique_ptr<LineString> lineSegment3D( const LineString& ls, double start, double end )
{
    BOOST_THROW_EXCEPTION( Exception( "SFCGAL::lineSegment3D:- Not implemented." ) );
    return nullptr;
}

} // ! namespace algorithm

} // ! namespace SFCGAL


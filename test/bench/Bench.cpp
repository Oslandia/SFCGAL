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
#include "Bench.h"


namespace SFCGAL {

///
///
///
Bench::~Bench()
{

}

///
///
///
void Bench::start( const std::string & description )
{
	_timers.push( std::make_pair( description, timer_t() ) );
	_timers.top().second.start();
}

///
///
///
void Bench::start( const boost::basic_format<char>& description )
{
	start( description.str() );
}

///
///
///
void Bench::stop()
{
	BOOST_ASSERT( ! _timers.empty() ) ;
	_timers.top().second.stop();
	s() << _timers.top().first << "\t" << ( _timers.top().second.elapsed().wall * 1.0e-9 ) << std::endl ;
	_timers.pop() ;
}

///
///
///
Bench & Bench::instance()
{
	static Bench bench ;
	return bench ;
}

///
///
///
std::ostream & Bench::s()
{
	return *_s ;
}

///
///
///
Bench::Bench():
	_s( &std::cout )
{

}


} // namespace SFCGAL


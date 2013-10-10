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
#ifndef _SFCGAL_BENCH_H_
#define _SFCGAL_BENCH_H_

#include <iostream>
#include <stack>

#include <boost/timer/timer.hpp>

#include <boost/format.hpp>

namespace SFCGAL {

/**
 * @brief helper class to write formated benchs
 */
class Bench {
public:
    typedef boost::timer::cpu_timer timer_t ;

    /**
     * destructor
     */
    ~Bench();

    /**
     * start a bench
     */
    void start( const std::string& description ) ;
    /**
     * start a bench
     */
    void start( const boost::basic_format<char>& description ) ;
    /**
     * stop a bench
     */
    void stop() ;

    /**
     * get bench instance
     */
    static Bench& instance() ;

    /**
     * Get output stream
     */
    std::ostream& s() ;
private:
    /**
     * default constructor
     */
    Bench();
    /**
     * copy constructor
     */
    Bench( const Bench& bench );

    /**
     * output stream to write bench result (default is std::cout)
     */
    std::ostream* _s ;
    /**
     * timer stack with description
     */
    std::stack< std::pair< std::string, timer_t > > _timers ;
};

/**
 * @Get bench instance
 */
inline Bench& bench()
{
    return Bench::instance() ;
}

} // namespace SFCGAL

#endif


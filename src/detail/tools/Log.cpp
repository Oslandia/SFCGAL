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
#include <boost/date_time.hpp>

#include <SFCGAL/detail/tools/Log.h>

namespace SFCGAL {

///
///
///
Logger::~Logger()
{

}

///
///
///
Logger* Logger::get()
{
    static Logger log( std::cout );
    return &log;
}

///
///
///
void Logger::log(
    const Level& level,
    const boost::format& message,
    const std::string& filename,
    const int& lineNumber
)
{
    log( level, message.str(), filename, lineNumber );
}

///
///
///
void Logger::log(
    const Level& level,
    const std::string& message,
    const std::string& filename,
    const int& lineNumber
)
{
    using namespace boost::posix_time;

    if ( level < _logLevel ) {
        return ;
    }

    //ptime now = second_clock::local_time();
    //_out << to_iso_string(now) << ":" ;

    if ( _displayFilePosition && ! filename.empty() ) {
        _out << filename << ":" ;
    }

    if ( _displayFilePosition && lineNumber >= 0 ) {
        _out << lineNumber <<  ":" ;
    }

    switch ( level ) {
    case Debug:
        _out << " debug: ";
        break;
    case Info:
        _out << " info: ";
        break;
    case Warning:
        _out << " warning: ";
        break;
    case Error:
        _out << " error: ";
        break;
    case Critical:
        _out << " critical: ";
        break;
    }

    _out << message << std::endl ;
}


///
///
///
const Logger::Level& Logger::logLevel() const
{
    return _logLevel ;
}

///
///
///
void Logger::setLogLevel( const Level& logLevel )
{
    _logLevel = logLevel ;
}

///
///
///
Logger::Logger( std::ostream& str ):
    _logLevel( Warning ),
    _displayFilePosition( true ),
    _out( str.rdbuf() )
{

}

///
///
///
Logger& logger()
{
    return * Logger::get() ;
}


}//SFCGAL




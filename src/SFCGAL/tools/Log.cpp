#include <iostream>
#include <boost/date_time.hpp>

#include <SFCGAL/tools/Log.h>

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
	const Level & level,
	const boost::format& message,
	const std::string & filename,
	const int & lineNumber
)
{
	log( level, message.str(), filename, lineNumber );
}

///
///
///
void Logger::log(
	const Level & level,
	const std::string& message,
	const std::string & filename,
	const int & lineNumber
)
{
	using namespace boost::posix_time;

	if ( level < _logLevel ){
		return ;
	}

	ptime now = second_clock::local_time();
	_out << to_iso_string(now) << ":" ;

	if ( _displayFilePosition && ! filename.empty() ){
		_out << filename << ":" ;
	}
	if ( _displayFilePosition && lineNumber >= 0 ){
		_out << lineNumber <<  ":" ;
	}
	std::cout << message << std::endl ;

}


///
///
///
const Logger::Level & Logger::logLevel() const
{
	return _logLevel ;
}

///
///
///
void Logger::setLogLevel( const Level & logLevel )
{
	_logLevel = logLevel ;
}

///
///
///
Logger::Logger( std::ostream& str ):
	_logLevel( Warning ),
	_displayFilePosition(false),
	_out( str.rdbuf() )
{

}

///
///
///
Logger & logger()
{
	return * Logger::get() ;
}


}//SFCGAL




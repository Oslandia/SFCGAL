#include <iostream>

#include <SFCGAL/tools/Log.h>

#include <boost/date_time.hpp>


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
	static Logger log;
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
	std::cout << to_iso_string(now) << ":" ;

	if ( _displayFilePosition && ! filename.empty() ){
		std::cout << filename << ":" ;
	}
	if ( _displayFilePosition && lineNumber >= 0 ){
		std::cout << lineNumber <<  ":" ;
	}
	std::cout << message << std::endl ;

	if ( _autoflush ){
		flush();
	}
}


///
///
///
void Logger::flush()
{
	std::cout.flush() ;
}

///
///
///
void Logger::setAutoflush( bool autoflush )
{
	_autoflush = autoflush ;
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
Logger::Logger():
	_logLevel( Info ),
	_autoflush(true),
	_displayFilePosition(false)
{

}


}//SFCGAL




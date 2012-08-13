#include <iostream>
#include <boost/date_time.hpp>

#include <SFCGAL/tools/Log.h>

// namespace SFCGAL
// {
// 	LogSink::LogSink( std::ostream& str ) :
// 		str_(str),
// 		autoflush_( true ) {}
	
// 	std::streamsize LogSink::write(const char* s, std::streamsize n)
// 	{
// 		if ( autoflush_ ) {
// 			str_.write( s, n );
// 		}
// 		return n;
// 	}
	
// 	void LogSink::autoflush( bool state )
// 	{
// 		if ( state == true ) {
// 			flush();
// 		}
// 		autoflush_ = state;
// 	}
	
// 	void LogSink::flush()
// 	{
// 		str_.write( buffer_.c_str(), buffer_.size() );
// 		buffer_.clear();
// 		str_.flush();
// 	}
	
// 	std::string& LogSink::buffer()
// 	{
// 		return buffer_;
// 	}
	
// 	SFCGAL::Log& Logger::get()
// 	{
// 		static Log log( std::cout );
// 		return log;

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
	_logLevel( Warning ),
	_autoflush(true),
	_displayFilePosition(false)
{

}


}//SFCGAL




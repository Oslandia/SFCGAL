#include <iostream>

#include <SFCGAL/tools/Log.h>

namespace SFCGAL
{
	LogSink::LogSink( std::ostream& str ) :
		str_(str),
		autoflush_( true ) {}
	
	std::streamsize LogSink::write(const char* s, std::streamsize n)
	{
		if ( autoflush_ ) {
			str_.write( s, n );
		}
		return n;
	}
	
	void LogSink::autoflush( bool state )
	{
		if ( state == true ) {
			flush();
		}
		autoflush_ = state;
	}
	
	void LogSink::flush()
	{
		str_.write( buffer_.c_str(), buffer_.size() );
		buffer_.clear();
		str_.flush();
	}
	
	std::string& LogSink::buffer()
	{
		return buffer_;
	}
	
	SFCGAL::Log& Logger::get()
	{
		static Log log( std::cout );
		return log;
	}

}

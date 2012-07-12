#include <iostream>

#include <SFCGAL/tools/Log.h>

namespace SFCGAL
{
	void Logger::log( int level, const std::string& msg )
	{
		if ( autoflush_ ) {
			std::cout << msg << std::endl;
		}
		else {
			lines_.push_back( msg );
		}
	}

	void Logger::flush()
	{
		while ( lines_.size() > 0 ) {
			std::cout << lines_.front() << std::endl;
			lines_.pop_front();
		}
	}

	Logger::~Logger()
	{
		// flush on destruction if needed
		flush();
	}
}

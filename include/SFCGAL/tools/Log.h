#ifndef _SFCGAL_LOG_H_
#define _SFCGAL_LOG_H_

#include <string>
#include <deque>
#include <vector>

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

namespace SFCGAL
{
	class LogSink {
	public:
		typedef char                       char_type;
		typedef boost::iostreams::sink_tag category;
		
		LogSink( std::ostream& str );		

		std::streamsize write(const char* s, std::streamsize n);
		void autoflush( bool state );
		void flush();
		
		std::string& buffer();

	private:
		std::ostream& str_;
		std::string buffer_;
		bool autoflush_;
	};


	/// Log class declaration
	typedef boost::iostreams::stream<LogSink> Log;
	
	///
	/// Log singleton class
	class Logger
	{
	public:
		static Log& get();
	};
}

#define SFCGAL_LOG( level, msg ) do { SFCGAL::Logger::get() << "[" << (level) << " " << __FILE__ << ":" << __LINE__ << "] " << msg << std::endl; } while (0)

#ifndef NDEBUG
#  define LOG_DEBUG( msg ) do { SFCGAL_LOG( "DEBUG", msg ); } while(0)
#else
#  define LOG_DEBUG( msg ) do {} while(0)
#endif
#define LOG_NOTICE( msg ) do { SFCGAL_LOG( "NOTICE", msg ); } while(0)
#define LOG_ERROR( msg ) do { SFCGAL_LOG( "ERROR", msg ); } while(0)

#endif

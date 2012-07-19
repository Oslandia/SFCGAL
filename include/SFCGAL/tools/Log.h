#ifndef _SFCGAL_LOG_H_
#define _SFCGAL_LOG_H_

#include <string>
#include <boost/format.hpp>

/**
 *
 * Helper method to log debug message
 *
 * \code
 * SFCGAL_DEBUG( "start new method" ) ;
 * \endcode
 */
#define SFCGAL_DEBUG( message ) SFCGAL::Logger::get()->log( SFCGAL::Logger::Debug, message, __FILE__, __LINE__ )
/**
 *
 * Helper method to log information message
 *
 * \code
 * SFCGAL_INFO( "start new method" ) ;
 * \endcode
 */
#define SFCGAL_INFO( message ) SFCGAL::Logger::get()->log( SFCGAL::Logger::Info, message, __FILE__, __LINE__ )
/**
 *
 * Helper method to log warning message
 *
 * \code
 * SFCGAL_WARNING( "start new method" ) ;
 * \endcode
 */
#define SFCGAL_WARNING( message ) SFCGAL::Logger::get()->log( SFCGAL::Logger::Warning, message, __FILE__, __LINE__ )
/**
 *
 * Helper method to log error message
 *
 * \code
 * SFCGAL_ERROR( "invalid geometry" ) ;
 * \endcode
 */
#define SFCGAL_ERROR( message ) SFCGAL::Logger::get()->log( SFCGAL::Logger::Info, message, __FILE__, __LINE__ )
/**
 *
 * Helper method to log critical message
 *
 * \code
 * SFCGAL_ERROR( "unexpected behavior in triangulate" ) ;
 * \endcode
 */
#define SFCGAL_CRITICAL( message ) SFCGAL::Logger::get()->log( SFCGAL::Logger::Critical, message, __FILE__, __LINE__ )


namespace SFCGAL {

	/**
	 * [Singleton]Logger class
	 *
	 * @warning saved_lines and co removed (dangerous for memory and could be done in a LogWriter).
	 */
	class Logger
	{
	public:
		/**
		 * destructor
		 */
		~Logger();

		/**
		 * log level
		 */
		enum Level
		{
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		/**
		 * singleton accessor
		 */
		static Logger* get() ;

		/**
		 * log a message using boost format
		 * @param level the log level
		 * @param message the message to log
		 * @param filename the filename (optional)
		 * @param lineNumber the line number in the file (optional)
		 */
		void log(
			const Level & level,
			const boost::format& message,
			const std::string & filename = "",
			const int & lineNumber = -1
		);

		/**
		 * log a message
		 * @param level the log level
		 * @param message the message to log
		 * @param filename the filename (optional)
		 * @param lineNumber the line number in the file (optional)
		 */
		void log(
			const Level & level,
			const std::string& message,
			const std::string & filename = "",
			const int & lineNumber = -1
		);

		/**
		 * flush log outputs (actualy std::cout.flush())
		 */
		void flush();
		/**
		 * indicates if each log output may be flushed
		 */
		void setAutoflush( bool autoflush ) ;

		/**
		 * get the current log level
		 */
		const Level & logLevel() const ;
		/**
		 * set the log level
		 */
		void setLogLevel( const Level & logLevel ) ;

	private:
		/**
		 * current log level
		 */
		Level _logLevel ;
		/**
		 * automatic call to flush() after each log?
		 */
		bool _autoflush ;
		/**
		 * display file position?
		 */
		bool _displayFilePosition ;

		/**
		 * private constructor
		 */
		Logger();
		/**
		 * no copy constructor
		 */
		Logger( const Logger & other );
	};


}//SFCGAL






#endif

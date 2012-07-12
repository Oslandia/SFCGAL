#ifndef _SFCGAL_LOG_H_
#define _SFCGAL_LOG_H_

#include <string>
#include <deque>
#include <vector>

namespace SFCGAL
{

	//
	// Yet-to-enhance logger class
	class Logger
	{
	public:
		~Logger();

		enum Level
		{
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		// singleton accessor
		static Logger* get()
		{
			static Logger log;
			return &log;
		}

		void log( int level, const std::string& msg );

		void flush();
		void enable_autoflush() { autoflush_ = true; }
		void disable_autoflush() { autoflush_ = false; }

		std::vector<std::string> saved_lines() const { return std::vector<std::string>(lines_.begin(), lines_.end()); }
	private:
		// private constructor
	Logger() : autoflush_(true) {}

		std::deque<std::string> lines_;
		bool autoflush_;
	};
}

#endif

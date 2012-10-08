#ifndef _IGN_EXCEPTION_H_
#define _IGN_EXCEPTION_H_

#include <boost/exception/all.hpp>

#include <string>


namespace SFCGAL {

	/**
	 * Base SFCGAL Exception
	 *
	 * \code
	 * BOOST_THROW_EXCEPTION( Exception("invalid geometry") );
	 * \endcode
	 */
	class Exception : public virtual boost::exception, public virtual  std::exception {
	public:
		Exception() throw();
		Exception( std::string const& message ) throw();
		virtual ~Exception() throw();
		
		/**
		 * returns the exception message
		 */
		virtual const char* what() const throw();
		/**
		 * returns diagnostic informations (file, line, etc.)
		 */
		std::string diagnostic() const throw();
	protected:
		std::string _message;
	};
}




#endif



#include <SFCGAL/Exception.h>


namespace SFCGAL {

///
///
///
Exception::Exception() throw():
	_message("unknown exception")
{

}


///
///
///
Exception::Exception( std::string const& message ) throw():
	_message(message)
{

}

///
///
///
Exception::~Exception() throw()
{

}

///
///
///
const char* Exception::what() const throw()
{
	return _message.c_str() ;
}

///
///
///
std::string Exception::diagnostic() const throw()
{
	return boost::diagnostic_information(*this) ;
}


}//SFCGAL




#ifndef _SFCGAL_BENCH_H_
#define _SFCGAL_BENCH_H_

#include <boost/chrono.hpp>

#define SFCGAL_BENCH( _invoke_ )\
{\
	boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();\
	_invoke_;\
	boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;\
	std::cout << #_invoke_ << " : " << sec.count() << " s"<< std::endl ;\
}\


#endif

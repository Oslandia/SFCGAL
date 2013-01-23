#include "Bench.h"


namespace SFCGAL {

///
///
///
Bench::~Bench()
{

}

///
///
///
void Bench::start( const std::string & description )
{
	_timers.push( std::make_pair( description, timer_t() ) );
	_timers.top().second.start();
}

///
///
///
void Bench::start( const boost::basic_format<char>& description )
{
	start( description.str() );
}

///
///
///
void Bench::stop()
{
	BOOST_ASSERT( ! _timers.empty() ) ;
	_timers.top().second.stop();
	s() << _timers.top().first << "\t" << ( _timers.top().second.elapsed().wall * 1.0e-9 ) << std::endl ;
	_timers.pop() ;
}

///
///
///
Bench & Bench::instance()
{
	static Bench bench ;
	return bench ;
}

///
///
///
std::ostream & Bench::s()
{
	return *_s ;
}

///
///
///
Bench::Bench():
	_s( &std::cout )
{

}


} // namespace SFCGAL


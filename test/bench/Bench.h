#ifndef _SFCGAL_BENCH_H_
#define _SFCGAL_BENCH_H_

#include <iostream>
#include <stack>

#include <boost/timer/timer.hpp>

#include <boost/format.hpp>

namespace SFCGAL {

/**
 * @brief helper class to write formated benchs
 */
class Bench {
public:
    typedef boost::timer::cpu_timer timer_t ;

    /**
     * destructor
     */
    ~Bench();

    /**
     * start a bench
     */
    void start( const std::string& description ) ;
    /**
     * start a bench
     */
    void start( const boost::basic_format<char>& description ) ;
    /**
     * stop a bench
     */
    void stop() ;

    /**
     * get bench instance
     */
    static Bench& instance() ;

    /**
     * Get output stream
     */
    std::ostream& s() ;
private:
    /**
     * default constructor
     */
    Bench();
    /**
     * copy constructor
     */
    Bench( const Bench& bench );

    /**
     * output stream to write bench result (default is std::cout)
     */
    std::ostream* _s ;
    /**
     * timer stack with description
     */
    std::stack< std::pair< std::string, timer_t > > _timers ;
};

/**
 * @Get bench instance
 */
inline Bench& bench()
{
    return Bench::instance() ;
}

} // namespace SFCGAL

#endif


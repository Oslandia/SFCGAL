#include <SFCGAL/detail/CoordinateProperties.h>

#include <SFCGAL/numeric.h>

namespace SFCGAL {
namespace detail {

///
///
///
CoordinateProperties::CoordinateProperties():
    _m( SFCGAL::NaN() )
{
    
}

///
///
///
bool CoordinateProperties::hasM() const 
{
    return ! SFCGAL::isNaN( _m ) ;
}


    
} // namespace detail
} // namespace SFCGAL



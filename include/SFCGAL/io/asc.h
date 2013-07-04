#ifndef _SFCGAL_IO_ASC_H_
#define _SFCGAL_IO_ASC_H_

#include <SFCGAL/config.h>

#include <sstream>

#include <SFCGAL/Grid.h>

namespace SFCGAL {
namespace io {

/**
 * @brief read ASC grid from input stream
 *
 * @see http://en.wikipedia.org/wiki/Esri_grid
 */
SFCGAL_API std::auto_ptr< Grid > readASC( std::istream& s ) ;


} // namespace io
} // namespace SFCGAL

#endif
